#include <avr/curves.h>
#include <avr/field.h>

#undef DEBUG

#ifdef DEBUG 
#include <stdio.h>
#endif

/* Computes p_result = p_left * p_right. */
void vli_mult(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right)
{
    uint16_t r01 = 0;
    uint8_t r2 = 0;
    
    uint8_t i, k;
    
    /* Compute each digit of p_result in sequence, maintaining the carries. */
    for(k=0; k < ECC_BYTES*2 - 1; ++k)
    {
        uint8_t l_min = (k < ECC_BYTES ? 0 : (k + 1) - ECC_BYTES);
        for(i=l_min; i<=k && i<ECC_BYTES; ++i)
        {
            uint16_t l_product = (uint16_t)p_left[i] * (uint16_t)p_right[k-i];
            r01 += l_product;
            r2 += (r01 < l_product);
        }
        p_result[k] = (uint8_t)r01;
        #ifdef DEBUG
        if (k==0 ) {
            printf("pleft[0] : %02x, pright[0] : %02x , test : %02x\n", (uint8_t)p_left[0], (uint8_t)p_right[0], (uint8_t)r01);            
        }
        #endif
        r01 = (r01 >> 8) | (((uint16_t)r2) << 8);
        r2 = 0;
    }
    
    p_result[ECC_BYTES*2 - 1] = (uint8_t)r01;

}

/* Computes p_result = (p_left * p_right) % curve_p. */
void vli_modMult_fast(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right)
{
    uint8_t l_product[2 * ECC_BYTES];
    vli_mult(l_product, p_left, p_right);

    vli_mmod_fast(p_result, l_product);
}

/* Computes p_result = (p_left * p_right) % p_mod. */
void vli_modMult(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right, uint8_t *p_mod)
{
    uint8_t l_product[2 * ECC_BYTES];
    uint8_t l_modMultiple[2 * ECC_BYTES];
    uint8_t l_digitShift, l_bitShift;
    uint16_t l_productBits;
    uint16_t l_modBits = vli_numBits(p_mod);
    #ifdef DEBUG 

    ERROR("multiplication");
    NUM_PRINT(p_left);
    NUM_PRINT(p_right);
    NUM_PRINT(p_mod);
    #endif

    vli_mult(l_product, p_left, p_right);
    #ifdef DEBUG 
    ERROR("l_product");
    NUM_PRINT((l_product+ECC_BYTES));
    NUM_PRINT(l_product);
    #endif
    
    l_productBits = vli_numBits(l_product + ECC_BYTES);
    if(l_productBits)
    {
        l_productBits += ECC_BYTES * 8;
    }
    else
    {
        l_productBits = vli_numBits(l_product);
    }

    if(l_productBits < l_modBits)
    { /* l_product < p_mod. */
        vli_set(p_result, l_product);
        return;
    }
    
    /* Shift p_mod by (l_leftBits - l_modBits). This multiplies p_mod by the largest
       power of two possible while still resulting in a number less than p_left. */
    vli_clear(l_modMultiple);
    vli_clear(l_modMultiple + ECC_BYTES);
    l_digitShift = (l_productBits - l_modBits) / 8;
    l_bitShift = (l_productBits - l_modBits) % 8;
    if(l_bitShift)
    {
        l_modMultiple[l_digitShift + ECC_BYTES] = vli_lshift(l_modMultiple + l_digitShift, p_mod, l_bitShift);
    }
    else
    {
        vli_set(l_modMultiple + l_digitShift, p_mod);
    }


    /* Subtract all multiples of p_mod to get the remainder. */
    vli_clear(p_result);
    p_result[0] = 1; /* Use p_result as a temp var to store 1 (for subtraction) */
    while(l_productBits > ECC_BYTES * 8 || vli_cmp(l_modMultiple, p_mod) >= 0)
    {
        int8_t l_cmp = vli_cmp(l_modMultiple + ECC_BYTES, l_product + ECC_BYTES);
        if(l_cmp < 0 || (l_cmp == 0 && vli_cmp(l_modMultiple, l_product) <= 0))
        {
            if(vli_sub(l_product, l_product, l_modMultiple))
            { /* borrow */
                vli_sub(l_product + ECC_BYTES, l_product + ECC_BYTES, p_result);
            }
            vli_sub(l_product + ECC_BYTES, l_product + ECC_BYTES, l_modMultiple + ECC_BYTES);
        }
        uint8_t l_carry = (l_modMultiple[ECC_BYTES] & 0x01) << 7;
        vli_rshift1(l_modMultiple + ECC_BYTES);
        vli_rshift1(l_modMultiple);
        l_modMultiple[ECC_BYTES-1] |= l_carry;
        
        --l_productBits;
    }
    vli_set(p_result, l_product);

    #ifdef DEBUG 

    ERROR("multiplication");
    NUM_PRINT(p_left);
    NUM_PRINT(p_right);
    NUM_PRINT(p_mod);
    ERROR("result");
    NUM_PRINT(p_result);
    
    #endif

}



