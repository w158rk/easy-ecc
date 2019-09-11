#include <curves.h>
#include <field.h>


#define HIGH_HALF_MASK  0xffffffff00000000ull
#define LOW_HALF_MASK   0x00000000ffffffffull

#undef DEBUG
#if ECC_CURVE == secp128r1

/* Computes p_result = p_product % curve_p.
   See algorithm 5 and 6 from http://www.isys.uni-klu.ac.at/PDF/2001-0126-MT.pdf */
void vli_mmod_fast(uint32_t *p_result, uint32_t *p_product)
{
    uint32_t l_tmp[NUM_ECC_DIGITS];
    int l_carry;

    vli_set(p_result, p_product);
    #ifdef DEBUG
    NUM_PRINT((p_product+NUM_ECC_DIGITS));
    NUM_PRINT(p_product);
    #endif    
    l_tmp[0] = p_product[4];
    l_tmp[1] = p_product[5];
    l_tmp[2] = p_product[6];
    l_tmp[3] = (p_product[4] << 1) | (p_product[7] & 0x00000001);

    // l_tmp[1] = (p_product[3] & 0x1FFFFFFFFull) | (p_product[2] << 33);
    l_carry = vli_add(p_result, p_result, l_tmp);
    #ifdef DEBUG
    ERROR("tmp1");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif    
    l_tmp[0] = (p_product[5] << 1)  | (p_product[4] >> 31);
    l_tmp[1] = (p_product[6] << 1)  | (p_product[5] >> 31);
    // l_tmp[0] = (p_product[2] >> 31) | (p_product[3] << 33);
    l_tmp[2] = (p_product[7] << 1) | (p_product[6] >> 31);
    l_tmp[3] = (p_product[5] << 2) | ((p_product[4] >> 30) & 0x00000002) | (p_product[7] >> 31);
    // l_tmp[1] = (p_product[3] >> 31) | ((p_product[2] & 0xFFFFFFFF80000000ull) << 2);
    l_carry += vli_add(p_result, p_result, l_tmp);
    #ifdef DEBUG
    ERROR("tmp2");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif    
    // l_tmp[0] = (p_product[2] >> 62) | (p_product[3] << 2);
    l_tmp[0] = (p_product[5] >> 30) | (p_product[6] << 2);
    l_tmp[1] = (p_product[6] >> 30) | (p_product[7] << 2);
    l_tmp[2] = (p_product[7] >> 30);
    l_tmp[3] = (p_product[6] << 3) | ((p_product[5] & 0xC0000000) >> 29);
    // l_tmp[1] = (p_product[3] >> 62) | ((p_product[2] & 0xC000000000000000ull) >> 29) | (p_product[3] << 35);
    l_carry += vli_add(p_result, p_result, l_tmp);
    #ifdef DEBUG
    ERROR("tmp3");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif    
    l_tmp[0] = (p_product[6] >> 29) | (p_product[7] << 3);
    l_tmp[1] = (p_product[7] >> 29);
    l_tmp[2] = 0;
    l_tmp[3] = (p_product[7] << 4) | ((p_product[6] >> 28) & 0x0e);
    // l_tmp[1] = ((p_product[3] & 0xFFFFFFFFE0000000ull) << 4);
    l_carry += vli_add(p_result, p_result, l_tmp);
    #ifdef DEBUG
    ERROR("tmp4");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif    
    l_tmp[0] = (p_product[7] >> 28);
    l_tmp[1] = 0;
    l_tmp[2] = 0;
    l_tmp[3] = (p_product[7] & 0xFFFFFFFE);
    l_carry += vli_add(p_result, p_result, l_tmp);
    #ifdef DEBUG
    ERROR("tmp5");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif    
    l_tmp[0] = 0;
    l_tmp[1] = 0;
    l_tmp[2] = 0;
    l_tmp[3] = (p_product[7] >> 27) & 0x1e;
    l_carry += vli_add(p_result, p_result, l_tmp);
    #ifdef DEBUG
    ERROR("tmp6");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif    
    while(l_carry || vli_cmp(curve_p, p_result) != 1)
    {
        l_carry -= vli_sub(p_result, p_result, curve_p);
    }
    #ifdef DEBUG
    ERROR("final result of mod");
    NUM_PRINT(p_result);
    #endif  
}

#elif ECC_CURVE == secp192r1

/* Computes p_result = p_product % curve_p.
   See algorithm 5 and 6 from http://www.isys.uni-klu.ac.at/PDF/2001-0126-MT.pdf */
