#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "MDIO_interface.h"

/* The work done is in the mode function is to go to the control register and set the mode */
#define DDRA  (* ( (volatile uint8_t*) (0x3A) ) )
#define DDRB  (* ( (volatile uint8_t*) (0x37) ) )
#define DDRC  (* ( (volatile uint8_t*) (0x34) ) )
#define DDRD  (* ( (volatile uint8_t*) (0x31) ) )

/* if DDR input, PORT is for control, if output: it's for data */
#define PORTA  (* ( (volatile uint8_t*) (0x3B) ) )
#define PORTB  (* ( (volatile uint8_t*) (0x38) ) )
#define PORTC  (* ( (volatile uint8_t*) (0x35) ) )
#define PORTD  (* ( (volatile uint8_t*) (0x32) ) )

/* DIO Status Register PIN */
#define PINA  (* ( (volatile uint8_t*) (0x39) ) )
#define PINB  (* ( (volatile uint8_t*) (0x36) ) )
#define PINC  (* ( (volatile uint8_t*) (0x33) ) )
#define PIND  (* ( (volatile uint8_t*) (0x30) ) )



/*
 * @brief Configure the DIO Pin Mode
 *
 * This function configure the DIO Pin, we have three modes in DIO: 
 * output, input floating, input_pull_up
 *
 * @param PIN_PORT, PIN_NO, PIN_MODE
 *
 * @return void
 **/
void MDIO_voidSet_Pin_DIO_Mode(enum_PortNumber_t PIN_PORT,  enum_PinNumber_t PIN_NO, 
    enum_PinMode_t PIN_MODE) {
        /* Define Functionality */
        /* 1. Select PIN_PORT: Which Port First */
        /* 2. Select PIN_NO: Which Pin inside the selected port */
        /* 3. Set the Mode */
        switch(PIN_PORT) {
            case PORT_NUMBER_A: 
                switch(PIN_MODE) {
                    case PIN_MODE_INPUT_FLOATING: 
                        /* To select input floating DDR BIT = 0, PORT BIT = 0 */
                        CLR_BIT(DDRA, PIN_NO);
                        CLR_BIT(PORTA, PIN_NO);
                    break;
                    case PIN_MODE_INPUT_PULL_UP: 
                        /* To select input PULL_UP DDR BIT = 0, PORT BIT = 1 */
                        CLR_BIT(DDRA, PIN_NO);
                        SET_BIT(PORTA, PIN_NO);
                    break;
                    case PIN_MODE_OUTPUT: 
                        /* To select output DDR BIT = 1, PORT BIT is not control, it's data reg */
                        SET_BIT(DDRA, PIN_NO);
                        CLR_BIT(PORTA, PIN_NO); // this just to init the bit to be off (DATA not CONTROL)
                    break;
                }
            
            break;
            
            case PORT_NUMBER_B: 
                switch(PIN_MODE) {
                    case PIN_MODE_INPUT_FLOATING: 
                        /* To select input floating DDR BIT = 0, PORT BIT = 0 */
                        CLR_BIT(DDRB, PIN_NO);
                        CLR_BIT(PORTB, PIN_NO);
                    break;
                    case PIN_MODE_INPUT_PULL_UP: 
                        /* To select input PULL_UP DDR BIT = 0, PORT BIT = 1 */
                        CLR_BIT(DDRB, PIN_NO);
                        SET_BIT(PORTB, PIN_NO);
                    break;
                    case PIN_MODE_OUTPUT: 
                        /* To select input PULL_UP DDR BIT = 1, PORT BIT is not control, it's data reg */
                        SET_BIT(DDRB, PIN_NO);
                        CLR_BIT(PORTB, PIN_NO); // this just to init the bit to be off (DATA not CONTROL)
                    break;
                }
            break;
            case PORT_NUMBER_C:  
                switch(PIN_MODE) {
                    case PIN_MODE_INPUT_FLOATING: 
                        /* To select input floating DDR BIT = 0, PORT BIT = 0 */
                        CLR_BIT(DDRC, PIN_NO);
                        CLR_BIT(PORTC, PIN_NO);
                    break;
                    case PIN_MODE_INPUT_PULL_UP: 
                        /* To select input PULL_UP DDR BIT = 0, PORT BIT = 1 */
                        CLR_BIT(DDRC, PIN_NO);
                        SET_BIT(PORTC, PIN_NO);
                    break;
                    case PIN_MODE_OUTPUT: 
                        /* To select input PULL_UP DDR BIT = 1, PORT BIT is not control, it's data reg */
                        SET_BIT(DDRC, PIN_NO);
                        CLR_BIT(PORTC, PIN_NO); // this just to init the bit to be off (DATA not CONTROL)
                    break;
                }

            break;
            case PORT_NUMBER_D: 
                switch(PIN_MODE) {
                    case PIN_MODE_INPUT_FLOATING: 
                        /* To select input floating DDR BIT = 0, PORT BIT = 0 */
                        CLR_BIT(DDRD, PIN_NO);
                        CLR_BIT(PORTD, PIN_NO);
                    break;
                    case PIN_MODE_INPUT_PULL_UP: 
                        /* To select input PULL_UP DDR BIT = 0, PORT BIT = 1 */
                        CLR_BIT(DDRD, PIN_NO);
                        SET_BIT(PORTD, PIN_NO);
                    break;
                    case PIN_MODE_OUTPUT: 
                        /* To select input PULL_UP DDR BIT = 1, PORT BIT is not control, it's data reg */
                        SET_BIT(DDRD, PIN_NO);
                        CLR_BIT(PORTD, PIN_NO); // this just to init the bit to be off (DATA not CONTROL)
                    break;
                }
            break;
        }
    }

