#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"

/* Point multiplication algorithm using Montgomery's ladder with co-Z coordinates.
From http://eprint.iacr.org/2011/338.pdf
*/


void EccPoint_double_jacobian(uint64_t *X, uint64_t *Y, uint64_t *X1, uint64_t *Y1)
{

    #ifdef DEBUG 
    printf("enter point doubling function \n");
    #endif 

    uint64_t Z1[NUM_ECC_DIGITS] = {1};

    /* t1 = X, t2 = Y, t3 = Z */
    uint64_t t1[NUM_ECC_DIGITS];
    uint64_t t2[NUM_ECC_DIGITS];
    uint64_t t3[NUM_ECC_DIGITS];
    uint64_t t4[NUM_ECC_DIGITS];
    uint64_t t5[NUM_ECC_DIGITS];

    vli_set(t1, X1);
    vli_set(t2, Y1);
    vli_set(t3, Z1);
    
    vli_modSquare_fast(t4, t3);             /* t4 = t3^2 */
    vli_modSub(t5, t1, t4, curve_p);        /* t5 = t1 - t4 */
    vli_modAdd(t4, t1, t4, curve_p);        /* t4 = t1 + t4 */
    vli_modMult_fast(t5, t4, t5);           /* t5 = t4 x t5 */
    vli_modAdd(t4, t5, t5, curve_p);        /* t4 = 2 x t5 */

    vli_modAdd(t4, t4, t5, curve_p);        /* t4 = 3 x t5 */
    vli_modMult_fast(t3, t3, t2);           /* t3 = t3 x t2 */
    vli_modAdd(t3, t3, t3, curve_p);        /* t3 = 2 x t3 */
    vli_modSquare_fast(t2, t2);             /* t2 = t2 ^ 2 */
    vli_modMult_fast(t5, t1, t2);           /* t5 = t1 x t2 */

    vli_modAdd(t5, t5, t5, curve_p);        /* t5 = 2 x t5 */
    vli_modAdd(t5, t5, t5, curve_p);        /* t5 = 4 x t5 */
    vli_modSquare_fast(t1, t4);             /* t1 = t4 ^ 2 */
    vli_modSub(t1, t1, t5, curve_p);        /* t1 = t1 - t5 */
    vli_modSub(t1, t1, t5, curve_p);        /* t1 = t1 - 2 x t5 */

    vli_modSquare_fast(t2, t2);             /* t2 = t2 ^ 2 */
    vli_modAdd(t2, t2, t2, curve_p);        /* t2 = 2 x t2 */
    vli_modAdd(t2, t2, t2, curve_p);        /* t2 = 4 x t2 */
    vli_modAdd(t2, t2, t2, curve_p);        /* t2 = 8 x t2 */
    vli_modSub(t5, t5, t1, curve_p);        /* t5 = t5 - t1 */

    vli_modMult_fast(t5, t4, t5);           /* t5 = t4 x t5 */
    vli_modSub(t2, t5, t2, curve_p);        /* t2 = t5 - t2 */
    apply_z(t1, t2, t3);

    vli_set(X, t1);
    vli_set(Y, t2);


    if(vli_testBit(t1, 0))
    {   
        /* t1 is an odd integer, add p to the t1, making it an even integer */
        uint64_t l_carry = vli_add(t1, t1, curve_p);
        vli_rshift1(t1);
        t1[NUM_ECC_DIGITS-1] |= l_carry << 63;      /* unify the overflow and unoverflow conditions */
    }
    else
    {
        vli_rshift1(t1);
    }
    /* t1 = 3/2*(x1^2 - z1^4) = B */
    
    vli_modSquare_fast(t3, t1);      /* t3 = B^2 */
    vli_modSub(t3, t3, t5, curve_p); /* t3 = B^2 - A */
    vli_modSub(t3, t3, t5, curve_p); /* t3 = B^2 - 2A = x3 */
    vli_modSub(t5, t5, t3, curve_p); /* t5 = A - x3 */
    vli_modMult_fast(t1, t1, t5);    /* t1 = B * (A - x3) */
    vli_modSub(t1, t1, t4, curve_p); /* t4 = B * (A - x3) - y1^4 = y3 */

    apply_z(t3, t1, t2);

    vli_set(X, t3);
    vli_set(Y, t1);

}


/* Double in place */
void EccPoint_double_jacobian_v1(uint64_t *X, uint64_t *Y, uint64_t *X1, uint64_t *Y1)
{

    #ifdef DEBUG 
    printf("enter point doubling function \n");
    #endif 

    uint64_t Z1[NUM_ECC_DIGITS] = {1};

    /* t1 = X, t2 = Y, t3 = Z */
    uint64_t t1[NUM_ECC_DIGITS];
    uint64_t t2[NUM_ECC_DIGITS];
    uint64_t t3[NUM_ECC_DIGITS];
    uint64_t t4[NUM_ECC_DIGITS];
    uint64_t t5[NUM_ECC_DIGITS];

    vli_set(t1, X1);
    vli_set(t2, Y1);
    vli_set(t3, Z1);
    
    vli_modSquare_fast(t4, t2);   /* t4 = y1^2 */
    vli_modMult_fast(t5, t1, t4); /* t5 = x1*y1^2 = A */
    vli_modSquare_fast(t4, t4);   /* t4 = y1^4 */
    vli_modMult_fast(t2, t2, t3); /* t2 = y1*z1 = z3 */
    vli_modSquare_fast(t3, t3);   /* t3 = z1^2 */
    
    vli_modAdd(t1, t1, t3, curve_p); /* t1 = x1 + z1^2 */
    vli_modAdd(t3, t3, t3, curve_p); /* t3 = 2*z1^2 */
    vli_modSub(t3, t1, t3, curve_p); /* t3 = x1 - z1^2 */
    vli_modMult_fast(t1, t1, t3);    /* t1 = x1^2 - z1^4 */
    
    vli_modAdd(t3, t1, t1, curve_p); /* t3 = 2*(x1^2 - z1^4) */
    vli_modAdd(t1, t1, t3, curve_p); /* t1 = 3*(x1^2 - z1^4) */

    if(vli_testBit(t1, 0))
    {   
        /* t1 is an odd integer, add p to the t1, making it an even integer */
        uint64_t l_carry = vli_add(t1, t1, curve_p);
        vli_rshift1(t1);
        t1[NUM_ECC_DIGITS-1] |= l_carry << 63;      /* unify the overflow and unoverflow conditions */
    }
    else
    {
        vli_rshift1(t1);
    }
    /* t1 = 3/2*(x1^2 - z1^4) = B */
    
    vli_modSquare_fast(t3, t1);      /* t3 = B^2 */
    vli_modSub(t3, t3, t5, curve_p); /* t3 = B^2 - A */
    vli_modSub(t3, t3, t5, curve_p); /* t3 = B^2 - 2A = x3 */
    vli_modSub(t5, t5, t3, curve_p); /* t5 = A - x3 */
    vli_modMult_fast(t1, t1, t5);    /* t1 = B * (A - x3) */
    vli_modSub(t1, t1, t4, curve_p); /* t4 = B * (A - x3) - y1^4 = y3 */

    apply_z(t3, t1, t2);

    vli_set(X, t3);
    vli_set(Y, t1);

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