void vli_mmod_fast(uint32_t *p_result, uint32_t *p_product)
{
    uint32_t l_tmp[NUM_ECC_DIGITS];
    int l_carry;
    
    vli_set(p_result, p_product);
    
    vli_set(l_tmp, &p_product[6]);
    l_carry = vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = 0;
    l_tmp[1] = 0;
    l_tmp[2] = p_product[6];
    l_tmp[3] = p_product[7];
    l_tmp[4] = p_product[8];
    l_tmp[5] = p_product[9];
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    l_tmp[0] = l_tmp[2] = p_product[10];
    l_tmp[1] = l_tmp[3] = p_product[11];
    l_tmp[4] = 0;
    l_tmp[5] = 0;
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    while(l_carry || vli_cmp(curve_p, p_result) != 1)
    {
        l_carry -= vli_sub(p_result, p_result, curve_p);
    }
}

#elif ECC_CURVE == secp256r1

/* Computes p_result = p_product % curve_p
   from http://www.nsa.gov/ia/_files/nist-routines.pdf */
/**
 * @brief mp_mod_256(r,a)
 * @p_result 256 bits 
 * @p_product 256 bits
 */

void vli_mmod_slow(uint32_t *p_result, uint32_t *p_product)
{
    // mod by substract 
    uint32_t expand_p[2 * NUM_ECC_DIGITS] = {0};
    uint32_t expand_tmp[2 * NUM_ECC_DIGITS] = {0};    
    uint32_t expand_result[2 * NUM_ECC_DIGITS] = {0};
    
    int i;
    for (i=0; i<NUM_ECC_DIGITS; i++){
        expand_p[i] = curve_p[i];
    }
    
    #ifdef DEBUG 
        printf("expanded p");
        LONG_NUM_PRINT(expand_p);
    #endif

    for (i=0; i<NUM_ECC_DIGITS*2; i++){
        expand_result[i] = p_product[i];
    }

    #ifdef DEBUG 
        printf("expanded result");
        LONG_NUM_PRINT(expand_result);
 
        printf("expanded result higher bits");
        uint32_t *p = expand_result + NUM_ECC_DIGITS;
        NUM_PRINT(p);

        printf("is zero ? :  %d\n", vli_isZero(expand_result+NUM_ECC_DIGITS));
    #endif


    while(0 == vli_isZero(expand_result+NUM_ECC_DIGITS) || 1 != vli_cmp(curve_p, expand_result)) {

        uint len = vli_numBits(expand_result + NUM_ECC_DIGITS)-1;

        // #ifdef DEBUG 
        // printf("%d\n", len);
        // #endif

        vli_clear(expand_tmp+4);
        vli_clear(expand_tmp);
        // left shift
        uint32_t carry = vli_lshift(expand_tmp+len/64, expand_p, len%64);
        expand_tmp[len/64+4] = carry;

        uint32_t l_borrow = 0;
        uint i;
        for(i=0; i<NUM_ECC_DIGITS * 2; ++i)
        {
            uint32_t l_diff = expand_result[i] - expand_tmp[i] - l_borrow;
            l_borrow = (l_diff > expand_result[i]);
            expand_result[i] = l_diff;
        }

        #ifdef DEBUG 
        printf("expanded tmp");
        LONG_NUM_PRINT(expand_tmp);
        printf("expanded result");
        LONG_NUM_PRINT(expand_result);
        #endif


    }

    #ifdef DEBUG 
    printf("finish \n");
    #endif

    vli_set(p_result, expand_result);


}

