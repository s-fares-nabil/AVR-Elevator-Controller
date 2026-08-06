#ifndef HDC_MOTOR_INTERFACE_H
#define HDC_MOTOR_INTERFACE_H

#include "MDIO_interface.h"

/* Define the Data */
typedef struct {
    enum_PortNumber_t m_PortNumber;
    enum_PinNumber_t  m_PinNumber;
} st_DCM_Pin_t;

/* State Types as enumeration */
typedef enum {
    DCM_STOP = 0,
    DCM_FORWARD = 1,
    DCM_BACKWARD = 2,
} enum_DCMotorState_t;

typedef struct {
    /* Define the members */
    st_DCM_Pin_t m_ForwardPin;
    st_DCM_Pin_t m_BackwardPin;

    /* State at run time */
    enum_DCMotorState_t m_State;
} st_DCMotor_Ojbect_t;

/* @brief Configure and Init the DC Motor inside the system
 *
 * Sets both direction pins as output, and starts stopped (both pins
 * LOW) so the motor never powers up in an undefined direction
 *
 * @param DC Motor object passed by address to save memory
 *
 * @return void
 **/
void HDCM_voidInit(st_DCMotor_Ojbect_t* ptr_DCMotorObject);

/* @brief Drive the motor Forward
 *
 * Always disables both pins first, then enables Forward. This ordering
 * guarantees Forward Pin and Backward Pin are never HIGH at the same time (short circuit).
 *
 * @param DC Motor object passed by address
 *
 * @return void
 **/
void HDCM_voidMoveForward(st_DCMotor_Ojbect_t* ptr_DCMotorObject);


/* @brief Drive the motor Backward
 *
 * Always disables both pins first, then enables Backward. This ordering
 * guarantees Forward Pin and Backward Pin are never HIGH at the same time (short circuit).
 *
 * @param DC Motor object passed by address
 *
 * @return void
 **/
void HDCM_voidMoveBackward(st_DCMotor_Ojbect_t* ptr_DCMotorObject);

/* @brief Stop the motor
 *
 * Drives both pins to LOW
 *
 * @param DC Motor object passed by address
 *
 * @return void
 **/
void HDCM_voidStop(st_DCMotor_Ojbect_t* ptr_DCMotorObject);

#endif