#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <curves.h>
#include <field.h>
#include <point.h>


/**
 * @brief initial doubling 
 * @param[in] X1 P.x 
 * @param[in] Y1 P.y 
 * @param[out] X1 P.x
 * @param[out] Y1 P.y
 * @param[out] X2 2P.x 
 * @param[out] Y2 2P.x
 */
void XYcZ_initial_double(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2)
{

    uint64_t one[NUM_ECC_DIGITS] = {1};
    uint64_t Rx[NUM_ECC_DIGITS];
    uint64_t Ry[NUM_ECC_DIGITS];
    uint64_t tmp[NUM_ECC_DIGITS];
    uint64_t S[NUM_ECC_DIGITS];
    uint64_t M[NUM_ECC_DIGITS];

    vli_modAdd(tmp, X1, one, curve_p);
    vli_modSub(M, X1, one, curve_p);
    vli_modMult_fast(M, M, tmp);
    vli_modAdd(tmp, M, M, curve_p);
    vli_modAdd(M, M, tmp, curve_p);         /* M = 3(X+1)(X-1) */

    vli_modMult_fast(S, X1, Y1);             /* X * Y */
    vli_modAdd(S, S, S, curve_p);           /* 2 * X * Y */
    vli_modAdd(S, S, S, curve_p);            /* S = 4 * X * Y */
    vli_modMult_fast(S, S, Y1);             /* S = 4 * X * Y^2 */
    vli_set(X1, S);                         /* X' = 4XY^2 */

    vli_modSquare_fast(Rx, M);
    vli_modSub(Rx, Rx, S, curve_p);
    vli_modSub(Rx, Rx, S, curve_p);            /* X' = M^2 - 2S */

    vli_modSub(Ry, S, Rx, curve_p);
    vli_modMult_fast(Ry, Ry, M);
    vli_modSquare_fast(tmp, Y1);
    vli_modAdd(tmp, tmp, tmp, curve_p);         /* 2Y^2 */
    vli_modSquare_fast(tmp, tmp);
    vli_modAdd(tmp, tmp, tmp, curve_p);
    vli_set(Y1, tmp);                           /* Y1 = 8Y^2 */
    vli_modSub(Ry, Ry, tmp, curve_p);            /* Y' = M(S-X') - 8Y^4 */

    vli_set(X2, Rx);
    vli_set(Y2, Ry);


}

/**
 * @brief Montgomery ladder with (X,Y)-only co-Z addition
 * @param[out] p_result the product 
 * @param[in] p_point the point 
 * @param[in] p_scalar the scalar 
 */
void EccPoint_mult_ladder_advance(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar)
{

    uint64_t Rx[2][NUM_ECC_DIGITS];
    uint64_t Ry[2][NUM_ECC_DIGITS];
    uint64_t z[NUM_ECC_DIGITS];
    
    int i, nb;
    
    vli_set(Rx[0], p_point->x);
    vli_set(Ry[0], p_point->y);

    XYcZ_initial_double(Rx[0], Ry[0], Rx[1], Ry[1]);        /* R[1] = 2P and R[0] = P */

    #ifdef DEBUG 

    fprintf(stderr, "%d\n", check(Rx[0], Ry[0]));
    fprintf(stderr, "R[0]");
    NUM_PRINT(Rx[0]);
    NUM_PRINT(Ry[0]);
    fprintf(stderr, "%d\n", check(Rx[1], Ry[1]));
    fprintf(stderr, "R[1]");
    NUM_PRINT(Rx[1]);
    NUM_PRINT(Ry[1]);

    #endif 

    for(i = vli_numBits(p_scalar) - 2; i > 0; --i)
    {
        nb = !vli_testBit(p_scalar, i);
        XYcZ_addC(Rx[1-nb], Ry[1-nb], Rx[nb], Ry[nb]);
        XYcZ_add(Rx[nb], Ry[nb], Rx[1-nb], Ry[1-nb]);
    }

    nb = !vli_testBit(p_scalar, 0);
    XYcZ_addC(Rx[1-nb], Ry[1-nb], Rx[nb], Ry[nb]);
    
    /* Find final 1/Z value. */
    vli_modSub(z, Rx[1], Rx[0], curve_p); /* X1 - X0 */
    vli_modMult_fast(z, z, Ry[1-nb]);     /* Yb * (X1 - X0) */
    vli_modMult_fast(z, z, p_point->x);   /* xP * Yb * (X1 - X0) */
    vli_modInv(z, z, curve_p);            /* 1 / (xP * Yb * (X1 - X0)) */
    vli_modMult_fast(z, z, p_point->y);   /* yP / (xP * Yb * (X1 - X0)) */
    vli_modMult_fast(z, z, Rx[1-nb]);     /* Xb * yP / (xP * Yb * (X1 - X0)) */
    /* End 1/Z calculation */

    XYcZ_add(Rx[nb], Ry[nb], Rx[1-nb], Ry[1-nb]);

    mult_z(p_result->x, p_result->y, Rx[0], Ry[0], z);

    
}


