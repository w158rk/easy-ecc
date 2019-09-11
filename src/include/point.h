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

int EccPoint_isZero(EccPoint *p_point);

/**
 * @brief function : P -> 2P
 * @param[out] X 2P.x
 * @param[out] Y 2P.y
 * @param[in] X1 P.x 
 * @param[in] Y1 P.y 
 */
void EccPoint_double_jacobian(uint32_t *X, uint32_t *Y, uint32_t *Z, uint32_t *X1, uint32_t *Y1, uint32_t *Z1);
void EccPoint_add_jacobian(uint32_t *x1, uint32_t *y1, uint32_t *z1, uint32_t *x2, uint32_t *y2);


void divide_z(uint32_t *x, uint32_t *y, uint32_t *X1, uint32_t *Y1, uint32_t *Z);

void mult_z(uint32_t *x, uint32_t *y, uint32_t *X1, uint32_t *Y1, uint32_t *Z);

void XYcZ_add(uint32_t *X1, uint32_t *Y1, uint32_t *X2, uint32_t *Y2);
void XYcZ_addC(uint32_t *X1, uint32_t *Y1, uint32_t *X2, uint32_t *Y2);

/**
 * @brief multiply a point with a scalar
 * @param[out] p_result the product 
 * @param[in] p_point the point 
 * @param[in] p_scalar the scalar 
 */
void EccPoint_mult(EccPoint *p_result, EccPoint *p_point, uint32_t *p_scalar);
void EccPoint_mult_P(EccPoint *p_result, uint32_t *p_scalar) ;
void ecc_point_decompress(EccPoint *p_point, const char p_compressed[ECC_BYTES+1]);

int check(uint32_t *x, uint32_t *y);


#ifdef DEBUG 
void XYcZ_add(uint32_t *X1, uint32_t *Y1, uint32_t *X2, uint32_t *Y2);
void XYcZ_addC(uint32_t *X1, uint32_t *Y1, uint32_t *X2, uint32_t *Y2);
#endif


#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* POINT_H */
