/* JUST COPY the interface to implement the functionality (implement the function ) */

/* Include what you want from interfaces like BIT_MATH, STD_TYPES, and so on */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MDIO_interface.h"
#include "HSSD_interface.h"

/* We don't like magic numbers, each number has a meaning */
/* when we were at the display segment I told you that a: bit 0 and so on */
/* Define them as macros (macros is the best way for this, I know there are other approaches) */
#define A_LED_BIT   (0)
#define B_LED_BIT   (1)
#define C_LED_BIT   (2)
#define D_LED_BIT   (3)
#define E_LED_BIT   (4)
#define F_LED_BIT   (5)
#define G_LED_BIT   (6)
/* The dot led inside the segment is the 7'th bit, our driver don't use it 
 * if you want to use it, you will extend the driver for it 
 * (adding this member inside the struct, here in the macros, inside the functions as well) 
 * we don't need it now: a Design Decision */

#define LEDS_OFF_VALUE (0x00)


void HSSD_voidInit(st_SSD_Object_t* ptr_SSDObject) {
    /* Define the functionlity */
    /* Configure each SSD Pin to be output, just that */
    if(ptr_SSDObject != NULL) {
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_aLed.m_PortNumber, 
            ptr_SSDObject->m_aLed.m_PinNumber, PIN_MODE_OUTPUT);
    
        /*  Be CAUTIOUS when you copy PLEASE */
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_bLed.m_PortNumber, 
            ptr_SSDObject->m_bLed.m_PinNumber, PIN_MODE_OUTPUT);
    
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_cLed.m_PortNumber, 
            ptr_SSDObject->m_cLed.m_PinNumber, PIN_MODE_OUTPUT);
    
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_dLed.m_PortNumber, 
            ptr_SSDObject->m_dLed.m_PinNumber, PIN_MODE_OUTPUT);
    
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_eLed.m_PortNumber, 
            ptr_SSDObject->m_eLed.m_PinNumber, PIN_MODE_OUTPUT);
    
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_fLed.m_PortNumber, 
            ptr_SSDObject->m_fLed.m_PinNumber, PIN_MODE_OUTPUT);
    
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_gLed.m_PortNumber, 
            ptr_SSDObject->m_gLed.m_PinNumber, PIN_MODE_OUTPUT);
    
        /* ***Start Clear Methodolgy*** */
        HSSD_voidTurnOff(ptr_SSDObject);
        HSSD_voidDisableOneSSD(ptr_SSDObject);

        /* Forgot, Fixed */
        MDIO_voidSet_Pin_DIO_Mode(ptr_SSDObject->m_EnablePin.m_PortNumber, 
            ptr_SSDObject->m_EnablePin.m_PinNumber, PIN_MODE_OUTPUT);
    }
}

void HSSD_voidDisplaySegments(st_SSD_Object_t* ptr_SSDObject, uint8_t copy_uint8_segments_byte) {
    /* Many ways to implement this functionlity, if it does it right, you win!! */
    /* one naive way is to convert each bit for the other configuration */
    /* using ~ one's compliement */
    /* assuming the byte that is comming is common cathode (turning the led on by 1) */
    /* check if the configuration is anode, convert the byte from 1's to 0's */
    
    if(ptr_SSDObject != NULL) {
        if(ptr_SSDObject->m_CommonType == SSD_COMMON_CATHODE) {
        /* Leave the copy as it */
            copy_uint8_segments_byte;
        }
        else { // common anode, convert the bits
            copy_uint8_segments_byte = ~copy_uint8_segments_byte;
        }
    
    
        /* Before The right Logic you want the simple naive approach to write on the segments */
        /* we get each corresponding bit from the segments bytes, 
         * a: bit 0
         * b: bit 1 and so on */
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_aLed.m_PortNumber, 
            ptr_SSDObject->m_aLed.m_PinNumber, GET_BIT(copy_uint8_segments_byte, A_LED_BIT));
        
        /* Be CAUTIOUS when you copy, all of the bugs appears when copying */
        /* change all the arguements not only one */
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_bLed.m_PortNumber, 
            ptr_SSDObject->m_bLed.m_PinNumber, GET_BIT(copy_uint8_segments_byte, B_LED_BIT));
    
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_cLed.m_PortNumber, 
            ptr_SSDObject->m_cLed.m_PinNumber, GET_BIT(copy_uint8_segments_byte, C_LED_BIT));
    
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_dLed.m_PortNumber, 
            ptr_SSDObject->m_dLed.m_PinNumber, GET_BIT(copy_uint8_segments_byte, D_LED_BIT));
    
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_eLed.m_PortNumber, 
            ptr_SSDObject->m_eLed.m_PinNumber, GET_BIT(copy_uint8_segments_byte, E_LED_BIT));
    
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_fLed.m_PortNumber, 
            ptr_SSDObject->m_fLed.m_PinNumber, GET_BIT(copy_uint8_segments_byte, F_LED_BIT));
    
        MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_gLed.m_PortNumber, 
            ptr_SSDObject->m_gLed.m_PinNumber, GET_BIT(copy_uint8_segments_byte, G_LED_BIT));
    }
}

