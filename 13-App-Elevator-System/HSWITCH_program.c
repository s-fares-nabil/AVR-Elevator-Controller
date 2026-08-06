#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "MDIO_interface.h"
#include "HSWITCH_interface.h"

/* @brief Configure and Init the Switch inside the system
 *
 * This function init the Switch Object 
 * 
 * @param Switch object passed by address to save memory 
 *
 * @return void
 **/
void HSWTICH_voidInit(st_SwitchObject_t* ptr_SwitchObject) {
    if(ptr_SwitchObject != NULL) {
        /* Init it to be input pull up */
        MDIO_voidSet_Pin_DIO_Mode(ptr_SwitchObject->m_PortNumber, 
            ptr_SwitchObject->m_PinNumber, PIN_MODE_INPUT_PULL_UP);
    }
}

/* @brief Read the status of the switch object
 *
 * This gets the status of the switch is pressed or not
 * 
 * @param switch object passed by address to save memory 
 *
 * @return void
 **/
void HSWTICH_voidReadState(st_SwitchObject_t* ptr_SwitchObject) {
    enum_PinVoltLevel_t local_PinVoltLevelState = PIN_HIGH_LEVEL;
    if(ptr_SwitchObject != NULL) {
        /* Read Pin Status */
        MDIO_voidGet_Pin_State(ptr_SwitchObject->m_PortNumber, 
            ptr_SwitchObject->m_PinNumber, &local_PinVoltLevelState );

        if(local_PinVoltLevelState == PIN_LOW_LEVEL) {
            ptr_SwitchObject->m_SwitchState = SWITCH_PRESSED;
        }
        else {
            ptr_SwitchObject->m_SwitchState = SWITCH_RELEASED;
        }
    }
}