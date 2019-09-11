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
void XYcZ_initial_double(uint32_t *X1, uint32_t *Y1, uint32_t *X2, uint32_t *Y2)
{

    uint32_t one[NUM_ECC_DIGITS] = {1};
    uint32_t Rx[NUM_ECC_DIGITS];
    uint32_t Ry[NUM_ECC_DIGITS];
    uint32_t tmp[NUM_ECC_DIGITS];
    uint32_t S[NUM_ECC_DIGITS];
    uint32_t M[NUM_ECC_DIGITS];

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
void EccPoint_mult_ladder_advance(EccPoint *p_result, EccPoint *p_point, uint32_t *p_scalar)
{
    if(vli_isZero(p_scalar)) {
        vli_clear(p_result->x);
        vli_clear(p_result->y);
        return ;
    }

    uint16_t len = vli_numBits(p_scalar);
    if(len==1) {
        /* scalar = 1 */
        vli_set(p_result->x, p_point->x);
        vli_set(p_result->y, p_point->y);
        return ;
    }

    uint32_t Rx[2][NUM_ECC_DIGITS];
    uint32_t Ry[2][NUM_ECC_DIGITS];
    uint32_t z[NUM_ECC_DIGITS];
    
    vli_set(Rx[0], p_point->x);
    vli_set(Ry[0], p_point->y);

    XYcZ_initial_double(Rx[0], Ry[0], Rx[1], Ry[1]);        /* R[1] = 2P and R[0] = P */

    int num_digit = (len-1) / 32;
    uint32_t digit = 0;
    int pos = 0;
    int i = len-2;
    int nb = 0;


    // for(i = vli_numBits(p_scalar) - 2; i > 0; --i)
    // {
    //     nb = !vli_testBit(p_scalar, i);
    //     XYcZ_addC(Rx[1-nb], Ry[1-nb], Rx[nb], Ry[nb]);
    //     XYcZ_add(Rx[nb], Ry[nb], Rx[1-nb], Ry[1-nb]);
    // }
    while (num_digit >= 0) {
        digit = p_scalar[num_digit];
        pos = i % 32;
        #ifdef DEBUG 
        printf("[before] i : %d, pos : %d, num_digit : %d\n", i, pos, num_digit);
        #endif
        while (pos >= 0 && i > 0) {
            #ifdef DEBUG 
            printf("[mul-nonfix] i : %d, pos : %d, num_digit : %d\n", i, pos, num_digit);
            #endif
            nb = !((digit >> pos) & 0x01);
            XYcZ_addC(Rx[1-nb], Ry[1-nb], Rx[nb], Ry[nb]);
            XYcZ_add(Rx[nb], Ry[nb], Rx[1-nb], Ry[1-nb]);

            -- i;
            -- pos;
        }
        -- num_digit;
    }

    nb = !(p_scalar[0] & 0x01);
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

void mult_new(EccPoint *p_result, EccPoint *p_point, uint32_t *p_scalar) 
{
    if(vli_isZero(p_scalar)) {
        vli_clear(p_result->x);
        vli_clear(p_result->y);
        return ;
    }

    uint16_t len = vli_numBits(p_scalar);
    // if(len==1) {
    //     /* scalar = 1 */
    //     vli_set(p_result->x, p_point->x);
    //     vli_set(p_result->y, p_point->y);
    //     return ;
    // }

    uint32_t x[2][NUM_ECC_DIGITS];
    uint32_t y[2][NUM_ECC_DIGITS];
    uint32_t z[2][NUM_ECC_DIGITS] = {{1}, {1}};
    vli_set(x[0], p_point->x);
    vli_set(y[0], p_point->y);
    uint8_t cur = 0, ncur = 1;

    int num_digit = (len-1) / 32;
    uint32_t digit = 0;
    int pos = 0;
    int i = len-2;
    int b = 0;

    while (i >= 0) {
        digit = p_scalar[num_digit];
        pos = i % 32;
        b = ((digit >> pos) & 0x01);
        
        EccPoint_double_jacobian(x[ncur], y[ncur], z[ncur], x[cur], y[cur], z[cur]);

        if(b){
            EccPoint_add_jacobian(x[ncur], y[ncur], z[ncur], p_point->x, p_point->y);
        }

        ncur = cur;
        cur = !cur;
        num_digit = pos==0 ? (num_digit-1) : num_digit;
        -- i;
    }

    divide_z(p_result->x, p_result->y, x[cur], y[cur], z[cur]);


}

void EccPoint_mult_P(EccPoint *p_result, uint32_t *p_scalar) 
{
    if(vli_isZero(p_scalar)) {
        vli_clear(p_result->x);
        vli_clear(p_result->y);
        return ;
    }
    uint32_t x[2][NUM_ECC_DIGITS];
    uint32_t y[2][NUM_ECC_DIGITS];
    uint32_t z[2][NUM_ECC_DIGITS] = {{1}, {1}};
    uint8_t cur = 0, ncur = 1;
    int len = vli_numBits(p_scalar);

    // intialize 
    int i = len % 32;               // how many bits in the first block
    int pos = len % 4;              // how many should be considered
    int num_digit = (len-1) / 32;
    #ifdef DEBUG 
    fprintf(stderr, "i : %d, pos : %d, num_digit : %d\n", i, pos, num_digit);
    #endif
    int b = p_scalar[num_digit] >> (i-pos);
    if (i==0) b = 0;

    vli_set(x[0], curve_nG[b].x);
    vli_set(y[0], curve_nG[b].y);

    uint32_t digit = 0;
    #ifdef DEBUG 
    fprintf(stderr, "i : %d, pos : %d, num_digit : %d\ncur : %d, ncur : %d\n", i, pos, num_digit, cur, ncur);
    #endif

    i = (len/4) * 4 - 4;
    pos = 0;
    if (i<0) {
        goto end;
    }

    while (num_digit >= 0) {
        digit = p_scalar[num_digit];
        pos = i % 32;
        while (pos >= 0) {

            #ifdef DEBUG 
            fprintf(stderr, "i : %d, pos : %d, num_digit : %d\ncur : %d, ncur : %d\n", i, pos, num_digit, cur, ncur);
            #endif
            b = ((digit >> pos) & 0x0f);
            
            EccPoint_double_jacobian(x[ncur], y[ncur], z[ncur], x[cur], y[cur], z[cur]);
            EccPoint_double_jacobian(x[cur], y[cur], z[cur], x[ncur], y[ncur], z[ncur]);
            EccPoint_double_jacobian(x[ncur], y[ncur], z[ncur], x[cur], y[cur], z[cur]);
            EccPoint_double_jacobian(x[cur], y[cur], z[cur], x[ncur], y[ncur], z[ncur]);
            EccPoint_add_jacobian(x[cur], y[cur], z[cur], curve_nG[b].x, curve_nG[b].y);

            i -= 4;
            pos -= 4;

        }
        num_digit --;
    }

end:
    #ifdef DEBUG 
    printf("mark\n");
    #endif
    divide_z(p_result->x, p_result->y, x[cur], y[cur], z[cur]);

}


void EccPoint_mult(EccPoint *p_result, EccPoint *p_point, uint32_t *p_scalar) {
    // EccPoint_mult_ladder(p_result, p_point, p_scalar);
    EccPoint_mult_ladder_advance(p_result, p_point, p_scalar);
    // mult_new(p_result, p_point, p_scalar);

}