void HSSD_voidTurnOff(st_SSD_Object_t* ptr_SSDObject) {
    /* Define the functionlity */
    /* هطفي كل الليدات */
    if(ptr_SSDObject != NULL) {
        HSSD_voidDisplaySegments(ptr_SSDObject, LEDS_OFF_VALUE);
    }
}

void HSSD_voidEnableOneSSD(st_SSD_Object_t* ptr_SSDObject) {
    /* Define the functionlity */
    /* this is for the common pin, if cathode (-) enable means set it to zero */
    /* if anode (+) enable means set it to 1 */
    if(ptr_SSDObject != NULL) {
        if(ptr_SSDObject->m_CommonType == SSD_COMMON_CATHODE) {
            MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_EnablePin.m_PortNumber,
                ptr_SSDObject->m_EnablePin.m_PinNumber, PIN_LOW_LEVEL);
        }
        else { // SSD_COMMON_ANODE
            MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_EnablePin.m_PortNumber,
                ptr_SSDObject->m_EnablePin.m_PinNumber, PIN_HIGH_LEVEL);
        }
    }
}

/* هنعكس في الديسابل */
/* خلي بالك دايما وانت بتكبير معظم المشاكل بتيجي من الكبيرة */
/* انك تعمل كوبي كدا كأنك بتاخد خرزة علي سرعة 100 بجد يتوفر عليك يانت عارف ايه الي ممكن يحصل ربنا يسترنا */
void HSSD_voidDisableOneSSD(st_SSD_Object_t* ptr_SSDObject) {
    /* Define the functionlity */
    /* this is for the common pin, if cathode (-) disable means set it to 1 */
    /* if anode (+) disable means set it to 0 */
    if(ptr_SSDObject != NULL) {
        if(ptr_SSDObject->m_CommonType == SSD_COMMON_CATHODE) {
            MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_EnablePin.m_PortNumber,
                ptr_SSDObject->m_EnablePin.m_PinNumber, PIN_HIGH_LEVEL);
        }
        else { // SSD_COMMON_ANODE
            MDIO_voidSet_Pin_Output_Volt_Level(ptr_SSDObject->m_EnablePin.m_PortNumber,
                ptr_SSDObject->m_EnablePin.m_PinNumber, PIN_LOW_LEVEL);
        }
    }
}



void HSSD_voidDisplayNumber(st_SSD_Object_t* ptr_SSDObject, uint8_t copy_uint8_literal_number) {
    /* Define the functionality */
    /* 1. having a map between the literal number (e.g. 0, 1, 2), and the relative segments */
    /* There are many ways to have the map, maybe switch case, maybe a look up array choose the suitable one */
    uint8_t local_uint8_mapped_segments = 0; 

    switch(copy_uint8_literal_number) {
        /* Turning on the relative segments 0b0gfedcba  */
        case 0: local_uint8_mapped_segments = 0b00111111; break; 
        case 1: local_uint8_mapped_segments = 0b00000110; break; 
        case 2: local_uint8_mapped_segments = 0b01011011; break; 
        case 3: local_uint8_mapped_segments = 0b01001111; break; 
        case 4: local_uint8_mapped_segments = 0b01100110; break; 
        case 5: local_uint8_mapped_segments = 0b01101101; break; 
        case 6: local_uint8_mapped_segments = 0b01111101; break; 
        case 7: local_uint8_mapped_segments = 0b00000111; break; 
        case 8: local_uint8_mapped_segments = 0b01111111; break; 
        case 9: local_uint8_mapped_segments = 0b01101111; break; 
        default: local_uint8_mapped_segments = 0x00; break; // close all leds brea
    }
    
    /* 2. Save the Relative Mapped Segments and Then Send it to Display Segments */
    HSSD_voidDisplaySegments(ptr_SSDObject, local_uint8_mapped_segments);
}