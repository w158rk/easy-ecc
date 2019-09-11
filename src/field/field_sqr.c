#include <curves.h>
#include <field.h>
#undef DEBUG


/* Computes p_result = p_left^2. */
void vli_square(uint32_t *p_result, uint32_t *p_left)
{

    uint64_t r01 = 0;
    uint32_t r2 = 0;
    
    uint i, k;
    for(k=0; k < NUM_ECC_DIGITS*2 - 1; ++k)
    {
        uint l_min = (k < NUM_ECC_DIGITS ? 0 : (k + 1) - NUM_ECC_DIGITS);
        for(i=l_min; 2*i<=k; ++i)
        {
            uint64_t l_product = (uint64_t)p_left[i] * p_left[k-i];
            if(i < k-i)
            {
                r2 += l_product >> 63;
                l_product *= 2;
            }
            r01 += l_product;
            r2 += (r01 < l_product);
        }
        p_result[k] = (uint32_t)r01;
        r01 = (r01 >> 32) | (((uint64_t)r2) << 32);
        r2 = 0;
    }
    
    p_result[NUM_ECC_DIGITS*2 - 1] = (uint32_t)r01;
}


/* Computes p_result = p_left^2 % curve_p. */
void vli_modSquare_fast(uint32_t *p_result, uint32_t *p_left)
{
    uint32_t l_product[2 * NUM_ECC_DIGITS];
    vli_square(l_product, p_left);
    #ifdef DEBUG 
    ERROR("product");
    NUM_PRINT((l_product+NUM_ECC_DIGITS));
    NUM_PRINT(l_product);
    #endif
    vli_mmod_fast(p_result, l_product);
    #ifdef DEBUG 
    ERROR("moded product");
    NUM_PRINT(p_result);
    #endif
}
