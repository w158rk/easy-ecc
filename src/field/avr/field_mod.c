#include <avr/curves.h>
#include <avr/field.h>

#include<string.h>

#ifdef DEBUG 
#include<stdio.h>
#endif

#define HIGH_HALF_MASK  0xffffffff00000000ull
#define LOW_HALF_MASK   0x00000000ffffffffull

#if ECC_CURVE == secp128r1

/* Computes p_result = p_product % curve_p.
   See algorithm 5 and 6 from http://www.isys.uni-klu.ac.at/PDF/2001-0126-MT.pdf */
void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product)
{
    uint8_t l_tmp[ECC_BYTES];
    uint8_t l_carry;
    
    vli_set(p_result, p_product);
    
    l_tmp[0] = p_product[2];
    l_tmp[1] = (p_product[3] & 0x1FFFFFFFFull) | (p_product[2] << 33);
    l_carry = vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = (p_product[2] >> 31) | (p_product[3] << 33);
    l_tmp[1] = (p_product[3] >> 31) | ((p_product[2] & 0xFFFFFFFF80000000ull) << 2);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = (p_product[2] >> 62) | (p_product[3] << 2);
    l_tmp[1] = (p_product[3] >> 62) | ((p_product[2] & 0xC000000000000000ull) >> 29) | (p_product[3] << 35);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = (p_product[3] >> 29);
    l_tmp[1] = ((p_product[3] & 0xFFFFFFFFE0000000ull) << 4);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = (p_product[3] >> 60);
    l_tmp[1] = (p_product[3] & 0xFFFFFFFE00000000ull);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = 0;
    l_tmp[1] = ((p_product[3] & 0xF000000000000000ull) >> 27);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    while(l_carry || vli_cmp(curve_p, p_result) != 1)
    {
        l_carry -= vli_sub(p_result, p_result, curve_p);
    }
}

#elif ECC_CURVE == secp192r1

/* Computes p_result = p_product % curve_p.
   See algorithm 5 and 6 from http://www.isys.uni-klu.ac.at/PDF/2001-0126-MT.pdf */
void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product)
{
    uint8_t l_tmp[ECC_BYTES];
    uint8_t l_carry;
    
    vli_set(p_result, p_product);
    
    vli_set(l_tmp, &p_product[3]);
    l_carry = vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = 0;
    l_tmp[1] = p_product[3];
    l_tmp[2] = p_product[4];
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = l_tmp[1] = p_product[5];
    l_tmp[2] = 0;
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    while(l_carry || vli_cmp(curve_p, p_result) != 1)
    {
        l_carry -= vli_sub(p_result, p_result, curve_p);
    }
}

#elif ECC_CURVE == secp256r1

