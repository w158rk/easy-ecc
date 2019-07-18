#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"

/* Point multiplication algorithm using Montgomery's ladder with co-Z coordinates.
From http://eprint.iacr.org/2011/338.pdf
*/

/* Double in place */
void EccPoint_double_jacobian(uint64_t *X1, uint64_t *Y1, uint64_t *Z1)
{

    uint64_t t1[NUM_ECC_DIGITS];
    uint64_t t2[NUM_ECC_DIGITS];
    uint64_t t3[NUM_ECC_DIGITS];
    uint64_t X3[NUM_ECC_DIGITS];
    uint64_t Y3[NUM_ECC_DIGITS];
    uint64_t Z3[NUM_ECC_DIGITS];
    uint64_t three[NUM_ECC_DIGITS] = {0};
    three[0] = 3;

    if(vli_isZero(Z1))
    {
        return;
    }

    // check the infinity

    vli_modSquare_fast(t1, Z1);
    vli_modSub(t2, X1, t1, curve_p);
    vli_modAdd(t1, X1, t1, curve_p);
    vli_modMult_fast(t2, t2, t1);

    vli_modMult_fast(t2, t2, three);
    vli_modAdd(Y3, Y1, Y1, curve_p);
    vli_modMult_fast(Z3, Y3, Z1);
    vli_modSquare_fast(Y3, Y3);

    vli_modMult_fast(t3, Y3, X1);
    vli_modSquare_fast(Y3, Y3);
    vli_rshift1(Y3);
    vli_modSquare_fast(X3, t2);

    vli_modAdd(t1, t3, t3, curve_p);
    vli_modSub(X3, X3, t1, curve_p);
    vli_modSub(t1, t3, X3, curve_p);
    vli_modMult_fast(t1, t1, t2);

    vli_modSub(Y3, t1, Y3, curve_p);

    vli_set(X1, X3);
    vli_set(Y1, Y3);
    vli_set(Z1, Z3);
}

void EccPoint_double_jacobian_origin(uint64_t *X1, uint64_t *Y1, uint64_t *Z1)
{
    /* t1 = X, t2 = Y, t3 = Z */
    uint64_t t4[NUM_ECC_DIGITS];
    uint64_t t5[NUM_ECC_DIGITS];
    
    if(vli_isZero(Z1))
    {
        return;
    }
    
    vli_modSquare_fast(t4, Y1);   /* t4 = y1^2 */
    vli_modMult_fast(t5, X1, t4); /* t5 = x1*y1^2 = A */
    vli_modSquare_fast(t4, t4);   /* t4 = y1^4 */
    vli_modMult_fast(Y1, Y1, Z1); /* t2 = y1*z1 = z3 */
    vli_modSquare_fast(Z1, Z1);   /* t3 = z1^2 */
    
    vli_modAdd(X1, X1, Z1, curve_p); /* t1 = x1 + z1^2 */
    vli_modAdd(Z1, Z1, Z1, curve_p); /* t3 = 2*z1^2 */
    vli_modSub(Z1, X1, Z1, curve_p); /* t3 = x1 - z1^2 */
    vli_modMult_fast(X1, X1, Z1);    /* t1 = x1^2 - z1^4 */
    
    vli_modAdd(Z1, X1, X1, curve_p); /* t3 = 2*(x1^2 - z1^4) */
    vli_modAdd(X1, X1, Z1, curve_p); /* t1 = 3*(x1^2 - z1^4) */
    if(vli_testBit(X1, 0))
    {   /* t1 is an odd integer, add p to the t1, making it an even integer */
        uint64_t l_carry = vli_add(X1, X1, curve_p);
        vli_rshift1(X1);
        X1[NUM_ECC_DIGITS-1] |= l_carry << 63;      /* unify the overflow and unoverflow conditions */
    }
    else
    {
        vli_rshift1(X1);
    }
    /* t1 = 3/2*(x1^2 - z1^4) = B */
    
    vli_modSquare_fast(Z1, X1);      /* t3 = B^2 */
    vli_modSub(Z1, Z1, t5, curve_p); /* t3 = B^2 - A */
    vli_modSub(Z1, Z1, t5, curve_p); /* t3 = B^2 - 2A = x3 */
    vli_modSub(t5, t5, Z1, curve_p); /* t5 = A - x3 */
    vli_modMult_fast(X1, X1, t5);    /* t1 = B * (A - x3) */
    vli_modSub(t4, X1, t4, curve_p); /* t4 = B * (A - x3) - y1^4 = y3 */
    
    vli_set(X1, Z1);
    vli_set(Z1, Y1);
    vli_set(Y1, t4);
}
