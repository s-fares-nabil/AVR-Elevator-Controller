/* Our header guard for the interface file */
#ifndef HLED_INTERFACE_H
#define HLED_INTERFACE_H

#include "MDIO_interface.h"

typedef enum {
    LED_OFF = 0,
    LED_ON = 1,
} enum_LedObjectState_t;

/* Declaring the Led Object (Struct) */
typedef struct {
    /* Which Port */
    enum_PortNumber_t m_PortNumber;

    /* Which Pin */
    enum_PinNumber_t m_PinNumber;

    /* Run Time State */
    enum_LedObjectState_t m_State;
} st_LedObject_t;


/* @brief Configure and Init the Led inside the system
 *
 * This function configure the Led Object 
 * 
 * @param Led object passed by address to save memory 
 *
 * @return void
 **/
void HLED_voidInit(st_LedObject_t* ptr_LedObject);

/* @brief Turn On the Led Object
 *
 * This function Turn on the Led Object 
 * 
 * @param Led object passed by address to save memory 
 *
 * @return void
 **/
void HLED_voidTurnOn(st_LedObject_t* ptr_LedObject);

/* @brief Turn off the Led Object
 *
 * This function Turns on the Led Object 
 * 
 * @param Led object passed by address to save memory 
 *
 * @return void
 **/
void HLED_voidTurnOff(st_LedObject_t* ptr_LedObject);

#endif