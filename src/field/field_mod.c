#include <curves.h>
#include <field.h>

#include<string.h>

#undef DEBUG 
#ifdef DEBUG 
#include<stdio.h>
#endif


#if ECC_CURVE == secp128r1

/* Computes p_result = p_product % curve_p.
   See algorithm 5 and 6 from http://www.isys.uni-klu.ac.at/PDF/2001-0126-MT.pdf */
void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product)
{

    uint8_t l_tmp[ECC_BYTES];
    uint8_t l_carry;


    vli_set(p_result, p_product);

    #ifdef DEBUG
    NUM_PRINT(p_result);
    #endif

    memcpy(l_tmp, p_product+16, 12);
    l_tmp[15] = (p_product[19] << 1) | (p_product[18] >> 7);
    l_tmp[14] = (p_product[18] << 1) | (p_product[17] >> 7);
    l_tmp[13] = (p_product[17] << 1) | (p_product[16] >> 7);
    l_tmp[12] = (p_product[16] << 1) | (p_product[28] & 0x01);
    l_carry = vli_add(p_result, p_result, l_tmp);

    #ifdef DEBUG
    ERROR("tmp1");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif

    l_tmp[15] = (p_product[23]<<2) | (p_product[22]>>6);
    l_tmp[14] = (p_product[22]<<2) | (p_product[21]>>6);
    l_tmp[13] = (p_product[21]<<2) | (p_product[20]>>6);
    l_tmp[12] = (p_product[20]<<2) | ((p_product[19]>>6) & 0x3e) | (p_product[31]>>7);
    l_tmp[11] = (p_product[31]<<1) | (p_product[30]>>7);
    l_tmp[10] = (p_product[30]<<1) | (p_product[29]>>7);
    l_tmp[9] = (p_product[29]<<1) | (p_product[28]>>7);
    l_tmp[8] = (p_product[28]<<1) | (p_product[27]>>7);

    l_tmp[7] = (p_product[27] << 1) | (p_product[26] >> 7);
    l_tmp[6] = (p_product[26] << 1) | (p_product[25] >> 7);
    l_tmp[5] = (p_product[25] << 1) | (p_product[24] >> 7);
    l_tmp[4] = (p_product[24] << 1) | (p_product[23] >> 7);
    l_tmp[3] = (p_product[23] << 1) | (p_product[22] >> 7);
    l_tmp[2] = (p_product[22] << 1) | (p_product[21] >> 7);
    l_tmp[1] = (p_product[21] << 1) | (p_product[20] >> 7);
    l_tmp[0] = (p_product[20] << 1) | (p_product[19] >> 7);
    l_carry += vli_add(p_result, p_result, l_tmp);

    #ifdef DEBUG
    ERROR("tmp2");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif

    l_tmp[0] = (p_product[24]<<2) | (p_product[23]>>6);
    l_tmp[1] = (p_product[25]<<2) | (p_product[24]>>6);
    l_tmp[2] = (p_product[26]<<2) | (p_product[25]>>6);
    l_tmp[3] = (p_product[27]<<2) | (p_product[26]>>6);
    l_tmp[4] = (p_product[28]<<2) | (p_product[27]>>6);
    l_tmp[5] = (p_product[29]<<2) | (p_product[28]>>6);
    l_tmp[6] = (p_product[30]<<2) | (p_product[29]>>6);
    l_tmp[7] = (p_product[31]<<2) | (p_product[30]>>6);
    l_tmp[15] = (p_product[27]<<3) | (p_product[26]>>5);
    l_tmp[14] = (p_product[26]<<3) | (p_product[25]>>5);
    l_tmp[13] = (p_product[25]<<3) | (p_product[24]>>5);
    l_tmp[12] = (p_product[24]<<3) | ((p_product[23]>>5) & 0xfe);
    l_tmp[11] = 0;
    l_tmp[10] = 0;
    l_tmp[9] = 0;
    l_tmp[8] = p_product[31] >> 6;
    l_carry += vli_add(p_result, p_result, l_tmp);

    #ifdef DEBUG
    ERROR("tmp3");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif

    l_tmp[7] = 0;
    l_tmp[6] = 0;
    l_tmp[5] = 0;
    l_tmp[4] = p_product[31] >> 5;
    l_tmp[3] = (p_product[31]<<3) | (p_product[30]>>5);
    l_tmp[2] = (p_product[30]<<3) | (p_product[29]>>5);
    l_tmp[1] = (p_product[29]<<3) | (p_product[28]>>5);
    l_tmp[0] = (p_product[28]<<3) | (p_product[27]>>5);
    l_tmp[15] = (p_product[31]<<4) | (p_product[30]>>4);
    l_tmp[14] = (p_product[30]<<4) | (p_product[29]>>4);
    l_tmp[13] = (p_product[29]<<4) | (p_product[28]>>4);
    l_tmp[12] = (p_product[28]<<4) | (p_product[27]>>4) & 0xfe;
    l_tmp[11] = 0;
    l_tmp[10] = 0;
    l_tmp[9] = 0;
    l_tmp[8] = 0;
    l_carry += vli_add(p_result, p_result, l_tmp);

    #ifdef DEBUG
    ERROR("tmp4");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif

    vli_clear(l_tmp);
    l_tmp[0] = p_product[31] >> 4;
    l_tmp[15] = p_product[31];
    l_tmp[14] = p_product[30];
    l_tmp[13] = p_product[29];
    l_tmp[12] = p_product[28] & 0xfe;
    l_carry += vli_add(p_result, p_result, l_tmp);

    #ifdef DEBUG
    ERROR("tmp5");
    NUM_PRINT(l_tmp);
    NUM_PRINT(p_result);
    #endif

    vli_clear(l_tmp);
    l_tmp[12] = (p_product[31]&0xf0) >> 3;
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
void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product)
{
    uint8_t l_tmp[ECC_BYTES];
    uint8_t l_carry;
    
    vli_set(p_result, p_product);
    vli_set(l_tmp, p_product + 3*8);
    l_carry = vli_add(p_result, p_result, l_tmp);
    
    vli_clear(l_tmp);
    memcpy(l_tmp + 8, p_product + 3*8, 8);
    memcpy(l_tmp + 2*8, p_product + 4*8, 8);
    l_carry += vli_add(p_result, p_result, l_tmp);
    
    vli_clear(l_tmp);
    memcpy(l_tmp, p_product + 5*8, 8);
    memcpy(l_tmp + 8, p_product + 5*8, 8);
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

    #ifdef DEBUG 
    ERROR("begin");
    NUM_PRINT(p_right);
    #endif

    uint8_t l_tmp[ECC_BYTES] = {0};
    uint8_t l_tmp1[ECC_BYTES] = {0};
    uint64_t l_carry, l_diff, l_carry1, l_diff1;
    
    /* Multiply by (2^128 + 2^96 - 2^32 + 1). */
    vli_set(p_result, p_right); /* 1 */
    // shift left by 32 bit
    uint8_t i;
    for (i=0; i<ECC_BYTES-4; i++) {
        l_tmp[i+4] = p_right[i];
    }

    l_carry = (uint64_t) p_right[ECC_BYTES-4];
    l_carry |= (uint64_t) p_right[ECC_BYTES-3] << 8;
    l_carry |= (uint64_t) p_right[ECC_BYTES-2] << 16 ;
    l_carry |= (uint64_t) p_right[ECC_BYTES-1] << 24;
    #ifdef DEBUG 
    ERROR("l_carry");
    printf("%08x\n", l_carry);
    #endif

    l_carry1 = l_carry + vli_add(p_result + 8, p_result + 8, l_tmp); /* 2^96 + 1 */
    #ifdef DEBUG 
    ERROR("p_result");
    NUM_PRINT(p_result);
    ERROR("l_carry1");
    printf("%016lx\n", l_carry1);
    #endif
    uint8_t *p = p_result + ECC_BYTES + 8;
    p[0] = (uint8_t) l_carry1;
    p[1] = (uint8_t) (l_carry1 >> 8);
    p[2] = (uint8_t) (l_carry1 >> 16);
    p[3] = (uint8_t) (l_carry1 >> 24);
    p[4] = (uint8_t) (l_carry1 >> 32);

    l_carry1 = vli_add(p_result + 2*8, p_result + 2*8, p_right); /* 2^128 + 2^96 + 1 */
    #ifdef DEBUG 
    ERROR("p_result");
    NUM_PRINT(p_result);
    #endif
    p = p_result + ECC_BYTES + 2*8;
    p[0] = (uint8_t) l_carry1;
    p[1] = (uint8_t) (l_carry1 >> 8);
    p[2] = (uint8_t) (l_carry1 >> 16);
    p[3] = (uint8_t) (l_carry1 >> 24);
    p[4] = (uint8_t) (l_carry1 >> 32);
    
    l_carry += vli_sub(p_result, p_result, l_tmp); /* 2^128 + 2^96 - 2^32 + 1 */
    l_diff = (uint64_t) p_result[ECC_BYTES];
    l_diff |= (uint64_t) p_result[ECC_BYTES+1] << 8;
    l_diff |= (uint64_t) p_result[ECC_BYTES+2] << 16 ;
    l_diff |= (uint64_t) p_result[ECC_BYTES+3] << 24;
    l_diff |= (uint64_t) p_result[ECC_BYTES+4] << 32;
    l_diff |= (uint64_t) p_result[ECC_BYTES+5] << 40;
    l_diff |= (uint64_t) p_result[ECC_BYTES+6] << 48;
    l_diff |= (uint64_t) p_result[ECC_BYTES+7] << 56;
    #ifdef DEBUG 
    ERROR("l_diff");
    printf("%016lx\n", l_diff);
    #endif
    l_diff1 = l_diff - l_carry;
    #ifdef DEBUG 
    ERROR("l_diff");
    printf("%016lx\n", l_diff1);
    #endif
    if(l_diff1 > l_diff)
    { /* Propagate borrow if necessary. */
        #ifdef DEBUG
        #endif
        uint8_t i;
        for(i = 8 + ECC_BYTES; ; ++i)
        {
            --p_result[i];
            if(p_result[i] != (uint8_t)-1)
            {
                break;
            }
        }
        // convert 
        l_diff = (uint64_t) p_right[ECC_BYTES];
        l_diff |= (uint64_t) p_right[ECC_BYTES+1] << 8;
        l_diff |= (uint64_t) p_right[ECC_BYTES+2] << 16 ;
        l_diff |= (uint64_t) p_right[ECC_BYTES+3] << 24;
        l_diff |= (uint64_t) p_right[ECC_BYTES+4] << 32;
        l_diff |= (uint64_t) p_right[ECC_BYTES+5] << 40;
        l_diff |= (uint64_t) p_right[ECC_BYTES+6] << 48;
        l_diff |= (uint64_t) p_right[ECC_BYTES+7] << 56;
    }
    p_result[ECC_BYTES] = (uint8_t) l_diff1;
    p_result[ECC_BYTES+1] = (uint8_t) (l_diff1 >> 8);
    p_result[ECC_BYTES+2] = (uint8_t) (l_diff1 >> 16);
    p_result[ECC_BYTES+3] = (uint8_t) (l_diff1 >> 24);
    p_result[ECC_BYTES+4] = (uint8_t) (l_diff1 >> 32);
    p_result[ECC_BYTES+5] = (uint8_t) (l_diff1 >> 40);
    p_result[ECC_BYTES+6] = (uint8_t) (l_diff1 >> 48);
    p_result[ECC_BYTES+7] = (uint8_t) (l_diff1 >> 56);
}

/* Computes p_result = p_product % curve_p
    see PDF "Comparing Elliptic Curve Cryptography and RSA on 8-bit CPUs"
    section "Curve-Specific Optimizations" */
void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product)
{
    #ifdef DEBUG 
    ERROR("begin");
    NUM_PRINT((p_product+ECC_BYTES));
    NUM_PRINT(p_product);
    #endif

    uint8_t l_tmp[2*ECC_BYTES];
     
    while(!vli_isZero(p_product + ECC_BYTES)) /* While c1 != 0 */
    {
        uint8_t l_carry = 0;
        uint8_t i;
        
        vli_clear(l_tmp);
        vli_clear(l_tmp + ECC_BYTES);
        omega_mult(l_tmp, p_product + ECC_BYTES); /* tmp = w * c1 */
        #ifdef DEBUG 
        ERROR("l_tmp");
        NUM_PRINT((l_tmp+ECC_BYTES));
        NUM_PRINT(l_tmp);
        #endif
        vli_clear(p_product + ECC_BYTES); /* p = c0 */
        
        /* (c1, c0) = c0 + w * c1 */
        for(i=0; i<ECC_BYTES+3*8; ++i)
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

