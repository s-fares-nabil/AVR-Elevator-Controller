#ifndef HSWITCH_INTERFACE_H
#define HSWITCH_INTERFACE_H

#include "MDIO_interface.h"

typedef enum {
    SWITCH_PRESSED = 0,
    SWITCH_RELEASED = 1,
} enum_SwitchState_t;

typedef struct {
    enum_PortNumber_t m_PortNumber;
    enum_PinNumber_t m_PinNumber;
    enum_SwitchState_t m_SwitchState;
} st_SwitchObject_t;

/* @brief Configure and Init the Switch inside the system
 *
 * This function init the Switch Object 
 * 
 * @param Switch object passed by address to save memory 
 *
 * @return void
 **/
void HSWTICH_voidInit(st_SwitchObject_t* ptr_SwitchObject);

/* @brief Read the status of the switch object
 *
 * This gets the status of the switch is pressed or not
 * 
 * @param switch object passed by address to save memory 
 *
 * @return void
 **/
void HSWTICH_voidReadState(st_SwitchObject_t* ptr_SwitchObject);

#endif