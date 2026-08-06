#ifndef HSSD_INTERFACE_H
#define HSSD_INTERFACE_H


/* Contains enum_PortNumber_t and other data types so it should be included */
#include "MDIO_interface.h"

/* Grouping each pin by its classfication from PORT and PIN Numbers */
/* AGAIN: this is a decision about data, it will ease the operations */
/* Design Is ART, ART has no right or wrong, it's all about your instinct */
typedef struct {
    enum_PortNumber_t m_PortNumber;
    enum_PinNumber_t  m_PinNumber;
} st_SSD_Pin_t;

/* AGAIN: a decision of data to help us, this is ART not mandatory */
typedef enum {
    SSD_COMMON_CATHODE = 0,
    SSD_COMMON_ANODE   = 1,
} enum_SSDCommonAnodeCathode_t;

typedef struct {
    /* 7 Segments Display has 7 Leds from A to G */
    st_SSD_Pin_t m_aLed; 
    st_SSD_Pin_t m_bLed;
    st_SSD_Pin_t m_cLed;
    st_SSD_Pin_t m_dLed;
    st_SSD_Pin_t m_eLed;
    st_SSD_Pin_t m_fLed;
    st_SSD_Pin_t m_gLed;

    /* THERE ARE MORE MEMBERS THAT DEFINE SSD OBJECT */
    enum_SSDCommonAnodeCathode_t m_CommonType;

    /* Enable Pin to operate the concept of POV (having multiable SSD in the same time) */
    st_SSD_Pin_t m_EnablePin;

    /* These members are sufficent for now, maybe later you might need one to be extended */
} st_SSD_Object_t;


/* @brief Configure and Init the SSD inside the system
 *
 * Sets all 7 segment pins and the enable pin as output, then turns
 * the SSD fully off and disabled (respecting the object's common type)
 *
 * @param SSD object passed by address to save memory
 *
 * @return void
 **/
void HSSD_voidInit(st_SSD_Object_t* ptr_SSDObject);

/* @brief Write a full set of segments to the SSD in one call
 *
 * This takes the SSD Objects, and the Semgents byte (a: first bit, b: second bit and so on)
 * depending on the common type (cathode or anode) it will turn on or off the segements  
 *  
 * in a nutshell this displays the data you want on the SSD object 
 *  
 * @param SSD object passed by address, copy_uint8_segments_byte
 *
 * @return void
 **/
void HSSD_voidDisplaySegments(st_SSD_Object_t* ptr_SSDObject, uint8_t copy_uint8_segments_byte);


/* @brief Turn off all 7 segments of the SSD
 *
 * @param SSD object passed by address
 *
 * @return void
 **/
void HSSD_voidTurnOff(st_SSD_Object_t* ptr_SSDObject);

/* @brief Activate this SSD's control pin
 *
 * Used during POV concept to select which physical display is
 * currently receiving the shared segment lines
 *
 * @param SSD object passed by address
 *
 * @return void
 **/
void HSSD_voidEnableOneSSD(st_SSD_Object_t* ptr_SSDObject);

/* @brief Deactivate this SSD's control pin
 *
 * @param SSD object passed by address
 *
 * @return void
 **/
void HSSD_voidDisableOneSSD(st_SSD_Object_t* ptr_SSDObject); 

/* @brief Deactivate this SSD's control pin
 *  
 * takes the ssd object and a literal number to be displayed 
 * 
 * @param SSD object passed by address, copy_uint8_literal_number (e.g. 0, 1, 2)
 *
 * @return void
 **/
void HSSD_voidDisplayNumber(st_SSD_Object_t* ptr_SSDObject, uint8_t copy_uint8_literal_number);

#endif
