/* This is called header guard */
/* Ensures that it only got replaced one time per file */
#ifndef LIB_STD_TYPES_H
#define LIB_STD_TYPES_H

/* typedef keyword create a new name for the data type from existing ones */
typedef unsigned char       uint8_t;
typedef unsigned short int  uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long int uint64_t;

typedef signed char         sint8_t;
typedef signed short int    sint16_t;
typedef signed int          sint32_t;
typedef signed long long int sint64_t;

typedef float                float32_t;
typedef double                float64_t;
typedef long double        float128_t;

#define NULL ( 0 )

#endif // ends #ifndef LIB_STD_TYPES_H