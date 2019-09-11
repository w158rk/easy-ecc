#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"

/* ------ Point operations ------ */

/* Returns 1 if p_point is the point at infinity, 0 otherwise. */
int EccPoint_isZero(EccPoint *p_point)
{
    return (vli_isZero(p_point->x) && vli_isZero(p_point->y));
}


/* Modify (x1, y1, ) => (x1 / z^2, y1 / z^3) */
void divide_z(uint64_t *x, uint64_t *y, uint64_t *X1, uint64_t *Y1, uint64_t *Z)
{
    uint64_t t1[NUM_ECC_DIGITS];
    uint64_t t2[NUM_ECC_DIGITS];

    vli_modSquare_fast(t1, Z);    /* z^2 */
    vli_modMult_fast(t2, t1, Z);  /* z^3 */

    vli_modInv(t1, t1, curve_p);
    vli_modInv(t2, t2, curve_p);

    vli_modMult_fast(x, X1, t1); /* x1 * z^2 */
    vli_modMult_fast(y, Y1, t2); /* y1 * z^3 */
}


/*
    this function is to convert ordinary (x,y) tuples to Jacobian-affine coordinates (X,Y,Z) 
 */
void mult_z(uint64_t *x, uint64_t *y, uint64_t *X1, uint64_t *Y1, uint64_t *Z)
{
    uint64_t t1[NUM_ECC_DIGITS];
    uint64_t t2[NUM_ECC_DIGITS];

    vli_modSquare_fast(t1, Z);    /* z^2 */
    vli_modMult_fast(t2, t1, Z);  /* z^3 */

    vli_modMult_fast(x, X1, t1); /* x1 * z^2 */
    vli_modMult_fast(y, Y1, t2); /* y1 * z^3 */
}


void ecc_point_decompress(EccPoint *p_point, const char p_compressed[ECC_BYTES+1])
{   /* get the point from the x coordinate */
    uint64_t _3[NUM_ECC_DIGITS] = {3}; /* -a = 3 */
    ecc_bytes2native(p_point->x, p_compressed+1);
    #ifdef DEBUG 
    ERROR("decompress");
    NUM_PRINT(p_point->x);
    #endif
    
    vli_modSquare_fast(p_point->y, p_point->x); /* y = x^2 */
    #ifdef DEBUG 
    ERROR("y");
    NUM_PRINT(p_point->y);
    #endif    
    vli_modSub(p_point->y, p_point->y, _3, curve_p); /* y = x^2 - 3 */
    vli_modMult_fast(p_point->y, p_point->y, p_point->x); /* y = x^3 - 3x */
    #ifdef DEBUG 
    ERROR("y");
    NUM_PRINT(p_point->y);
    #endif    
    vli_modAdd(p_point->y, p_point->y, curve_b, curve_p); /* y = x^3 - 3x + b */
    #ifdef DEBUG 
    ERROR("y");
    NUM_PRINT(p_point->y);
    #endif
    
    mod_sqrt(p_point->y);
    #ifdef DEBUG 
    ERROR("y");
    NUM_PRINT(p_point->y);
    #endif
    
    if((p_point->y[0] & 0x01) != (p_compressed[0] & 0x01))
    {/* seems like that the first bits of coordinates are used to represent that the 
        location of the point, upsides the x-axis or downsides */
        vli_sub(p_point->y, curve_p, p_point->y);
    }
}

#undef DEBUG

int check(uint64_t *x, uint64_t *y) {
    uint64_t _3[NUM_ECC_DIGITS] = {3}; /* -a = 3 */
    uint64_t _y[NUM_ECC_DIGITS]; /* -a = 3 */
    uint64_t _y1[NUM_ECC_DIGITS]; /* -a = 3 */

    #ifdef DEBUG
    printf("x : \n");
    NUM_PRINT(x);
    #endif
    vli_modSquare_fast(_y, x); /* y = x^2 */
    #ifdef DEBUG
    printf("x^2 : \n");
    NUM_PRINT(_y);
    #endif
    vli_modSub(_y, _y, _3, curve_p); /* y = x^2 - 3 */
    #ifdef DEBUG
    printf("x^2-3 : \n");
    NUM_PRINT(_y);
    #endif
    vli_modMult_fast(_y, _y, x); /* y = x^3 - 3x */
    #ifdef DEBUG
    printf("x^3-3x : \n");
    NUM_PRINT(_y);
    printf("b : \n");
    NUM_PRINT(curve_b);
    #endif
    vli_modAdd(_y, _y, curve_b, curve_p); /* y = x^3 - 3x + b */

    #ifdef DEBUG
    printf("_y : \n");
    NUM_PRINT(_y);
    #endif

    vli_modSquare_fast(_y1, y);

    #ifdef DEBUG

    printf("y : \n");
    NUM_PRINT(_y1);
    printf("cmp : %d\n", vli_cmp(_y, _y1));

    #endif

    if( 0 == vli_cmp(_y, _y1))
        return 1;

    return 0;
    

}