/**
 * @brief simple implementation of mult_plain, using double-and-add method 
 * @param[out] p_result the product 
 * @param[in] p_point the point 
 * @param[in] p_scalar the scalar 
 */
void EccPoint_mult_plain(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar)
{
    /* R0 and R1 */
    /* R0 = (Rx[0], Ry[0]) */
    /* R1 = (Rx[1], Ry[1]) */

    int len = vli_numBits(p_scalar); /* get the length of the scalar */

    uint64_t Rx[NUM_ECC_DIGITS];
    uint64_t Ry[NUM_ECC_DIGITS];

    /* initialize the result as P */
    vli_set(Rx, p_point->x);
    vli_set(Ry, p_point->y);

    int i;
    for(i=len-2; i>=0; --i)
    {

        EccPoint_double_jacobian(Rx, Ry, Rx, Ry);

        if(0 != vli_testBit(p_scalar, i)) /* this bit is not zero */
        {

            EccPoint_add_jacobian(Rx, Ry, Rx, Ry, p_point->x, p_point->y);

        }

    }

    vli_set(p_result->x, Rx);
    vli_set(p_result->y, Ry);
}

/**
 * @brief Montgomery ladder, using double-and-add method 
 * @param[out] p_result the product 
 * @param[in] p_point the point 
 * @param[in] p_scalar the scalar 
 */
void EccPoint_mult_ladder(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar)
{

    int len = vli_numBits(p_scalar); /* get the length of the scalar */

    uint64_t Rx[2][NUM_ECC_DIGITS];
    uint64_t Ry[2][NUM_ECC_DIGITS];

    /* init */
    vli_clear(Rx[0]);
    vli_clear(Ry[0]);
    vli_set(Rx[1], p_point->x);
    vli_set(Ry[1], p_point->y);

    /* process */
    int i, nb, b;
    for(i=len-1; i>=0; --i)
    {

        nb = !vli_testBit(p_scalar, i);
        b = 1 - nb;

        #ifdef DEBUG 
        printf("add R[%d] with R[%d]\n", nb, b);
        #endif
        EccPoint_add_jacobian(Rx[nb], Ry[nb], Rx[nb], Ry[nb], Rx[b], Ry[b]);

        #ifdef DEBUG 
        printf("double R[%d]\n", b);
        #endif
        EccPoint_double_jacobian(Rx[b], Ry[b], Rx[b], Ry[b]);

    }

    vli_set(p_result->x, Rx[0]);
    vli_set(p_result->y, Ry[0]);

}


void EccPoint_mult(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar) {
    // EccPoint_mult_plain(p_result, p_point, p_scalar);
    // EccPoint_mult_ladder(p_result, p_point, p_scalar);
    EccPoint_mult_ladder_advance(p_result, p_point, p_scalar);
}
