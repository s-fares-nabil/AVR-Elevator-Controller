#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "MDIO_interface.h"
#include "HDC_Motor_interface.h"

void HDCM_voidInit(st_DCMotor_Ojbect_t* ptr_DCMotorObject) {
    /* Define Functionality */
    /* 1. Set the modes of the pins to be output */
    if(ptr_DCMotorObject != NULL) {
        MDIO_voidSet_Pin_DIO_Mode(ptr_DCMotorObject->m_ForwardPin.m_PortNumber,
            ptr_DCMotorObject->m_ForwardPin.m_PinNumber, PIN_MODE_OUTPUT);
 
        MDIO_voidSet_Pin_DIO_Mode(ptr_DCMotorObject->m_BackwardPin.m_PortNumber,
            ptr_DCMotorObject->m_BackwardPin.m_PinNumber, PIN_MODE_OUTPUT);
 
        /* 2. Stop the motor at init so you start clean */
        HDCM_voidStop(ptr_DCMotorObject);
    }
}

void HDCM_voidMoveForward(st_DCMotor_Ojbect_t* ptr_DCMotorObject) {
    /* Define Functionality */
    if(ptr_DCMotorObject != NULL) {
        /* Stop at first to ensure the pins are low so you avoid short circuit */
        HDCM_voidStop(ptr_DCMotorObject);

        /* Then Drive the forward pin to be high */
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_DCMotorObject->m_ForwardPin.m_PortNumber,
            ptr_DCMotorObject->m_ForwardPin.m_PinNumber, PIN_HIGH_LEVEL);
        
        /* Update the state to be forward */
        ptr_DCMotorObject->m_State = DCM_FORWARD;
    }
}

/* Same as forward, however drive the backward pin */
void HDCM_voidMoveBackward(st_DCMotor_Ojbect_t* ptr_DCMotorObject) {
    /* Define Functionality */
    if(ptr_DCMotorObject != NULL) {
        /* Stop at first to ensure the pins are low so you avoid short circuit */
        HDCM_voidStop(ptr_DCMotorObject);

        /* Then Drive the forward pin to be high */
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_DCMotorObject->m_BackwardPin.m_PortNumber,
            ptr_DCMotorObject->m_BackwardPin.m_PinNumber, PIN_HIGH_LEVEL);
        
        /* Update the state to be forward */
        ptr_DCMotorObject->m_State = DCM_BACKWARD;
    }
}

void HDCM_voidStop(st_DCMotor_Ojbect_t* ptr_DCMotorObject) {
    /* Define Functionality */
    if(ptr_DCMotorObject != NULL) {
        /* Drive the pins to be low */
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_DCMotorObject->m_ForwardPin.m_PortNumber,
            ptr_DCMotorObject->m_ForwardPin.m_PinNumber, PIN_LOW_LEVEL);

        MDIO_voidSet_Pin_Output_Volt_Level(ptr_DCMotorObject->m_BackwardPin.m_PortNumber,
            ptr_DCMotorObject->m_BackwardPin.m_PinNumber, PIN_LOW_LEVEL);
        
        /* Update the state */
        ptr_DCMotorObject->m_State = DCM_STOP;
    }
}