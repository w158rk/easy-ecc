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
void EccPoint_double_jacobian(uint64_t *X1, uint64_t *Y1, uint64_t *Z1);
void apply_z(uint64_t *X1, uint64_t *Y1, uint64_t *Z);
void XYcZ_initial_double(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2, uint64_t *p_initialZ);
void XYcZ_add(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2);
void XYcZ_addC(uint64_t *X1, uint64_t *Y1, uint64_t *X2, uint64_t *Y2);
void EccPoint_mult_plain(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar, uint64_t *p_initialZ);
void EccPoint_mult(EccPoint *p_result, EccPoint *p_point, uint64_t *p_scalar);
void ecc_point_decompress(EccPoint *p_point, const uint8_t p_compressed[ECC_BYTES+1]);
void EccPoint_add_jacobian(uint64_t *X3, uint64_t *Y3, uint64_t *Z3, uint64_t *X1, 
                            uint64_t *Y1, uint64_t *Z1, uint64_t *x2, uint64_t *y2);



#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif /* POINT_H */
