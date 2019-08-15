#include <avr/curves.h>
#include <avr/field.h>

#ifdef DEBUG 
#include <stdio.h>
#endif

/* Computes p_result = p_left^2. */
void vli_square(uint8_t *p_result, uint8_t *p_left)
{

    uint16_t r01 = 0;
    uint8_t r2 = 0;

    uint8_t i, k;

    for(k=0; k < ECC_BYTES*2 - 1; ++k)
    {
        uint8_t l_min = (k < ECC_BYTES ? 0 : (k + 1) - ECC_BYTES);
        for(i=l_min; i<=k && i<=k-i; ++i)
        {
            uint16_t l_product = (uint16_t)p_left[i] * p_left[k-i];
            if(i < k-i)
            {
                r2 += l_product >> 15;
                l_product *= 2;
            }
            r01 += l_product;
            r2 += (r01 < l_product);
        }
        p_result[k] = (uint8_t)r01;
        r01 = (r01 >> 8) | (((uint16_t)r2) << 8);
        r2 = 0;
    }
    
    p_result[ECC_BYTES*2 - 1] = (uint8_t)r01;
}


/* Computes p_result = p_left^2 % curve_p. */
void vli_modSquare_fast(uint8_t *p_result, uint8_t *p_left)
{
    uint8_t l_product[2 * ECC_BYTES];
    vli_square(l_product, p_left);

    vli_mmod_fast(p_result, l_product);

}
