#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"

/* Point multiplication algorithm using Montgomery's ladder with co-Z coordinates.
From http://eprint.iacr.org/2011/338.pdf
*/

#undef DEBUG
uint8_t one[ECC_BYTES] = {1};
#ifdef DEBUG 
uint8_t three[ECC_BYTES] = {3};
uint8_t four[ECC_BYTES] = {4};
#endif


void EccPoint_double_jacobian_v3(uint8_t *X, uint8_t *Y, uint8_t *X1, uint8_t *Y1) {

    /* Rx = X, Ry = Y, tmp = Z */
    uint8_t Rx[ECC_BYTES];
    uint8_t Ry[ECC_BYTES];
    uint8_t tmp[ECC_BYTES];
    uint8_t S[ECC_BYTES];
    uint8_t M[ECC_BYTES];

    vli_modSquare_fast(S, Y1);              /* Y^2 */
    vli_modMult_fast(S, S, X1);             /* X * Y^2 */
    vli_modAdd(S, S, S, curve_p);           /* 2 * X * Y^2 */
    vli_modAdd(S, S, S, curve_p);            /* S = 4 * X * Y^2 */


    vli_modAdd(tmp, X1, one, curve_p);
    vli_modSub(M, X1, one, curve_p);
    vli_modMult_fast(M, M, tmp);
    vli_modAdd(tmp, M, M, curve_p);
    vli_modAdd(M, M, tmp, curve_p);         /* M = 3(X+1)(X-1) */

    vli_modSquare_fast(Rx, M);
    vli_modSub(Rx, Rx, S, curve_p);
    vli_modSub(Rx, Rx, S, curve_p);            /* X' = M^2 - 2S */
    vli_modSub(Ry, S, Rx, curve_p);

    vli_modMult_fast(Ry, Ry, M);
    vli_modSquare_fast(tmp, Y1);
    vli_modAdd(tmp, tmp, tmp, curve_p);
    vli_modSquare_fast(tmp, tmp);
    vli_modAdd(tmp, tmp, tmp, curve_p);

    vli_modSub(Ry, Ry, tmp, curve_p);            /* Y' = M(S-X') - 8Y^4 */
    vli_modAdd(tmp, Y1, Y1, curve_p);                        /* Z' = 2Y */

    divide_z(X, Y, Rx, Ry, tmp);

}


void EccPoint_double_jacobian(uint8_t *X, uint8_t *Y, uint8_t *X1, uint8_t *Y1)
{

    EccPoint_double_jacobian_v3(X, Y, X1, Y1);

}