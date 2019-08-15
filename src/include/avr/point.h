#ifndef POINT_H
#define POINT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
    the configuration parameters
 */

#define MAX_TRIES 16

uint8_t EccPoint_isZero(EccPoint *p_point);

/**
 * @brief function : P -> 2P
 * @param[out] X 2P.x
 * @param[out] Y 2P.y
 * @param[in] X1 P.x 
 * @param[in] Y1 P.y 
 */
void EccPoint_double_jacobian(uint8_t *X, uint8_t *Y, uint8_t *X1, uint8_t *Y1);


void divide_z(uint8_t *x, uint8_t *y, uint8_t *X1, uint8_t *Y1, uint8_t *Z);

void mult_z(uint8_t *x, uint8_t *y, uint8_t *X1, uint8_t *Y1, uint8_t *Z);

void XYcZ_add(uint8_t *X1, uint8_t *Y1, uint8_t *X2, uint8_t *Y2);
void XYcZ_addC(uint8_t *X1, uint8_t *Y1, uint8_t *X2, uint8_t *Y2);

/**
 * @brief multiply a point with a scalar
 * @param[out] p_result the product 
 * @param[in] p_point the point 
 * @param[in] p_scalar the scalar 
 */
void EccPoint_mult(EccPoint *p_result, EccPoint *p_point, uint8_t *p_scalar);
void ecc_point_decompress(EccPoint *p_point, const uint8_t p_compressed[ECC_BYTES+1]);
void EccPoint_add_jacobian(uint8_t *x3, uint8_t *y3, uint8_t *x1, 
                            uint8_t *y1, uint8_t *x2, uint8_t *y2);

uint8_t check(uint8_t *x, uint8_t *y);


#ifdef DEBUG 
void XYcZ_add(uint8_t *X1, uint8_t *Y1, uint8_t *X2, uint8_t *Y2);
void XYcZ_addC(uint8_t *X1, uint8_t *Y1, uint8_t *X2, uint8_t *Y2);
#endif


#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* POINT_H */