#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"

/* Point multiplication algorithm using MontgomeY's ladder with co-Z coordinates.
From http://eprint.iacr.org/2011/338.pdf
*/

#undef DEBUG
uint32_t one[NUM_ECC_DIGITS] = {1};
#ifdef DEBUG 
uint32_t three[NUM_ECC_DIGITS] = {3};
uint32_t four[NUM_ECC_DIGITS] = {4};
#endif


void EccPoint_double_jacobian_v3(uint32_t *X, uint32_t *Y, uint32_t *Z, uint32_t *X1, uint32_t *Y1, uint32_t *Z1) {


    if (vli_isZero(X1) && vli_isZero(Y1)) {
        vli_clear(X);
        vli_clear(Y);
        return ;
    }

    uint32_t t1[NUM_ECC_DIGITS];
    uint32_t t2[NUM_ECC_DIGITS];
    uint32_t t3[NUM_ECC_DIGITS];

    vli_modSquare_fast(t1, Z1);
    vli_modSub(t2, X1, t1, curve_p);
    vli_modAdd(t1, X1, t1, curve_p);
    vli_modMult_fast(t2, t2, t1);
    vli_modAdd(X, t2, t2, curve_p);

    vli_modAdd(t2, t2, X, curve_p);
    vli_modAdd(Y, Y1, Y1, curve_p);
    vli_modMult_fast(Z, Z1, Y);
    vli_modSquare_fast(Y, Y);
    vli_modMult_fast(t3, Y, X1);

    vli_modSquare_fast(Y, Y);
    if (vli_testBit(Y, 0)) {
        /* odd */
        uint32_t carry = vli_add(Y, Y, curve_p);
        vli_rshift1(Y);
        Y[NUM_ECC_DIGITS-1] |= carry << 31;
    } else {
        vli_rshift1(Y);
    }
    vli_modSquare_fast(X, t2);
    vli_modAdd(t1, t3, t3, curve_p);
    vli_modSub(X, X, t1, curve_p);
    vli_modSub(t1, t3, X, curve_p);
    vli_modMult_fast(t1, t1, t2);
    vli_modSub(Y, t1, Y, curve_p);

}


void EccPoint_double_jacobian(uint32_t *X, uint32_t *Y, uint32_t *Z, uint32_t *X1, uint32_t *Y1, uint32_t *Z1)
{

    EccPoint_double_jacobian_v3(X, Y, Z, X1, Y1, Z1);

}