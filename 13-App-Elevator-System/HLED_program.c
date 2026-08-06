#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "HLED_interface.h"

/* @brief Configure and Init the Led inside the system
 *
 * This function configure the Led Object 
 * 
 * @param Led object passed by address to save memory 
 *
 * @return void
 **/
void HLED_voidInit(st_LedObject_t* ptr_LedObject) {
    if(ptr_LedObject != NULL) {
        /* Call MDIO interface to set the pin of the led to be output */
        MDIO_voidSet_Pin_DIO_Mode(ptr_LedObject->m_PortNumber, 
            ptr_LedObject->m_PinNumber, PIN_MODE_OUTPUT);
    }
}

/* @brief Turn On the Led Object
 *
 * This function Turn on the Led Object 
 * 
 * @param Led object passed by address to save memory 
 *
 * @return void
 **/
void HLED_voidTurnOn(st_LedObject_t* ptr_LedObject) {
    if(ptr_LedObject != NULL) {
        /* Call MDIO interface to set high level on the pin */
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_LedObject->m_PortNumber, 
            ptr_LedObject->m_PinNumber, PIN_HIGH_LEVEL);

        /* Update the state of the Led */
        ptr_LedObject->m_State = LED_ON;
    }
}

/* @brief Turn off the Led Object
 *
 * This function Turns on the Led Object 
 * 
 * @param Led object passed by address to save memory 
 *
 * @return void
 **/
void HLED_voidTurnOff(st_LedObject_t* ptr_LedObject) {
    if(ptr_LedObject != NULL) {
        /* Call MDIO interface to set low level on the pin */
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_LedObject->m_PortNumber, 
            ptr_LedObject->m_PinNumber, PIN_LOW_LEVEL);

        /* Update the state of the Led */
        ptr_LedObject->m_State = LED_OFF;
    }
}

