#include <curves.h>
#include <field.h>

#ifdef DEBUG 
#include <stdio.h>
#endif

void vli_clear(uint32_t *p_vli)
{
    uint i;
    for(i=0; i<NUM_ECC_DIGITS; ++i)
    {
        p_vli[i] = 0;
    }
}

/* Returns 1 if p_vli == 0, 0 otherwise. */
int vli_isZero(uint32_t *p_vli)
{
    uint i;
    for(i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        if(p_vli[i])
        {
            return 0;
        }
    }
    return 1;
}

/* Returns nonzero if bit p_bit of p_vli is set. */
uint32_t vli_testBit(uint32_t *p_vli, uint p_bit)
{
    return (p_vli[p_bit/32] & ((uint32_t)1 << (p_bit % 32)));
}

/* Counts the number of 32-bit "digits" in p_vli. */
uint vli_numDigits(uint32_t *p_vli)
{
    int i;
    /* Search from the end until we find a non-zero digit.
       We do it in reverse because we expect that most digits will be nonzero. */
    for(i = NUM_ECC_DIGITS - 1; i >= 0 && p_vli[i] == 0; --i)
    {
    }

    return (i + 1);
}

/* Counts the number of bits required for p_vli. */
uint vli_numBits(uint32_t *p_vli)
{
    uint i;
    uint32_t l_digit;
    
    uint l_numDigits = vli_numDigits(p_vli);
    if(l_numDigits == 0)
    {
        return 0;
    }

    l_digit = p_vli[l_numDigits - 1];
    for(i=0; l_digit; ++i)
    {
        l_digit >>= 1;
    }
    
    return ((l_numDigits - 1) * 32 + i);
}

/* Sets p_dest = p_src. */
void vli_set(uint32_t *p_dest, uint32_t *p_src)
{
    uint i;
    for(i=0; i<NUM_ECC_DIGITS; ++i)
    {
        p_dest[i] = p_src[i];
    }
}

/* Returns sign of p_left - p_right. */
int vli_cmp(uint32_t *p_left, uint32_t *p_right)
{
    int i;
    for(i = NUM_ECC_DIGITS-1; i >= 0; --i)
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

/* Computes p_result = p_in << c, returning carry. Can modify in place (if p_result == p_in). 0 < p_shift < 32. */
uint32_t vli_lshift(uint32_t *p_result, uint32_t *p_in, uint p_shift)
{
    uint32_t l_carry = 0;
    uint i;
    for(i = 0; i < NUM_ECC_DIGITS; ++i)
    {
        uint32_t l_temp = p_in[i];
        p_result[i] = (l_temp << p_shift) | l_carry;
        l_carry = l_temp >> (32 - p_shift);
    }
    
    return l_carry;
}

/* Computes p_vli = p_vli >> 1. */
void vli_rshift1(uint32_t *p_vli)
{
    uint32_t *l_end = p_vli;
    uint32_t l_carry = 0;
    
    p_vli += NUM_ECC_DIGITS;
    while(p_vli-- > l_end)
    {
        uint32_t l_temp = *p_vli;
        *p_vli = (l_temp >> 1) | l_carry;
        l_carry = l_temp << 31;
    }
}

void vli_xor(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right)
{

    uint i;
    for(i=0; i<NUM_ECC_DIGITS; ++i) 
    {
        p_result[i] = p_left[i] ^ p_right[i];
    }

}


void ecc_bytes2native(uint32_t p_native[NUM_ECC_DIGITS], const char p_bytes[ECC_BYTES])
{
    unsigned i;
    for(i=0; i<NUM_ECC_DIGITS; ++i)
    {
        const uint8_t *p_digit = p_bytes + 4 * (NUM_ECC_DIGITS - 1 - i);
        p_native[i] = ((uint32_t)p_digit[0] << 24) | ((uint32_t)p_digit[1] << 16) | ((uint32_t)p_digit[2] << 8) | ((uint32_t)p_digit[3]);
    }
}

void ecc_native2bytes(char p_bytes[ECC_BYTES], const uint32_t p_native[NUM_ECC_DIGITS])
{
    unsigned i;
    for(i=0; i<NUM_ECC_DIGITS; ++i)
    {
        char *p_digit = p_bytes + 4 * (NUM_ECC_DIGITS - 1 - i);
        p_digit[0] = p_native[i] >> 24;
        p_digit[1] = p_native[i] >> 16;
        p_digit[2] = p_native[i] >> 8;
        p_digit[3] = p_native[i];
    }
}


