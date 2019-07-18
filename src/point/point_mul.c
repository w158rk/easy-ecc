#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <curves.h>
#include <field.h>
#include <point.h>

/* ------ Point operations ------ */

/* P = (x1, y1) => 2P, (x2, y2) => P' */
/* 
    Result : 
        2P = (X1, Y1, Z)
        P = (X2, Y2, Z)
 */
void XYcZ_initial_double(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2, uint64_t *p_initialZ)
{
    // uint64_t z[NUM_ECC_DIGITS];
    
    // vli_set(X2, X1);
    // vli_set(Y2, Y1);
    
    // vli_clear(z);
    // z[0] = 1;
    // if(p_initialZ)
    // {
    //     vli_set(z, p_initialZ);
    // }

    // apply_z(X1, Y1, z);
    
    // EccPoint_double_jacobian(X1, Y1, z);
    
    // apply_z(X2, Y2, z);

    uint64_t *t1 = X1, *t2 = X2; 
    uint64_t t3[NUM_ECC_DIGITS], t4[NUM_ECC_DIGITS];
    uint64_t t5[NUM_ECC_DIGITS], t6[NUM_ECC_DIGITS];

    vli_modSquare_fast(t3, t1);
    vli_modAdd(t4, t3, t3, curve_p);
    vli_modAdd(t3, t3, t4, curve_p);
    vli_modAdd(t3, t3, curve_a, curve_p); 

    vli_modSquare_fast(t4, t2);
    vli_modAdd(t4, t4, t4, curve_p);
    vli_modAdd(t5, t4, t4, curve_p);
    vli_modMult_fast(t5, t5, t1);

    vli_modSquare_fast(t6, t3);
    vli_modSub(t6, t6, t5, curve_p);
    vli_modSub(t6, t6, t5, curve_p);
    vli_modSub(t1, t5, t6, curve_p);

    vli_modMult_fast(t1, t1, t3);
    vli_modSquare_fast(t3, t4);
    vli_modAdd(t3, t3, t3, curve_p);
    vli_modSub(t1, t1, t3, curve_p);

    vli_set(X2, t6);
    vli_set(Y2, t1);
    vli_set(X1, t5);
    vli_set(Y1, t3);

}

/*
    Algorithm 9 : Montgomery ladder with (X,Y)-only co-Z addition
 */
void EccPoint_mult_plain(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar, uint64_t *p_initialZ)
{
    /* R0 and R1 */
    /* R0 = (Rx[0], Ry[0]) */
    /* R1 = (Rx[1], Ry[1]) */
    uint64_t Rx[2][NUM_ECC_DIGITS];
    uint64_t Ry[2][NUM_ECC_DIGITS];
    uint64_t z[NUM_ECC_DIGITS];
    
    int i, nb;
    
    vli_set(Rx[1], p_point->x);
    vli_set(Ry[1], p_point->y);

    XYcZ_initial_double(Rx[1], Ry[1], Rx[0], Ry[0], p_initialZ);        /* R[1] = 2P and R[0] = P */

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
    
    apply_z(Rx[0], Ry[0], z);
    
    vli_set(p_result->x, Rx[0]);
    vli_set(p_result->y, Ry[0]);
}

void EccPoint_mult(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar /*, uint64_t *p_initialZ */)
{
    /* R0 and R1 */
    /* R0 = (Rx[0], Ry[0]) */
    /* R1 = (Rx[1], Ry[1]) */

    int len = vli_numBits(p_scalar); /* get the length of the scalar */

    uint64_t Rx[NUM_ECC_DIGITS];
    uint64_t Ry[NUM_ECC_DIGITS];
    uint64_t Px[NUM_ECC_DIGITS];
    uint64_t Py[NUM_ECC_DIGITS];
    uint64_t Tx[NUM_ECC_DIGITS];
    uint64_t Ty[NUM_ECC_DIGITS];
    uint64_t Tz[NUM_ECC_DIGITS];

    /* initialize the result as P */
    vli_set(Rx, p_point->x);
    vli_set(Ry, p_point->y);


    int i;
    for(i=len-2; i>=0; --i)
    {

        uint64_t Rz[NUM_ECC_DIGITS] = {1};
        vli_set(Tx, Rx);
        vli_set(Ty, Ry);
        vli_set(Tz, Rz);
        vli_set(Px, Rx);
        vli_set(Py, Ry);
        
        EccPoint_add_jacobian(Rx, Ry, Rz, Tx, Ty, Tz, Px, Py);
        // EccPoint_double_jacobian(Rx, Ry, Rz);
        // apply_z(Rx, Ry, Rz);


        if(0 != vli_testBit(p_scalar, i)) /* this bit is not zero */
        {

            // XYcZ_add(Rx[0], Ry[0], Rx[1], Ry[1]);
            uint64_t Rz[NUM_ECC_DIGITS] = {1};
            vli_set(Tx, Rx);
            vli_set(Ty, Ry);
            vli_set(Tz, Rz);
            
            EccPoint_add_jacobian(Rx, Ry, Rz, Tx, Ty, Tz, p_point->x, p_point->y);
            apply_z(Rx, Ry, Rz);

        }

    }

    vli_set(p_result->x, Rx);
    vli_set(p_result->y, Ry);
}