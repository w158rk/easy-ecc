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
#include "hash.h"

#define UMAX(a, b) a>b ? a : b
#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s", __FILE__, \
                __LINE__, __func__, info) 

int ecdsa_verify(const char p_publicKey[ECC_BYTES+1], 
                const char p_message[ECC_BYTES], 
                size_t message_len,
                const char p_signature[ECC_BYTES*2])
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

    uint32_t u1[NUM_ECC_DIGITS], u2[NUM_ECC_DIGITS];
    uint32_t z[NUM_ECC_DIGITS];
    uint32_t rx[NUM_ECC_DIGITS];
    
    uint32_t l_r[NUM_ECC_DIGITS], l_s[NUM_ECC_DIGITS];

    ecc_bytes2native(l_r, p_signature);
    ecc_bytes2native(l_s, p_signature + ECC_BYTES);
    
    if(vli_isZero(l_r) || vli_isZero(l_s))
    { /* r, s must not be 0. */
        return 0;
    }
    
    if(vli_cmp(curve_n, l_r) != 1 || vli_cmp(curve_n, l_s) != 1)
    { /* r, s must be < n. */
        return 0;
    }

    char *p_hash = sha_256_new(message_len);
    sha_256(p_hash, p_message, message_len);


    /* Calculate u1 and u2. */
    vli_modInv(z, l_s, curve_n); /* Z = s^-1 */
    ecc_bytes2native(u1, p_hash);
    vli_modMult(u1, u1, z, curve_n); /* u1 = e/s */
    vli_modMult(u2, l_r, z, curve_n); /* u2 = r/s */

    EccPoint_mult_P(&l_sum, u1);
    EccPoint_mult(&l_public, &l_public, u2);

    vli_clear(z);
    z[0] = 1;
    EccPoint_add_jacobian(l_sum.x, l_sum.y, z, l_public.x, l_public.y);
    divide_z(l_sum.x, l_sum.y, l_sum.x, l_sum.y, z);
    
    vli_set(rx, l_sum.x);
    /* v = x1 (mod q) */
    if(vli_cmp(curve_n, rx) != 1)
    {
        vli_sub(rx, rx, curve_n);
    }

    if (0 == vli_cmp(rx, l_r)) {
        return 1;
    }

end:    
    return 0;

}


