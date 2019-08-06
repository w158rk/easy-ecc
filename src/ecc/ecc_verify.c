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

    uint64_t u1[NUM_ECC_DIGITS], u2[NUM_ECC_DIGITS];
    uint64_t z[NUM_ECC_DIGITS];
    uint64_t rx[NUM_ECC_DIGITS];
    
    uint64_t l_r[NUM_ECC_DIGITS], l_s[NUM_ECC_DIGITS];

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

    EccPoint_mult(&l_sum, &curve_G, u1);
    EccPoint_mult(&l_public, &l_public, u2);
    EccPoint_add_jacobian(l_sum.x, l_sum.y, l_sum.x, l_sum.y, l_public.x, l_public.y);
    
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


int ecdsa_verify_origin(const uint8_t p_publicKey[ECC_BYTES+1], 
                const uint8_t p_hash[ECC_BYTES], 
                const uint8_t p_signature[ECC_BYTES*2])
{
    uint64_t u1[NUM_ECC_DIGITS], u2[NUM_ECC_DIGITS];
    uint64_t z[NUM_ECC_DIGITS];
    EccPoint l_public, l_sum;
    uint64_t rx[NUM_ECC_DIGITS];
    uint64_t ry[NUM_ECC_DIGITS];
    uint64_t tx[NUM_ECC_DIGITS];
    uint64_t ty[NUM_ECC_DIGITS];
    uint64_t tz[NUM_ECC_DIGITS];
    
    uint64_t l_r[NUM_ECC_DIGITS], l_s[NUM_ECC_DIGITS];
    
    ecc_point_decompress(&l_public, p_publicKey);
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

    /* Calculate u1 and u2. */
    vli_modInv(z, l_s, curve_n); /* Z = s^-1 */
    ecc_bytes2native(u1, p_hash);
    vli_modMult(u1, u1, z, curve_n); /* u1 = e/s */
    vli_modMult(u2, l_r, z, curve_n); /* u2 = r/s */
    
    /* Calculate l_sum = G + Q. */
    vli_set(l_sum.x, l_public.x);
    vli_set(l_sum.y, l_public.y);
    vli_set(tx, curve_G.x);
    vli_set(ty, curve_G.y);
    vli_modSub(z, l_sum.x, tx, curve_p); /* Z = x2 - x1 */
    XYcZ_add(tx, ty, l_sum.x, l_sum.y);
    vli_modInv(z, z, curve_p); /* Z = 1/Z */
    mult_z(l_sum.x, l_sum.y, l_sum.x, l_sum.y, z);
    
    /* Use Shamir's trick to calculate u1*G + u2*Q */
    EccPoint *l_points[4] = {NULL, &curve_G, &l_public, &l_sum};
    uint l_numBits = UMAX(vli_numBits(u1), vli_numBits(u2));
    
    EccPoint *l_point = l_points[(!!vli_testBit(u1, l_numBits-1)) | ((!!vli_testBit(u2, l_numBits-1)) << 1)];
    vli_set(rx, l_point->x);
    vli_set(ry, l_point->y);
    // vli_clear(z);
    // z[0] = 1;

    int i;
    for(i = l_numBits - 2; i >= 0; --i)
    {
        EccPoint_double_jacobian(rx, ry, rx, ry);
        
        int l_index = (!!vli_testBit(u1, i)) | ((!!vli_testBit(u2, i)) << 1);
        EccPoint *l_point = l_points[l_index];
        if(l_point)
        {
            vli_set(tx, l_point->x);
            vli_set(ty, l_point->y);
            mult_z(rx, ry, rx, ry, z);
            vli_modSub(tz, rx, tx, curve_p); /* Z = x2 - x1 */
            XYcZ_add(tx, ty, rx, ry);
            vli_modMult_fast(z, z, tz);
        }
    }

    vli_modInv(z, z, curve_p); /* Z = 1/Z */
    mult_z(rx, ry, rx, ry, z);
    
    /* v = x1 (mod n) */
    if(vli_cmp(curve_n, rx) != 1)
    {
        vli_sub(rx, rx, curve_n);
    }

    /* Accept only if v == r. */
    return (vli_cmp(rx, l_r) == 0);
}
 