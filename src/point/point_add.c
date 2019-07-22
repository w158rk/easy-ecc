#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"


void EccPoint_add_jacobian(uint64_t *X3, uint64_t *Y3, uint64_t *Z3, uint64_t *X1, 
                            uint64_t *Y1, uint64_t *Z1, uint64_t *x2, uint64_t *y2)
{

    uint64_t t1[NUM_ECC_DIGITS];
    uint64_t t2[NUM_ECC_DIGITS];
    uint64_t t3[NUM_ECC_DIGITS];
    uint64_t t4[NUM_ECC_DIGITS];

    if(vli_isZero(Z1))
    {
        return;
    }

    // check the infinity

    vli_modSquare_fast(t1, Z1);
    vli_modMult_fast(t2, t1, Z1);
    vli_modMult_fast(t1, t1, x2);
    vli_modMult_fast(t2, t2, y2);

    vli_modSub(t1, t1, X1, curve_p);
    vli_modSub(t2, t2, Y1, curve_p);
    if (vli_isZero(t1)) {

        if(vli_isZero(t2)) {
            uint64_t z[NUM_ECC_DIGITS] = {0};
            z[0] = 1;
            EccPoint_double_jacobian(x2, y2, z);
            vli_set(X3, x2);
            vli_set(Y3, y2);
            vli_set(Z3, z);

        } else {

            // return infinity

        }

    }
    vli_modMult_fast(Z3, Z1, t1);

    vli_modSquare_fast(t3, t1);
    vli_modMult_fast(t4, t3, t1);
    vli_modMult_fast(t3, t3, X1);
    vli_modAdd(t1, t3, t3, curve_p);

    vli_modSquare_fast(X3, t2);
    vli_modSub(X3, X3, t1, curve_p);
    vli_modSub(X3, X3, t4, curve_p);
    vli_modSub(t3, t3, X3, curve_p);

    vli_modMult_fast(t3, t3, t2);
    vli_modMult_fast(t4, t4, Y1);
    vli_modSub(Y3, t3, t4, curve_p);

}



/* Input P = (x1, y1, Z), Q = (x2, y2, Z)
   Output P' = (x1', y1', Z3), P + Q = (x3, y3, Z3)
   or P => P', Q => P + Q

    (X,Y)-onlly co-Z addition with update - XYCZ-ADD
*/
void XYcZ_add(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2)
{
    /* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
    uint64_t *t1 = X1, *t2 = Y1, *t3 = X2, *t4 = Y2;
    uint64_t t5[NUM_ECC_DIGITS];
    
    vli_modSub(t5, t3, t1, curve_p); /* t5 = x2 - x1 */
    vli_modSquare_fast(t5, t5);      /* t5 = (x2 - x1)^2 = A */
    vli_modMult_fast(t1, t1, t5);    /* t1 = x1*A = B */
    vli_modMult_fast(t3, t3, t5);    /* t3 = x2*A = C */
    vli_modSub(t4, t4, t2, curve_p); /* t4 = y2 - y1 */
    vli_modSquare_fast(t5, t4);      /* t5 = (y2 - y1)^2 = D */
    
    vli_modSub(t5, t5, t1, curve_p); /* t5 = D - B */
    vli_modSub(t5, t5, t3, curve_p); /* t5 = D - B - C = x3 */
    vli_modSub(t3, t3, t1, curve_p); /* t3 = C - B */
    vli_modMult_fast(t2, t2, t3);    /* t2 = y1*(C - B) */
    vli_modSub(t3, t1, t5, curve_p); /* t3 = B - x3 */
    vli_modMult_fast(t4, t4, t3);    /* t4 = (y2 - y1)*(B - x3) */
    vli_modSub(t4, t4, t2, curve_p); /* t4 = y3 */
    
    vli_set(X2, t5);
}

/* Input P = (x1, y1, Z), Q = (x2, y2, Z)
   Output P + Q = (x3, y3, Z3), P - Q = (x3', y3', Z3)
   or P => P - Q, Q => P + Q

   (X,Y)-only co-A conjugate addition - XYCZ-ADDC
*/
void XYcZ_addC(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2)
{
    /* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
    uint64_t t5[NUM_ECC_DIGITS];
    uint64_t t6[NUM_ECC_DIGITS];
    uint64_t t7[NUM_ECC_DIGITS];
    uint64_t *t1, *t2, *t3, *t4;
    t1 = X1;
    t2 = Y1;
    t3 = X2;
    t4 = Y2;
    
    vli_modSub(t5, t3, t1, curve_p); /* t5 = x2 - x1 */
    vli_modSquare_fast(t5, t5);      /* t5 = (x2 - x1)^2 = A */
    vli_modMult_fast(t1, t1, t5);    /* t1 = x1*A = B */
    vli_modMult_fast(t3, t3, t5);    /* t3 = x2*A = C */
    vli_modAdd(t5, t4, t2, curve_p); /* t4 = y2 + y1 */
    vli_modSub(t4, t4, t2, curve_p); /* t4 = y2 - y1 */

    vli_modSub(t6, t3, t1, curve_p); /* t6 = C - B */
    vli_modMult_fast(t2, t2, t6);    /* t2 = y1 * (C - B) */
    vli_modAdd(t6, t3, t1, curve_p); /* t6 = B + C */
    vli_modSquare_fast(t3, t4);      /* t3 = (y2 - y1)^2 */
    vli_modSub(t3, t3, t6, curve_p); /* t3 = x3 */
    
    vli_modSub(t7, t1, t3, curve_p); /* t7 = B - x3 */
    vli_modMult_fast(t4, t4, t7);    /* t4 = (y2 - y1)*(B - x3) */
    vli_modSub(t4, t4, t2, curve_p); /* t4 = y3 */
    
    vli_modSquare_fast(t7, t5);      /* t7 = (y2 + y1)^2 = F */
    vli_modSub(t7, t7, t6, curve_p); /* t7 = x3' */
    vli_modSub(t6, t7, t1, curve_p); /* t6 = x3' - B */
    vli_modMult_fast(t6, t6, t5);    /* t6 = (y2 + y1)*(x3' - B) */
    vli_modSub(Y1, t6, t2, curve_p); /* t2 = y3' */
    
    // vli_set(X2, t3);
    // vli_set(Y2, t4);
    vli_set(X1, t7);
    vli_set(Y1, t6);
}


