#ifndef BIT_MATH_H
#define BIT_MATH_H

#include "STD_TYPES.h"

#define SET_BIT(REG, BIT_NO) ( (REG) |= ( 1 << (BIT_NO) ) )

#define CLR_BIT(REG, BIT_NO) ( (REG) &= ~( 1 << (BIT_NO) ) )

#define TOG_BIT(REG, BIT_NO) ( (REG) ^= ( 1 << (BIT_NO) ) )

#define GET_BIT(REG, BIT_NO) ( ( (REG) >> (BIT_NO) ) & 1 )

/* Writes val into a group of bits inside reg starting at start_bit_no.
 * num_of_bits is just how many bits you want to write (3, 4, whatever),
 * reg is passed by the address to be updated 
 * If num_of_bits is 0 or less it's not a valid group */
static inline void WRT_GROUP_OF_BITS(uint8_t *reg, uint8_t start_bit_no, 
    uint8_t val, sint8_t num_of_bits)
{
	if (num_of_bits > 0 && num_of_bits <= 8)
	{
        /* To mask the value, you need to clear the bits at first (make it zeros ) */
		*reg &= ~( ( (1 << num_of_bits) - 1 ) << start_bit_no );  
        
        /* Then Or with the value you want */
        *reg |= (val << start_bit_no);
	}
}

#endif
