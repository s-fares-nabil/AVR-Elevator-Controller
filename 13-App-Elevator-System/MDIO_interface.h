/* Define the interfaces (the functionality that the user will use ) */

/* Header Guard as this is a header (.h) file */
#ifndef MDIO_INTERFACE_H
#define MDIO_INTERFACE_H

/* a data type for PORT Numbers */
typedef enum {
    PORT_NUMBER_A = 0,
    PORT_NUMBER_B = 1,
    PORT_NUMBER_C = 2,
    PORT_NUMBER_D = 3,
} enum_PortNumber_t;

/* Pin Number we have 8 pins inside each port */
typedef enum {
    PIN_NUMBER_0 = 0,
    PIN_NUMBER_1 = 1,
    PIN_NUMBER_2 = 2,
    PIN_NUMBER_3 = 3,
    PIN_NUMBER_4 = 4,
    PIN_NUMBER_5 = 5,
    PIN_NUMBER_6 = 6,
    PIN_NUMBER_7 = 7,
} enum_PinNumber_t;

/* Pin Mode */
typedef enum {
    PIN_MODE_INPUT_FLOATING = 0,
    PIN_MODE_INPUT_PULL_UP = 1,
    PIN_MODE_OUTPUT = 2,
} enum_PinMode_t;

/* Pin Volt Level */
typedef enum {
    PIN_LOW_LEVEL = 0,
    PIN_HIGH_LEVEL = 1,
} enum_PinVoltLevel_t;


/* @brief Configure the DIO Pin Mode
 *
 * This function configure the DIO Pin, we have three modes in DIO: 
 * output, input floating, input_pull_up
 *
 * @param PIN_PORT, PIN_NO, PIN_MODE
 *
 * @return void
 **/
void MDIO_voidSet_Pin_DIO_Mode(enum_PortNumber_t PIN_PORT,  enum_PinNumber_t PIN_NO, 
    enum_PinMode_t PIN_MODE);

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
    enum_PinVoltLevel_t PIN_VOLT_LEVEL);

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
    enum_PinVoltLevel_t * PTR_PIN_VOLT_LEVEL);


#endif // #ifndef MDIO_INTERFACE_H