#include <avr/curves.h>
#include <avr/field.h>

#undef DEBUG 

#ifdef DEBUG 
#include <stdio.h>
#endif

void vli_clear(uint8_t *p_vli)
{
    uint8_t i;
    for(i=0; i<ECC_BYTES; ++i)
    {
        p_vli[i] = 0;
    }
}

/* Returns 1 if p_vli == 0, 0 otherwise. */
uint8_t vli_isZero(uint8_t *p_vli)
{
    uint8_t i;
    for(i = 0; i < ECC_BYTES; ++i)
    {
        if(p_vli[i])
        {
            return 0;
        }
    }
    return 1;
}

/* Returns nonzero if bit p_bit of p_vli is set. */
uint8_t vli_testBit(uint8_t *p_vli, uint8_t p_bit)
{
    return (p_vli[p_bit/8] & (1 << (p_bit % 8)));
}

/* Counts the number of 8-bit "digits" in p_vli. */
uint8_t vli_numDigits(uint8_t *p_vli)
{
    int8_t i;
    /* Search from the end until we find a non-zero digit.
       We do it in reverse because we expect that most digits will be nonzero. */
    for(i = ECC_BYTES - 1; i >= 0 && p_vli[i] == 0; --i)
    {
    }

    return (i + 1);
}

/* Counts the number of bits required for p_vli. */
uint16_t vli_numBits(uint8_t *p_vli)
{
    uint8_t i;
    uint8_t l_digit;
    
    uint8_t l_numDigits = vli_numDigits(p_vli);
    if(l_numDigits == 0)
    {
        return 0;
    }

    l_digit = p_vli[l_numDigits - 1];
    for(i=0; l_digit; ++i)
    {
        l_digit >>= 1;
    }

    return (((uint16_t)l_numDigits - 1) * 8 + i);
}

/* Sets p_dest = p_src. */
void vli_set(uint8_t *p_dest, uint8_t *p_src)
{
    uint8_t i;
    for(i=0; i<ECC_BYTES; ++i)
    {
        p_dest[i] = p_src[i];
    }
}

/* Returns sign of p_left - p_right. */
int8_t vli_cmp(uint8_t *p_left, uint8_t *p_right)
{
    int8_t i;
    for(i = ECC_BYTES-1; i >= 0; --i)
    {
        if(p_left[i] > p_right[i])
        {
            return 1;
        }
        else if(p_left[i] < p_right[i])
        {
            return -1;
        }
    }
    return 0;
}

/* Computes p_result = p_in << c, returning carry. Can modify in place (if p_result == p_in). 0 < p_shift < 8. */
uint8_t vli_lshift(uint8_t *p_result, uint8_t *p_in, uint8_t p_shift)
{
    uint8_t l_carry = 0;
    uint8_t i;
    for(i = 0; i < ECC_BYTES; ++i)
    {
        uint8_t l_temp = p_in[i];
        p_result[i] = (l_temp << p_shift) | l_carry;
        l_carry = l_temp >> (8 - p_shift);
    }
    
    return l_carry;
}

/* Computes p_vli = p_vli >> 1. */
void vli_rshift1(uint8_t *p_vli)
{
    uint8_t *l_end = p_vli;
    uint8_t l_carry = 0;
    
    p_vli += ECC_BYTES;
    while(p_vli-- > l_end)
    {
        uint8_t l_temp = *p_vli;
        *p_vli = (l_temp >> 1) | l_carry;
        l_carry = l_temp << 7;
    }
}

void vli_xor(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right)
{

    uint8_t i;
    for(i=0; i<ECC_BYTES; ++i) 
    {
        p_result[i] = p_left[i] ^ p_right[i];
    }

}

