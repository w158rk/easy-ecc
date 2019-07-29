#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif


void EccPoint_add_jacobian(uint64_t *x3, uint64_t *y3, uint64_t *x1, uint64_t *y1, uint64_t *x2, uint64_t *y2)
{

    if(vli_isZero(x1) && vli_isZero(y1)) 
    {
        vli_set(x3, x2);
        vli_set(y3, y2);
        return ;
    }

    if(vli_isZero(x2) && vli_isZero(y2)) 
    {
        vli_set(x3, x1);
        vli_set(y3, y1);
        return ;
    }

    uint64_t z1[NUM_ECC_DIGITS] = {1};

    uint64_t t1[NUM_ECC_DIGITS];
    uint64_t t2[NUM_ECC_DIGITS];
    uint64_t t3[NUM_ECC_DIGITS];
    uint64_t t4[NUM_ECC_DIGITS];
    uint64_t t5[NUM_ECC_DIGITS];
    uint64_t t7[NUM_ECC_DIGITS];

    vli_set(t1, x1);
    vli_set(t2, y1);
    vli_set(t3, z1);
    vli_set(t4, x2);
    vli_set(t5, y2);


    vli_modSquare_fast(t7, t3);
    vli_modMult_fast(t4, t4, t7);
    vli_modMult_fast(t7, t3, t7);
    vli_modMult_fast(t5, t5, t7);
    vli_modSub(t4, t1, t4, curve_p);
    vli_modSub(t5, t2, t5, curve_p);

    if(vli_isZero(t4)) {

        if(vli_isZero(t5)) {

            EccPoint_double_jacobian(x3, y3, x1, y1);
            return ;

        }

        else {

            vli_clear(x3);
            vli_clear(y3);
            return ;

        }
    }

    vli_modAdd(t1, t1, t1, curve_p);
    vli_modSub(t1, t1, t4, curve_p);
    vli_modAdd(t2, t2, t2, curve_p);
    vli_modSub(t2, t2, t5, curve_p);
    vli_modMult_fast(t3, t3, t4);

    vli_modSquare_fast(t7, t4);
    vli_modMult_fast(t4, t4, t7);
    vli_modMult_fast(t7, t1, t7);
    vli_modSquare_fast(t1, t5);
    vli_modSub(t1, t1, t7, curve_p);

    vli_modSub(t7, t7, t1, curve_p);
    vli_modSub(t7, t7, t1, curve_p);
    vli_modMult_fast(t5, t5, t7);
    vli_modMult_fast(t4, t2, t4);
    vli_modSub(t2, t5, t4, curve_p);

    if(vli_testBit(t2, 0))
    {   
        /* t1 is an odd integer, add p to the t1, making it an even integer */
        uint64_t l_carry = vli_add(t2, t2, curve_p);
        vli_rshift1(t2);
        t2[NUM_ECC_DIGITS-1] |= l_carry << 63;      /* unify the overflow and unoverflow conditions */
    }
    else
    {
        vli_rshift1(t2);
    }

    divide_z(x3, y3, t1, t2, t3);

}

/**
 * @brief (X,Y)-only co-Z addtion with update, the result cannot be used as a point.
 *          It can only be used in the multiplication function
 * @param[in] X1 P.x
 * @param[in] Y1 P.y
 * @param[in] X2 Q.x
 * @param[in] Y2 Q.y
 * @param[out] X1 P.x
 * @param[out] Y1 P.y
 * @param[out] X2 (P+Q).x
 * @param[out] Y2 (P+Q).y
 */
void XYcZ_add(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2)
{

    #ifdef DEBUG 
    fprintf(stderr, "in xycz_add");

    NUM_PRINT(X1);
    NUM_PRINT(Y1);
    NUM_PRINT(X2);
    NUM_PRINT(Y2);
    #endif
    

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

/**
 * @brief (X,Y)-only co-Z conjugate addition 
 * @param[in] X1 P.x
 * @param[in] Y1 P.y
 * @param[in] X2 Q.x 
 * @param[in] Y2 Q.y 
 * @param[out] X1 (P-Q).x
 * @param[out] Y1 (P-Q).y
 * @param[out] X2 (P+Q).x
 * @param[out] Y2 (P+Q).y
 * */
void XYcZ_addC(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2)
{

    #ifdef DEBUG 
    fprintf(stderr, "enter xycz_addc");

    NUM_PRINT(X1);
    NUM_PRINT(Y1);
    NUM_PRINT(X2);
    NUM_PRINT(Y2);
    #endif

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
    vli_modSub(t6, t6, t2, curve_p); /* t2 = y3' */
    
    vli_set(X1, t7);
    vli_set(Y1, t6);
    vli_set(X2, t3);
    vli_set(Y2, t4);

    #ifdef DEBUG 
    fprintf(stderr, "exit xycz_addc");

    NUM_PRINT(X1);
    NUM_PRINT(Y1);
    NUM_PRINT(X2);
    NUM_PRINT(Y2);
    #endif

}


