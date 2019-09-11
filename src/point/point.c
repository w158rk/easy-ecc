#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "curves.h"
#include "field.h"
#include "point.h"

// #undef DEBUG
/* ------ Point operations ------ */

/* Returns 1 if p_point is the point at infinity, 0 otherwise. */
uint8_t EccPoint_isZero(EccPoint *p_point)
{
    return (vli_isZero(p_point->x) && vli_isZero(p_point->y));
}


/* Modify (x1, y1, ) => (x1 / z^2, y1 / z^3) */
void divide_z(uint8_t *x, uint8_t *y, uint8_t *X1, uint8_t *Y1, uint8_t *Z)
{
    uint8_t t1[ECC_BYTES];
    uint8_t t2[ECC_BYTES];

    vli_modSquare_fast(t1, Z);    /* z^2 */
    vli_modMult_fast(t2, t1, Z);  /* z^3 */
    #ifdef DEBUG 
    printf("Z^2 : \n");
    NUM_PRINT(t1);
    printf("Z^3 : \n");
    NUM_PRINT(t2);
    #endif

    vli_modInv(t1, t1, curve_p);
    vli_modInv(t2, t2, curve_p);

    #ifdef DEBUG 
    printf("1 / Z^2 : \n");
    NUM_PRINT(t1);
    printf("1 / Z^3 : \n");
    NUM_PRINT(t2);
    #endif

    vli_modMult_fast(x, X1, t1); /* x1 * z^2 */
    vli_modMult_fast(y, Y1, t2); /* y1 * z^3 */
}


/*
    this function is to convert ordinary (x,y) tuples to Jacobian-affine coordinates (X,Y,Z) 
 */
void mult_z(uint8_t *x, uint8_t *y, uint8_t *X1, uint8_t *Y1, uint8_t *Z)
{
    uint8_t t1[ECC_BYTES];
    uint8_t t2[ECC_BYTES];

    vli_modSquare_fast(t1, Z);    /* z^2 */
    vli_modMult_fast(t2, t1, Z);  /* z^3 */
    #ifdef DEBUG 
    printf("Z^2 : \n");
    NUM_PRINT(t1);
    printf("Z^3 : \n");
    NUM_PRINT(t2);
    #endif

    vli_modMult_fast(x, X1, t1); /* x1 * z^2 */
    vli_modMult_fast(y, Y1, t2); /* y1 * z^3 */
}


void ecc_point_decompress(EccPoint *p_point, const uint8_t p_compressed[ECC_BYTES+1])
{   /* get the point from the x coordinate */
    uint8_t _3[ECC_BYTES] = {3}; /* -a = 3 */
    memcpy(p_point->x, p_compressed+1, ECC_BYTES);

    vli_modSquare_fast(p_point->y, p_point->x); /* y = x^2 */
    vli_modSub(p_point->y, p_point->y, _3, curve_p); /* y = x^2 - 3 */
    vli_modMult_fast(p_point->y, p_point->y, p_point->x); /* y = x^3 - 3x */
    vli_modAdd(p_point->y, p_point->y, curve_b, curve_p); /* y = x^3 - 3x + b */
    
    mod_sqrt(p_point->y);
    
    if((p_point->y[0] & 0x01) != (p_compressed[0] & 0x01))
    {/* seems like that the first bits of coordinates are used to represent that the 
        location of the point, upsides the x-axis or downsides */
        vli_sub(p_point->y, curve_p, p_point->y);
    }
}

#undef DEBUG

uint8_t check(uint8_t *x, uint8_t *y) {
    uint8_t _3[ECC_BYTES] = {3}; /* -a = 3 */
    uint8_t _y[ECC_BYTES]; /* -a = 3 */
    uint8_t _y1[ECC_BYTES]; /* -a = 3 */

    vli_modSquare_fast(_y, x); /* y = x^2 */
    vli_modSub(_y, _y, _3, curve_p); /* y = x^2 - 3 */
    vli_modMult_fast(_y, _y, x); /* y = x^3 - 3x */
    vli_modAdd(_y, _y, curve_b, curve_p); /* y = x^3 - 3x + b */

    vli_modSquare_fast(_y1, y);

    if( 0 == vli_cmp(_y, _y1))
        return 1;

    return 0;
    

}
