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


/* Modify (x1, y1) => (x1 * z^2, y1 * z^3) */
/*
    this function is to convert ordinary (x,y) tuples to Jacobian-affine coordinates (X,Y,Z) 
 */
void apply_z(uint64_t *X1, uint64_t *Y1, uint64_t *Z)
{
    uint64_t t1[NUM_ECC_DIGITS];

    vli_modSquare_fast(t1, Z);    /* z^2 */
    vli_modMult_fast(X1, X1, t1); /* x1 * z^2 */
    vli_modMult_fast(t1, t1, Z);  /* z^3 */
    vli_modMult_fast(Y1, Y1, t1); /* y1 * z^3 */
}


void ecc_point_decompress(EccPoint *p_point, const uint8_t p_compressed[ECC_BYTES+1])
{   /* get the point from the x coordinate */
    uint64_t _3[NUM_ECC_DIGITS] = {3}; /* -a = 3 */
    ecc_bytes2native(p_point->x, p_compressed+1);
    
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