/* @brief Source the DIO Pin Volt Level 
 *
 * This function Source the DIO Pin Volt Level, if mode is <output>:
 * we can make that pin deliver high volt, or low volt
 *
 * @param PIN_PORT, PIN_NO, PIN_LEVEL
 *
 * @return void
 **/
void MDIO_voidSet_Pin_Output_Volt_Level(enum_PortNumber_t PIN_PORT,  enum_PinNumber_t PIN_NO,
    enum_PinVoltLevel_t PIN_VOLT_LEVEL) {
        /* Define Functionality */
        /* 1. Select PIN_PORT: Which Port First */
        /* 2. Select PIN_NO: Which Pin inside the selected port */
        /* 3. Set the PORT bit to the wanted level (PORT here is the DATA register, pin must be output already) */
        switch(PIN_PORT) {
            case PORT_NUMBER_A:
                switch(PIN_VOLT_LEVEL) {
                    case PIN_LOW_LEVEL:
                        /* To source LOW level, PORT BIT = 0 */
                        CLR_BIT(PORTA, PIN_NO);
                    break;
                    case PIN_HIGH_LEVEL:
                        /* To source HIGH level, PORT BIT = 1 */
                        SET_BIT(PORTA, PIN_NO);
                    break;
                }
            break;

            case PORT_NUMBER_B:
                switch(PIN_VOLT_LEVEL) {
                    case PIN_LOW_LEVEL:
                        /* To source LOW level, PORT BIT = 0 */
                        CLR_BIT(PORTB, PIN_NO);
                    break;
                    case PIN_HIGH_LEVEL:
                        /* To source HIGH level, PORT BIT = 1 */
                        SET_BIT(PORTB, PIN_NO);
                    break;
                }
            break;

            case PORT_NUMBER_C:
                switch(PIN_VOLT_LEVEL) {
                    case PIN_LOW_LEVEL:
                        /* To source LOW level, PORT BIT = 0 */
                        CLR_BIT(PORTC, PIN_NO);
                    break;
                    case PIN_HIGH_LEVEL:
                        /* To source HIGH level, PORT BIT = 1 */
                        SET_BIT(PORTC, PIN_NO);
                    break;
                }
            break;

            case PORT_NUMBER_D:
                switch(PIN_VOLT_LEVEL) {
                    case PIN_LOW_LEVEL:
                        /* To source LOW level, PORT BIT = 0 */
                        CLR_BIT(PORTD, PIN_NO);
                    break;
                    case PIN_HIGH_LEVEL:
                        /* To source HIGH level, PORT BIT = 1 */
                        SET_BIT(PORTD, PIN_NO);
                    break;
                }
            break;
        }
    }

/* @brief Read Volt Level on a pin (even if it was output) 
 *
 * This function Reads the DIO Pin Volt Level:
 * we have two levels PIN_HIGH, or PIN_LOW
 *
 * @param PIN_PORT, PIN_NO, *PIN_LEVEL: *PIN_LEVEL is the address to be updated 
 *
 * @return void
 **/
void MDIO_voidGet_Pin_State(enum_PortNumber_t PIN_PORT,  enum_PinNumber_t PIN_NO,
    enum_PinVoltLevel_t * PTR_PIN_VOLT_LEVEL) {
        /* Define Functionality */
        /* 1. Select PIN_PORT: Which Port First */
        /* 2. Select PIN_NO: Which Pin inside the selected port */
        /* 3. Read the PIN register bit and write the level back through the pointer */
        switch(PIN_PORT) {
            case PORT_NUMBER_A:
                if (GET_BIT(PINA, PIN_NO) == 1) {
                    /* PIN register bit = 1 means the pin is currently HIGH */
                    *PTR_PIN_VOLT_LEVEL = PIN_HIGH_LEVEL;
                } else {
                    /* PIN register bit = 0 means the pin is currently LOW */
                    *PTR_PIN_VOLT_LEVEL = PIN_LOW_LEVEL;
                }
            break;

            case PORT_NUMBER_B:
                if (GET_BIT(PINB, PIN_NO) == 1) {
                    /* PIN register bit = 1 means the pin is currently HIGH */
                    *PTR_PIN_VOLT_LEVEL = PIN_HIGH_LEVEL;
                } else {
                    /* PIN register bit = 0 means the pin is currently LOW */
                    *PTR_PIN_VOLT_LEVEL = PIN_LOW_LEVEL;
                }
            break;

            case PORT_NUMBER_C:
                if (GET_BIT(PINC, PIN_NO) == 1) {
                    /* PIN register bit = 1 means the pin is currently HIGH */
                    *PTR_PIN_VOLT_LEVEL = PIN_HIGH_LEVEL;
                } else {
                    /* PIN register bit = 0 means the pin is currently LOW */
                    *PTR_PIN_VOLT_LEVEL = PIN_LOW_LEVEL;
                }
            break;

            case PORT_NUMBER_D:
                if (GET_BIT(PIND, PIN_NO) == 1) {
                    /* PIN register bit = 1 means the pin is currently HIGH */
                    *PTR_PIN_VOLT_LEVEL = PIN_HIGH_LEVEL;
                } else {
                    /* PIN register bit = 0 means the pin is currently LOW */
                    *PTR_PIN_VOLT_LEVEL = PIN_LOW_LEVEL;
                }
            break;
        }
    }