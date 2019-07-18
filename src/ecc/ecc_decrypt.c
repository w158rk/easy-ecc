/** !
 * @file ecc_opt.c
 * @author Wang Ruikai 
 * @date July 15th, 2019
 * @brief the point arithmetic operation
 * */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "curves.h"
#include "field.h"
#include "ecc.h"
#include "point.h"

#define UMAX(a, b) a>b ? a : b
#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s", __FILE__, \
                __LINE__, __func__, info) 

int ecc_decrypt(uint8_t **m, size_t *m_len, uint8_t *c, 
                        size_t c_len, uint8_t *key)
{

    size_t num_digit = c_len - ECC_CURVE;
    uint8_t *ret = (uint8_t *)malloc(num_digit);

    uint64_t sk[NUM_ECC_DIGITS];
    ecc_bytes2native(sk, key);

    EccPoint p;
    ecc_point_decompress(&p, c);         /* get the k*G */

    EccPoint_mult(&p, &curve_G, sk);   /* get the s*G */

    uint64_t res[NUM_ECC_DIGITS];
    uint64_t r[NUM_ECC_DIGITS];
    uint64_t *l = p.x;

    uint8_t *rpt = c + ECC_CURVE + 1;
    uint8_t *respt = ret;

    int i=0;
    
    for(i=0; i<num_digit; ++i) 
    {

        ecc_bytes2native(r,rpt);
        vli_xor(res, l, r);
        ecc_native2bytes(respt, res);
        
        rpt += ECC_CURVE;
        respt += ECC_CURVE;
    }

    *m = ret;
    *m_len = num_digit;
    return 1;

end:
    free(ret);
    return 0;
    


}
