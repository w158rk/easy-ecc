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

int ecdh_shared_secret(const uint8_t p_publicKey[ECC_BYTES+1], 
                        const uint8_t p_privateKey[ECC_BYTES], 
                        uint8_t p_secret[ECC_BYTES])
{
    EccPoint l_public;
    uint64_t l_private[NUM_ECC_DIGITS];
    uint64_t l_random[NUM_ECC_DIGITS];
    
    if(!getRandomNumber(l_random))
    {
        return 0;
    }
    
    ecc_point_decompress(&l_public, p_publicKey);
    ecc_bytes2native(l_private, p_privateKey);
    
    EccPoint l_product;
    // EccPoint_mult(&l_product, &l_public, l_private, l_random);
    EccPoint_mult(&l_product, &l_public, l_private);
    
    ecc_native2bytes(p_secret, l_product.x);
    
    return !EccPoint_isZero(&l_product);
}
