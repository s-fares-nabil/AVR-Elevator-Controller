
/* To use busy loop delay function */
/* Pulls in avr-libc's own uint8_t/uint16_t/... (via stdint.h), so we don't
 * also include STD_TYPES.h here: including both in the same file makes
 * the compiler see two conflicting typedefs for the same names */
#include <util/delay.h>

/* Should be included to use the interfaces of the DC Motor */
#include "HDC_Motor_interface.h"
#include "HSWITCH_interface.h"
#include "HSSD_interface.h"
#include "HLED_interface.h"

/* 1 ms, determined in real systems by design not simple, not random */
#define TICK_TIME (1)

/* Wait for the raw pin level to become stable before trusting a press or release */
#define DEBOUNCE_PERIOD_MS (20U)

/* Simulated time to travel one floor, no position feedback so this is timed */
#define FLOOR_TRAVEL_DURATION_MS (2000U)

/* How long the door motor runs for one open (or close) cycle */
#define DOOR_MOVE_DURATION_MS (2000U)

/* How long the door stays open before it auto-closes */
#define DOOR_OPEN_HOLD_MS (3000U)

#define NUMBER_OF_FLOORS (6)

/* 6 floor buttons + door-open + door-close */
#define NUMBER_OF_BUTTONS (8)
#define DOOR_OPEN_BUTTON_INDEX (6)
#define DOOR_CLOSE_BUTTON_INDEX (7)

typedef enum {
    ELEV_IDLE = 0,
    ELEV_MOVING_UP = 1,
    ELEV_MOVING_DOWN = 2,
    ELEV_DOOR_OPENING = 3,
    ELEV_DOOR_OPEN = 4,
    ELEV_DOOR_CLOSING = 5,
} enum_ElevatorState_t;

typedef enum {
    DIRECTION_NONE = 0,
    DIRECTION_UP = 1,
    DIRECTION_DOWN = 2,
} enum_ElevatorDirection_t;

typedef enum {
    REQUEST_NONE = 0,
    REQUEST_PENDING = 1,
} enum_FloorRequestFlag_t;

/* Whether the state we're currently in still has to run its entry action */
typedef enum {
    ENTRY_ACTION_DONE = 0,
    ENTRY_ACTION_PENDING = 1,
} enum_EntryActionFlag_t;

typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESS_DEBOUNCE = 1,
    BUTTON_PRESSED = 2,
    BUTTON_RELEASE_DEBOUNCE = 3,
} enum_ButtonFSMState_t;

/* One entry per button: its switch object plus its own independent debounce FSM */
typedef struct {
    st_SwitchObject_t m_Switch;
    enum_ButtonFSMState_t m_FSMState;
    uint16_t m_ElapsedTimeMs;
} st_DebouncedButton_t;

