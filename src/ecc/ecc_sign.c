/** !
 * @file ecc_opt.c
 * @author Wang Ruikai 
 * @date July 15th, 2019
 * @brief the point arithmetic operation
 * */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "curves.h"
#include "field.h"
#include "ecc.h"
#include "point.h"

#define UMAX(a, b) a>b ? a : b
#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s", __FILE__, \
                __LINE__, __func__, info) 
int ecdsa_sign(const uint8_t p_privateKey[ECC_BYTES], 
                const uint8_t p_hash[ECC_BYTES], 
                uint8_t p_signature[ECC_BYTES*2])
{
    uint64_t k[NUM_ECC_DIGITS];
    uint64_t l_tmp[NUM_ECC_DIGITS];
    uint64_t l_s[NUM_ECC_DIGITS];
    EccPoint p;
    unsigned l_tries = 0;
    
    do
    {
        if(!getRandomNumber(k) || (l_tries++ >= MAX_TRIES))
        {
            return 0;
        }
        if(vli_isZero(k))
        {
            continue;
        }
    
        if(vli_cmp(curve_n, k) != 1)
        {
            vli_sub(k, k, curve_n);
        }
    
        /* tmp = k * G */
        EccPoint_mult(&p, &curve_G, k);
    
        /* r = x1 (mod n) */
        if(vli_cmp(curve_n, p.x) != 1)
        {
            vli_sub(p.x, p.x, curve_n);
        }
    } while(vli_isZero(p.x));

    ecc_native2bytes(p_signature, p.x);
    
    ecc_bytes2native(l_tmp, p_privateKey);
    vli_modMult(l_s, p.x, l_tmp, curve_n); /* s = r*d */
    ecc_bytes2native(l_tmp, p_hash);
    vli_modAdd(l_s, l_tmp, l_s, curve_n); /* s = e + r*d */
    vli_modInv(k, k, curve_n); /* k = 1 / k */
    vli_modMult(l_s, l_s, k, curve_n); /* s = (e + r*d) / k */
    ecc_native2bytes(p_signature + ECC_BYTES, l_s);
    
    return 1;
}
