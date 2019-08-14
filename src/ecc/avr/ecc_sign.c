/** !
 * @file ecc_sign.c
 * @author Wang Ruikai 
 * @date July 31th, 2019
 * @brief the signature generation of ECDSA
 * */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "avr/curves.h"
#include "avr/field.h"
#include "avr/ecc.h"
#include "avr/point.h"
#include "avr/hash.h"


uint8_t *ecdsa_sign_new() 
{
    uint8_t *rtn = (uint8_t *)malloc(2 * ECC_BYTES);
    return rtn;
}

void ecdsa_sign_print(uint8_t *signature) {
    uint8_t i;
    for (i=0; i<2 * ECC_BYTES; i++) {
        if(i%4==0) printf(" ");
        if(i%16==0) printf(" \n");
        printf("%02x", signature[i] & 0xff);
    }
    printf("\n\n");
   
}


uint8_t ecdsa_sign(uint8_t *p_signature, const uint8_t p_privateKey[ECC_BYTES], 
                uint8_t *p_message, size_t message_len) 
{

    uint8_t k[ECC_BYTES];
    uint8_t *l_tmp;
    uint8_t l_s[ECC_BYTES];
    EccPoint p;
    unsigned l_tries = 0;
    
    uint8_t *hash = sha_256_new(message_len);
    sha_256(hash, p_message, message_len);

    #ifdef DEBUG 
    ERROR("[sign] hash value");
    NUM_PRINT(hash);
    #endif

    do
    {
        if(!getRandomNumber(k) || (l_tries++ >= MAX_TRIES))
        {
            ERROR("cannot generate random number k");
            goto end;
        }
        if(vli_isZero(k))
        {
            continue;
        }
    
        while(vli_cmp(curve_n, k) != 1)
        {
            vli_sub(k, k, curve_n);
        }
    
        /* tmp = k * G */
        EccPoint_mult(&p, &curve_G, k);
        #ifdef DEBUG 
        ERROR("k used in signature");
        NUM_PRINT(k)
        #endif
    
        /* r = x1 (mod n) */
        while(vli_cmp(curve_n, p.x) != 1)
        {
            vli_sub(p.x, p.x, curve_n);
        }
    } while(vli_isZero(p.x));

    memcpy(p_signature, p.x, ECC_BYTES);
    
    l_tmp = p_privateKey;
    vli_modMult_fast(l_s, p.x, l_tmp);
    l_tmp = hash;
    vli_modAdd(l_s, l_tmp, l_s, curve_p); /* s = e + r*d */

    #ifdef DEBUG 
    ERROR("s");
    NUM_PRINT(l_s);
    ERROR("p.x");
    NUM_PRINT(p.x);
    #endif

    vli_modInv(k, k, curve_p); /* k = 1 / k */
    vli_modMult_fast(l_s, l_s, k);
    while (vli_cmp(curve_n, p.x) != 1) {
        vli_sub(l_s, l_s, curve_n);
    }
 
    memcpy(p_signature+ECC_BYTES, l_s, ECC_BYTES);
    
    sha_256_free(hash);
    return 1;

end:
    sha_256_free(hash);
    return 0;
}