void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product)
{
    uint8_t l_tmp[ECC_BYTES];
    int8_t l_carry;
    
    /* t */
    vli_set(p_result, p_product);
    
    /* s1 */
    memset(l_tmp, 0, 3*4);
    memcpy(l_tmp + 3*4, p_product + 11*4, 5*4);
    l_carry = vli_lshift(l_tmp, l_tmp, 1);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* s2 */
    memcpy(l_tmp + 3*4, p_product + 12*4, 4*4);    
    memset(l_tmp + 7*4, 0, 4);
    l_carry += vli_lshift(l_tmp, l_tmp, 1);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* s3 */
    memcpy(l_tmp, p_product + 8*4, 3*4);
    memset(l_tmp + 3*4, 0, 3*4);
    memcpy(l_tmp + 6*4, p_product + 14*4, 2*4);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* s4 */
    memcpy(l_tmp, p_product + 9*4, 3*4);
    memcpy(l_tmp + 3*4, p_product + 13*4, 3*4);
    memcpy(l_tmp + 6*4, p_product + 13*4, 4);
    memcpy(l_tmp + 7*4, p_product + 8*4, 4);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* d1 */
    memcpy(l_tmp, p_product + 11*4, 3*4);
    memset(l_tmp + 3*4, 0, 3*4);
    memcpy(l_tmp + 6*4, p_product + 8*4, 4);
    memcpy(l_tmp + 7*4, p_product + 10*4, 4);
    l_carry -= vli_sub(p_result, p_result, l_tmp);
    
    /* d2 */
    memcpy(l_tmp, p_product + 12*4, 4*4);
    memset(l_tmp + 4*4, 0, 2*4);
    memcpy(l_tmp + 6*4, p_product + 9*4, 4);
    memcpy(l_tmp + 7*4, p_product + 11*4, 4);
    l_carry -= vli_sub(p_result, p_result, l_tmp);
    
    /* d3 */
    memcpy(l_tmp, p_product + 13*4, 3*4);
    memcpy(l_tmp + 3*4, p_product + 8*4, 3*4);
    memset(l_tmp + 6*4, 0, 4);
    memcpy(l_tmp + 7*4, p_product + 12*4, 4);
    l_carry -= vli_sub(p_result, p_result, l_tmp);
    
    /* d4 */
    memcpy(l_tmp, p_product + 14*4, 2*4);
    memset(l_tmp + 2*4, 0, 4);
    memcpy(l_tmp + 3*4, p_product + 9*4, 3*4);
    memset(l_tmp + 6*4, 0, 4);
    memcpy(l_tmp + 7*4, p_product + 13*4, 4);
    l_carry -= vli_sub(p_result, p_result, l_tmp);
    
    if(l_carry < 0)
    {
        do
        {
            l_carry += vli_add(p_result, p_result, curve_p);
        } while(l_carry < 0);
    }
    else
    {
        while(l_carry || vli_cmp(curve_p, p_result) != 1)
        {
            l_carry -= vli_sub(p_result, p_result, curve_p);
        }
    }
}

#elif ECC_CURVE == secp384r1

void omega_mult(uint8_t *p_result, uint8_t *p_right)
{
    uint8_t l_tmp[ECC_BYTES];
    uint8_t l_carry, l_diff;
    
    /* Multiply by (2^128 + 2^96 - 2^32 + 1). */
    vli_set(p_result, p_right); /* 1 */
    l_carry = vli_lshift(l_tmp, p_right, 32);
    p_result[1 + ECC_BYTES] = l_carry + vli_add(p_result + 1, p_result + 1, l_tmp); /* 2^96 + 1 */
    p_result[2 + ECC_BYTES] = vli_add(p_result + 2, p_result + 2, p_right); /* 2^128 + 2^96 + 1 */
    l_carry += vli_sub(p_result, p_result, l_tmp); /* 2^128 + 2^96 - 2^32 + 1 */
    l_diff = p_result[ECC_BYTES] - l_carry;
    if(l_diff > p_result[ECC_BYTES])
    { /* Propagate borrow if necessary. */
        uint i;
        for(i = 1 + ECC_BYTES; ; ++i)
        {
            --p_result[i];
            if(p_result[i] != (uint8_t)-1)
            {
                break;
            }
        }
    }
    p_result[ECC_BYTES] = l_diff;
}

/* Computes p_result = p_product % curve_p
    see PDF "Comparing Elliptic Curve Cryptography and RSA on 8-bit CPUs"
    section "Curve-Specific Optimizations" */
void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product)
{
    uint8_t l_tmp[2*ECC_BYTES];
     
    while(!vli_isZero(p_product + ECC_BYTES)) /* While c1 != 0 */
    {
        uint8_t l_carry = 0;
        uint i;
        
        vli_clear(l_tmp);
        vli_clear(l_tmp + ECC_BYTES);
        omega_mult(l_tmp, p_product + ECC_BYTES); /* tmp = w * c1 */
        vli_clear(p_product + ECC_BYTES); /* p = c0 */
        
        /* (c1, c0) = c0 + w * c1 */
        for(i=0; i<ECC_BYTES+3; ++i)
        {
            uint8_t l_sum = p_product[i] + l_tmp[i] + l_carry;
            if(l_sum != p_product[i])
            {
                l_carry = (l_sum < p_product[i]);
            }
            p_product[i] = l_sum;
        }
    }
    
    while(vli_cmp(p_product, curve_p) > 0)
    {
        vli_sub(p_product, p_product, curve_p);
    }
    vli_set(p_result, p_product);
}

#endif

