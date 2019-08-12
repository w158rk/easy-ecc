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

#include "curves.h"
#include "field.h"
#include "ecc.h"
#include "point.h"
#include "hash.h"


#define UMAX(a, b) a>b ? a : b
#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s", __FILE__, \
                __LINE__, __func__, info) 

char *ecdsa_sign_new() 
{
    char *rtn = (char *)malloc(2 * ECC_BYTES);
    return rtn;
}

void ecdsa_sign_print(char *signature) {
    int i;
    for (i=0; i<2 * ECC_BYTES; i++) {
        if(i%4==0) printf(" ");
        if(i%16==0) printf(" \n");
        printf("%02x", signature[i] & 0xff);
    }
    printf("\n\n");
   
}


int ecdsa_sign(char *p_signature, const char p_privateKey[ECC_BYTES], 
                char *p_message, size_t message_len) 
{

    uint64_t k[NUM_ECC_DIGITS];
    uint64_t l_tmp[NUM_ECC_DIGITS];
    uint64_t l_s[NUM_ECC_DIGITS];
    EccPoint p;
    unsigned l_tries = 0;
    
    char *hash = sha_256_new(message_len);
    sha_256(hash, p_message, message_len);

    #ifdef DEBUG 
    {
        int i;
        for (i=0; i<256/8; i++) {
            if(i%4==0) printf(" ");
            if(i%16==0) printf(" \n");
            printf("%02x", hash[i] & 0xff);
        }
        printf("\n\n");
    }
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
    
        /* r = x1 (mod n) */
        while(vli_cmp(curve_n, p.x) != 1)
        {
            vli_sub(p.x, p.x, curve_n);
        }
    } while(vli_isZero(p.x));

    ecc_native2bytes(p_signature, p.x);
    
    ecc_bytes2native(l_tmp, p_privateKey);
    vli_modMult(l_s, p.x, l_tmp, curve_n); /* s = r*d */

    ecc_bytes2native(l_tmp, hash);
    vli_modAdd(l_s, l_tmp, l_s, curve_n); /* s = e + r*d */

    vli_modInv(k, k, curve_n); /* k = 1 / k */
    vli_modMult(l_s, l_s, k, curve_n); /* s = (e + r*d) / k */

    ecc_native2bytes(p_signature + ECC_BYTES, l_s);
    
    sha_256_free(hash);
    return 1;

end:
    sha_256_free(hash);
    return 0;
}

int ecdsa_sign_origin(const char p_privateKey[ECC_BYTES], 
                const char p_hash[ECC_BYTES], 
                char p_signature[ECC_BYTES*2])
{
}