void vli_mmod_fast(uint32_t *p_result, uint32_t *p_product)
{
    uint32_t l_tmp[NUM_ECC_DIGITS];
    int l_carry;
    
    /* t */
    vli_set(p_result, p_product);
    
    /* s1 */
    l_tmp[0] = 0;
    l_tmp[1] = p_product[5] & HIGH_HALF_MASK;
    l_tmp[2] = p_product[6];
    l_tmp[3] = p_product[7];
    l_carry = vli_lshift(l_tmp, l_tmp, 1);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* s2 */
    l_tmp[1] = p_product[6] << 32;
    l_tmp[2] = (p_product[6] >> 32) | (p_product[7] << 32);
    l_tmp[3] = p_product[7] >> 32;
    l_carry += vli_lshift(l_tmp, l_tmp, 1);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* s3 */
    l_tmp[0] = p_product[4];
    l_tmp[1] = p_product[5] & LOW_HALF_MASK;
    l_tmp[2] = 0;
    l_tmp[3] = p_product[7];
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* s4 */
    l_tmp[0] = (p_product[4] >> 32) | (p_product[5] << 32);
    l_tmp[1] = (p_product[5] >> 32) | (p_product[6] & HIGH_HALF_MASK);
    l_tmp[2] = p_product[7];
    l_tmp[3] = (p_product[6] >> 32) | (p_product[4] << 32);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    /* d1 */
    l_tmp[0] = (p_product[5] >> 32) | (p_product[6] << 32);
    l_tmp[1] = (p_product[6] >> 32);
    l_tmp[2] = 0;
    l_tmp[3] = (p_product[4] & LOW_HALF_MASK) | (p_product[5] << 32);
    l_carry -= vli_sub(p_result, p_result, l_tmp);
    
    /* d2 */
    l_tmp[0] = p_product[6];
    l_tmp[1] = p_product[7];
    l_tmp[2] = 0;
    l_tmp[3] = (p_product[4] >> 32) | (p_product[5] & HIGH_HALF_MASK);
    l_carry -= vli_sub(p_result, p_result, l_tmp);
    
    /* d3 */
    l_tmp[0] = (p_product[6] >> 32) | (p_product[7] << 32);
    l_tmp[1] = (p_product[7] >> 32) | (p_product[4] << 32);
    l_tmp[2] = (p_product[4] >> 32) | (p_product[5] << 32);
    l_tmp[3] = (p_product[6] << 32);
    l_carry -= vli_sub(p_result, p_result, l_tmp);
    
    /* d4 */
    l_tmp[0] = p_product[7];
    l_tmp[1] = p_product[4] & HIGH_HALF_MASK;
    l_tmp[2] = p_product[5];
    l_tmp[3] = p_product[6] & HIGH_HALF_MASK;
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

void omega_mult(uint32_t *p_result, uint32_t *p_right)
{
    #ifdef DEBUG 
    ERROR("begin");
    NUM_PRINT(p_right);
    #endif

    uint32_t l_tmp[NUM_ECC_DIGITS];
    uint32_t l_carry, l_diff;
    
    /* Multiply by (2^128 + 2^96 - 2^32 + 1). */
    vli_set(p_result, p_right); /* 1 */
    l_carry = vli_lshift(l_tmp, p_right, 32);
    #ifdef DEBUG 
    ERROR("p_result");
    NUM_PRINT(p_result);
    ERROR("l_carry");
    printf("%08x\n", l_carry);
    #endif
    p_result[1 + NUM_ECC_DIGITS] = l_carry + vli_add(p_result + 1, p_result + 1, l_tmp); /* 2^96 + 1 */
    
    p_result[2 + NUM_ECC_DIGITS] = vli_add(p_result + 2, p_result + 2, p_right); /* 2^128 + 2^96 + 1 */
    #ifdef DEBUG 
    ERROR("p_result");
    NUM_PRINT(p_result);
    #endif
    l_carry += vli_sub(p_result, p_result, l_tmp); /* 2^128 + 2^96 - 2^32 + 1 */
    #ifdef DEBUG 
    ERROR("l_carry");
    printf("%016lx\n", l_carry);
    #endif
    l_diff = p_result[NUM_ECC_DIGITS] - l_carry;
    #ifdef DEBUG 
    ERROR("l_diff");
    printf("%016lx\n", l_diff);
    #endif
    if(l_diff > p_result[NUM_ECC_DIGITS])
    { /* Propagate borrow if necessary. */
        uint i;
        for(i = 1 + NUM_ECC_DIGITS; ; ++i)
        {
            --p_result[i];
            if(p_result[i] != (uint32_t)-1)
            {
                break;
            }
        }
    }
    p_result[NUM_ECC_DIGITS] = l_diff;
}

/* Computes p_result = p_product % curve_p
    see PDF "Comparing Elliptic Curve Cryptography and RSA on 8-bit CPUs"
    section "Curve-Specific Optimizations" */
void vli_mmod_fast(uint32_t *p_result, uint32_t *p_product)
{
    #ifdef DEBUG 
    ERROR("begin");
    NUM_PRINT((p_product+NUM_ECC_DIGITS));
    NUM_PRINT(p_product);
    #endif
    
    uint32_t l_tmp[2*NUM_ECC_DIGITS];
     
    while(!vli_isZero(p_product + NUM_ECC_DIGITS)) /* While c1 != 0 */
    {
        uint32_t l_carry = 0;
        uint i;
        
        vli_clear(l_tmp);
        vli_clear(l_tmp + NUM_ECC_DIGITS);
        omega_mult(l_tmp, p_product + NUM_ECC_DIGITS); /* tmp = w * c1 */
        vli_clear(p_product + NUM_ECC_DIGITS); /* p = c0 */
        #ifdef DEBUG 
        ERROR("l_tmp");
        NUM_PRINT((l_tmp+NUM_ECC_DIGITS));
        NUM_PRINT(l_tmp);
        #endif
        
        /* (c1, c0) = c0 + w * c1 */
        for(i=0; i<NUM_ECC_DIGITS+3; ++i)
        {
            uint32_t l_sum = p_product[i] + l_tmp[i] + l_carry;
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