int main(void) {
    /* Init Section */
    enum_ElevatorState_t current_elevator_state = ELEV_IDLE;

    /* Every state starts by running its entry action once */
    enum_EntryActionFlag_t is_state_entry = ENTRY_ACTION_PENDING;

    /* Which floor the cabin is currently at, 1 to NUMBER_OF_FLOORS */
    uint8_t current_floor = 1;

    /* Which way we're currently sweeping, persists across door stops */
    enum_ElevatorDirection_t current_direction = DIRECTION_NONE;

    /* One flag per floor, set by that floor's button, cleared once served */
    enum_FloorRequestFlag_t request_pending[NUMBER_OF_FLOORS] = {
        REQUEST_NONE, REQUEST_NONE, REQUEST_NONE,
        REQUEST_NONE, REQUEST_NONE, REQUEST_NONE,
    };

    /* Define the Climb Motor Object, moves the cabin up/down between floors */
    st_DCMotor_Ojbect_t climb_motor = {
        .m_ForwardPin = {PORT_NUMBER_A, PIN_NUMBER_0},
        .m_BackwardPin = {PORT_NUMBER_A, PIN_NUMBER_1},

        .m_State = DCM_STOP
    };

    /* Define the Door Motor Object, opens/closes the cabin door */
    st_DCMotor_Ojbect_t door_motor = {
        .m_ForwardPin = {PORT_NUMBER_B, PIN_NUMBER_1},
        .m_BackwardPin = {PORT_NUMBER_B, PIN_NUMBER_0},

        .m_State = DCM_STOP
    };

    /* Displaying which floor the cabin is currently at */
    st_SSD_Object_t ssd_object = {
        .m_aLed = {PORT_NUMBER_C, PIN_NUMBER_6},
        .m_bLed = {PORT_NUMBER_C, PIN_NUMBER_5},
        .m_cLed = {PORT_NUMBER_C, PIN_NUMBER_4},
        .m_dLed = {PORT_NUMBER_C, PIN_NUMBER_3},
        .m_eLed = {PORT_NUMBER_C, PIN_NUMBER_2},
        .m_fLed = {PORT_NUMBER_C, PIN_NUMBER_1},
        .m_gLed = {PORT_NUMBER_C, PIN_NUMBER_0},

        .m_EnablePin = {PORT_NUMBER_D, PIN_NUMBER_7},

        .m_CommonType = SSD_COMMON_CATHODE,
    };

    /* Direction Leds, reflect current_direction, not just "motor running" */
    st_LedObject_t up_direction_led = {PORT_NUMBER_A, PIN_NUMBER_6, LED_OFF};
    st_LedObject_t down_direction_led = {PORT_NUMBER_A, PIN_NUMBER_7, LED_OFF};

    /* On while the door is open or on its way to being open */
    st_LedObject_t door_open_led = {PORT_NUMBER_A, PIN_NUMBER_2, LED_OFF};

    /* 6 floor buttons + door-open + door-close, each with its own debounce FSM */
    st_DebouncedButton_t buttons[NUMBER_OF_BUTTONS] = {
        {{PORT_NUMBER_D, PIN_NUMBER_2, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
        {{PORT_NUMBER_D, PIN_NUMBER_3, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
        {{PORT_NUMBER_B, PIN_NUMBER_7, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
        {{PORT_NUMBER_D, PIN_NUMBER_0, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
        {{PORT_NUMBER_B, PIN_NUMBER_2, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
        {{PORT_NUMBER_B, PIN_NUMBER_3, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
        {{PORT_NUMBER_D, PIN_NUMBER_6, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
        {{PORT_NUMBER_D, PIN_NUMBER_5, SWITCH_RELEASED}, BUTTON_RELEASED, 0},
    };

    /* How long we've been in the current elevator state, drives floor ticks and door timing */
    uint16_t elevator_state_elapsed_time_ms = 0;

    HDCM_voidInit(&climb_motor);
    HDCM_voidInit(&door_motor);

    HSSD_voidInit(&ssd_object);
    HSSD_voidEnableOneSSD(&ssd_object);
    HSSD_voidDisplayNumber(&ssd_object, current_floor);

    HLED_voidInit(&up_direction_led);
    HLED_voidInit(&down_direction_led);
    HLED_voidInit(&door_open_led);

    for(uint8_t button_index = 0; button_index < NUMBER_OF_BUTTONS; button_index++) {
        HSWTICH_voidInit(&buttons[button_index].m_Switch);
    }

    /* Run time Section */
    while (1) {
        /* Tick one time then increament each independent task's counter */
        /* The tick time is determined by design not random */
        _delay_ms(TICK_TIME);
        elevator_state_elapsed_time_ms++;

        /* One loop drives all 8 buttons through the same debounce FSM */
        for(uint8_t button_index = 0; button_index < NUMBER_OF_BUTTONS; button_index++) {
            buttons[button_index].m_ElapsedTimeMs++;

            /* Read Your inputs at first */
            HSWTICH_voidReadState(&buttons[button_index].m_Switch);

            switch(buttons[button_index].m_FSMState) {
                case BUTTON_RELEASED:
                    if(buttons[button_index].m_Switch.m_SwitchState == SWITCH_PRESSED) {
                        buttons[button_index].m_FSMState = BUTTON_PRESS_DEBOUNCE;
                        buttons[button_index].m_ElapsedTimeMs = 0;
                    }
                break;

                case BUTTON_PRESS_DEBOUNCE:
                    if(buttons[button_index].m_Switch.m_SwitchState == SWITCH_RELEASED) {
                        /* Bounced back before it got stable, reject it */
                        buttons[button_index].m_FSMState = BUTTON_RELEASED;
                        buttons[button_index].m_ElapsedTimeMs = 0;
                    }
                    else if(buttons[button_index].m_ElapsedTimeMs >= DEBOUNCE_PERIOD_MS) {
                        /* Press confirmed stable, one press, one action */
                        if(button_index < NUMBER_OF_FLOORS) {
                            /* Floor request: just sets a flag, never touches a motor directly */
                            request_pending[button_index] = REQUEST_PENDING;
                        }
                        else if(button_index == DOOR_OPEN_BUTTON_INDEX) {
                            if(current_elevator_state == ELEV_IDLE) {
                                current_elevator_state = ELEV_DOOR_OPENING;
                                is_state_entry = ENTRY_ACTION_PENDING;
                                elevator_state_elapsed_time_ms = 0;
                            }
                            else if(current_elevator_state == ELEV_DOOR_OPEN) {
                                /* Someone still wants the door open, extend the hold */
                                elevator_state_elapsed_time_ms = 0;
                            }
                            else if(current_elevator_state == ELEV_DOOR_CLOSING) {
                                /* Interrupt the close, full restart of the open cycle */
                                HDCM_voidStop(&door_motor);
                                HDCM_voidMoveForward(&door_motor);
                                HLED_voidTurnOn(&door_open_led);

                                current_elevator_state = ELEV_DOOR_OPENING;
                                is_state_entry = ENTRY_ACTION_DONE;
                                elevator_state_elapsed_time_ms = 0;
                            }
                            /* Ignored while moving or already opening */
                        }
                        else { /* DOOR_CLOSE_BUTTON_INDEX */
                            if(current_elevator_state == ELEV_DOOR_OPEN) {
                                current_elevator_state = ELEV_DOOR_CLOSING;
                                is_state_entry = ENTRY_ACTION_PENDING;
                                elevator_state_elapsed_time_ms = 0;
                            }
                            /* Ignored everywhere else */
                        }

                        buttons[button_index].m_FSMState = BUTTON_PRESSED;
                        buttons[button_index].m_ElapsedTimeMs = 0;
                    }
                break;

                case BUTTON_PRESSED:
                    /* Holding does nothing more, wait for the release */
                    if(buttons[button_index].m_Switch.m_SwitchState == SWITCH_RELEASED) {
                        buttons[button_index].m_FSMState = BUTTON_RELEASE_DEBOUNCE;
                        buttons[button_index].m_ElapsedTimeMs = 0;
                    }
                break;

                case BUTTON_RELEASE_DEBOUNCE:
                    if(buttons[button_index].m_Switch.m_SwitchState == SWITCH_PRESSED) {
                        /* Bounced back to pressed before it got stable */
                        buttons[button_index].m_FSMState = BUTTON_PRESSED;
                        buttons[button_index].m_ElapsedTimeMs = 0;
                    }
                    else if(buttons[button_index].m_ElapsedTimeMs >= DEBOUNCE_PERIOD_MS) {
                        /* Release confirmed stable, ready for the next press */
                        buttons[button_index].m_FSMState = BUTTON_RELEASED;
                        buttons[button_index].m_ElapsedTimeMs = 0;
                    }
                break;
            }
        }

        /* Structure the elevator state machine via switch case */
        switch(current_elevator_state) {
            case ELEV_IDLE:
                if(is_state_entry == ENTRY_ACTION_PENDING) {
                    /* On entry we're not going anywhere, both direction leds off */
                    HLED_voidTurnOff(&up_direction_led);
                    HLED_voidTurnOff(&down_direction_led);

                    is_state_entry = ENTRY_ACTION_DONE;
                }

                /* Dispatch runs every tick while idling, not just on entry */
                if(request_pending[current_floor - 1] == REQUEST_PENDING) {
                    /* Requested floor is right here, no need to move */
                    request_pending[current_floor - 1] = REQUEST_NONE;

                    current_elevator_state = ELEV_DOOR_OPENING;
                    is_state_entry = ENTRY_ACTION_PENDING;
                    elevator_state_elapsed_time_ms = 0;
                }
                else {
                    uint8_t floor_index;
                    uint8_t found_above = 0;
                    uint8_t found_below = 0;

                    for(floor_index = current_floor; floor_index < NUMBER_OF_FLOORS; floor_index++) {
                        if(request_pending[floor_index] == REQUEST_PENDING) {
                            found_above = 1;
                            break;
                        }
                    }

                    for(floor_index = 0; floor_index < current_floor - 1; floor_index++) {
                        if(request_pending[floor_index] == REQUEST_PENDING) {
                            found_below = 1;
                            break;
                        }
                    }

                    if(current_direction == DIRECTION_UP && found_above) {
                        current_elevator_state = ELEV_MOVING_UP;
                        is_state_entry = ENTRY_ACTION_PENDING;
                        elevator_state_elapsed_time_ms = 0;
                    }
                    else if(current_direction == DIRECTION_DOWN && found_below) {
                        current_elevator_state = ELEV_MOVING_DOWN;
                        is_state_entry = ENTRY_ACTION_PENDING;
                        elevator_state_elapsed_time_ms = 0;
                    }
                    else if(found_above) {
                        current_direction = DIRECTION_UP;
                        current_elevator_state = ELEV_MOVING_UP;
                        is_state_entry = ENTRY_ACTION_PENDING;
                        elevator_state_elapsed_time_ms = 0;
                    }
                    else if(found_below) {
                        current_direction = DIRECTION_DOWN;
                        current_elevator_state = ELEV_MOVING_DOWN;
                        is_state_entry = ENTRY_ACTION_PENDING;
                        elevator_state_elapsed_time_ms = 0;
                    }
                    /* else nothing pending anywhere, stay idle */
                }
            break;

            case ELEV_MOVING_UP:
                if(is_state_entry == ENTRY_ACTION_PENDING) {
                    /* On entry show we're going up and start climbing */
                    HLED_voidTurnOff(&down_direction_led);
                    HLED_voidTurnOn(&up_direction_led);
                    HDCM_voidMoveForward(&climb_motor);

                    is_state_entry = ENTRY_ACTION_DONE;
                }

                /* Treat every FLOOR_TRAVEL_DURATION_MS as "arrived at the next floor" */
                if(elevator_state_elapsed_time_ms % FLOOR_TRAVEL_DURATION_MS == 0) {
                    current_floor++;
                    HSSD_voidDisplayNumber(&ssd_object, current_floor);

                    if(request_pending[current_floor - 1] == REQUEST_PENDING) {
                        /* This floor was requested, stop here and open the door */
                        request_pending[current_floor - 1] = REQUEST_NONE;
                        HDCM_voidStop(&climb_motor);
                        HLED_voidTurnOff(&up_direction_led);

                        current_elevator_state = ELEV_DOOR_OPENING;
                        is_state_entry = ENTRY_ACTION_PENDING;
                        elevator_state_elapsed_time_ms = 0;
                    }
                    /* else keep climbing, elapsed_time_ms keeps counting toward the next floor */
                }
            break;

            case ELEV_MOVING_DOWN:
                if(is_state_entry == ENTRY_ACTION_PENDING) {
                    /* On entry show we're going down and start descending */
                    HLED_voidTurnOff(&up_direction_led);
                    HLED_voidTurnOn(&down_direction_led);
                    HDCM_voidMoveBackward(&climb_motor);

                    is_state_entry = ENTRY_ACTION_DONE;
                }

                /* Treat every FLOOR_TRAVEL_DURATION_MS as "arrived at the next floor" */
                if(elevator_state_elapsed_time_ms % FLOOR_TRAVEL_DURATION_MS == 0) {
                    current_floor--;
                    HSSD_voidDisplayNumber(&ssd_object, current_floor);

                    if(request_pending[current_floor - 1] == REQUEST_PENDING) {
                        /* This floor was requested, stop here and open the door */
                        request_pending[current_floor - 1] = REQUEST_NONE;
                        HDCM_voidStop(&climb_motor);
                        HLED_voidTurnOff(&down_direction_led);

                        current_elevator_state = ELEV_DOOR_OPENING;
                        is_state_entry = ENTRY_ACTION_PENDING;
                        elevator_state_elapsed_time_ms = 0;
                    }
                    /* else keep descending, elapsed_time_ms keeps counting toward the next floor */
                }
            break;

            case ELEV_DOOR_OPENING:
                if(is_state_entry == ENTRY_ACTION_PENDING) {
                    /* On entry start opening the door */
                    HDCM_voidMoveForward(&door_motor);
                    HLED_voidTurnOn(&door_open_led);

                    is_state_entry = ENTRY_ACTION_DONE;
                }

                if(elevator_state_elapsed_time_ms >= DOOR_MOVE_DURATION_MS) {
                    /* Door has fully opened */
                    HDCM_voidStop(&door_motor);

                    current_elevator_state = ELEV_DOOR_OPEN;
                    is_state_entry = ENTRY_ACTION_PENDING;
                    elevator_state_elapsed_time_ms = 0;
                }
            break;

            case ELEV_DOOR_OPEN:
                if(is_state_entry == ENTRY_ACTION_PENDING) {
                    /* On entry nothing to do, just hold, the door motor is already stopped */
                    is_state_entry = ENTRY_ACTION_DONE;
                }

                /* Auto close once the hold time is up, the door-close button (handled in the
                   button loop above) can also cut this short by transitioning early */
                if(elevator_state_elapsed_time_ms >= DOOR_OPEN_HOLD_MS) {
                    current_elevator_state = ELEV_DOOR_CLOSING;
                    is_state_entry = ENTRY_ACTION_PENDING;
                    elevator_state_elapsed_time_ms = 0;
                }
            break;

            case ELEV_DOOR_CLOSING:
                if(is_state_entry == ENTRY_ACTION_PENDING) {
                    /* On entry start closing the door, it's no longer open or opening */
                    HDCM_voidMoveBackward(&door_motor);
                    HLED_voidTurnOff(&door_open_led);

                    is_state_entry = ENTRY_ACTION_DONE;
                }

                /* The door-open button interrupt (handled in the button loop above) can
                   restart the open cycle before this ever fires */
                if(elevator_state_elapsed_time_ms >= DOOR_MOVE_DURATION_MS) {
                    /* Door has fully closed */
                    HDCM_voidStop(&door_motor);

                    /* Dispatch again, same logic as ELEV_IDLE's per-tick dispatch above */
                    if(request_pending[current_floor - 1] == REQUEST_PENDING) {
                        /* Requested floor is right here, no need to move */
                        request_pending[current_floor - 1] = REQUEST_NONE;

                        current_elevator_state = ELEV_DOOR_OPENING;
                        is_state_entry = ENTRY_ACTION_PENDING;
                        elevator_state_elapsed_time_ms = 0;
                    }
                    else {
                        uint8_t floor_index;
                        uint8_t found_above = 0;
                        uint8_t found_below = 0;

                        for(floor_index = current_floor; floor_index < NUMBER_OF_FLOORS; floor_index++) {
                            if(request_pending[floor_index] == REQUEST_PENDING) {
                                found_above = 1;
                                break;
                            }
                        }

                        for(floor_index = 0; floor_index < current_floor - 1; floor_index++) {
                            if(request_pending[floor_index] == REQUEST_PENDING) {
                                found_below = 1;
                                break;
                            }
                        }

                        if(current_direction == DIRECTION_UP && found_above) {
                            current_elevator_state = ELEV_MOVING_UP;
                            is_state_entry = ENTRY_ACTION_PENDING;
                            elevator_state_elapsed_time_ms = 0;
                        }
                        else if(current_direction == DIRECTION_DOWN && found_below) {
                            current_elevator_state = ELEV_MOVING_DOWN;
                            is_state_entry = ENTRY_ACTION_PENDING;
                            elevator_state_elapsed_time_ms = 0;
                        }
                        else if(found_above) {
                            current_direction = DIRECTION_UP;
                            current_elevator_state = ELEV_MOVING_UP;
                            is_state_entry = ENTRY_ACTION_PENDING;
                            elevator_state_elapsed_time_ms = 0;
                        }
                        else if(found_below) {
                            current_direction = DIRECTION_DOWN;
                            current_elevator_state = ELEV_MOVING_DOWN;
                            is_state_entry = ENTRY_ACTION_PENDING;
                            elevator_state_elapsed_time_ms = 0;
                        }
                        else {
                            /* Nothing pending anywhere, go idle */
                            current_direction = DIRECTION_NONE;
                            current_elevator_state = ELEV_IDLE;
                            is_state_entry = ENTRY_ACTION_PENDING;
                            elevator_state_elapsed_time_ms = 0;
                        }
                    }
                }
            break;
        }
    }

    return 0;
}
