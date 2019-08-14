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

#include "avr/curves.h"
#include "avr/field.h"
#include "avr/ecc.h"
#include "avr/point.h"
#include "avr/hash.h"


uint8_t ecdsa_verify(const uint8_t p_publicKey[ECC_BYTES+1], 
                const uint8_t p_message[ECC_BYTES], 
                size_t message_len,
                const uint8_t p_signature[ECC_BYTES*2])
{
    // check the point

    EccPoint l_public, l_sum;    
    ecc_point_decompress(&l_public, p_publicKey);

    if( 0 == vli_cmp(l_public.x, curve_G.x)) {

        ERROR(" the public key equals to the group generator");
        goto end;

    }

    if( 0 == check(l_public.x, l_public.y)) {

        ERROR(" the public key is not on the curve");
        goto end;

    }

    uint8_t u1[ECC_BYTES], u2[ECC_BYTES];
    uint8_t z[ECC_BYTES];
    uint8_t rx[ECC_BYTES];
    
    uint8_t *l_r=p_signature, *l_s=p_signature+ECC_BYTES;
    
    if(vli_isZero(l_r) || vli_isZero(l_s))
    { /* r, s must not be 0. */
        return 0;
    }
    
    if(vli_cmp(curve_n, l_r) != 1 || vli_cmp(curve_n, l_s) != 1)
    { /* r, s must be < n. */
        return 0;
    }

    uint8_t *p_hash = sha_256_new(message_len);
    sha_256(p_hash, p_message, message_len);

    /* Calculate u1 and u2. */
    vli_modInv(z, l_s, curve_p); /* Z = s^-1 */
    #ifdef DEBUG 
    ERROR("1/s");
    NUM_PRINT(z);
    ERROR("r");
    NUM_PRINT(l_r);
    #endif
    vli_modMult_fast(u1, p_hash, z); /* u1 = e/s */
    vli_modMult_fast(u2, l_r, z); /* u2 = r/s */

    #ifdef DEBUG 
    ERROR("u1");
    NUM_PRINT(u1);
    ERROR("u2");
    NUM_PRINT(u2);
    #endif

    EccPoint_mult(&l_sum, &curve_G, u1);
    EccPoint_mult(&l_public, &l_public, u2);
    EccPoint_add_jacobian(l_sum.x, l_sum.y, l_sum.x, l_sum.y, l_public.x, l_public.y);
    int test = check(l_sum.x, l_sum.y);
    printf("test : %d\n", test);
    
    vli_set(rx, l_sum.x);
    /* v = x1 (mod q) */
    if(vli_cmp(curve_n, rx) != 1)
    {
        vli_sub(rx, rx, curve_n);
    }
    #ifdef DEBUG
    ERROR("result of verification"); 
    NUM_PRINT(rx);
    NUM_PRINT(l_r);
    #endif

    if (0 == vli_cmp(rx, l_r)) {
        return 1;
    }

end:    
    return 0;

}

