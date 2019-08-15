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

int ecc_decrypt(char **m, size_t *m_len, char *c, 
                        size_t c_len, char *key)
{

    size_t num_digit = c_len - ECC_CURVE;
    char *ret = (char *)malloc(num_digit);

    uint64_t sk[NUM_ECC_DIGITS];
    ecc_bytes2native(sk, key);

    EccPoint p;
    ecc_point_decompress(&p, c);         /* get the k*G */

    EccPoint_mult(&p, &p, sk);   /* get the s*kG */

    #ifdef DEBUG 
    printf("decrypt p : \n");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);
    #endif

    uint64_t res[NUM_ECC_DIGITS];
    uint64_t r[NUM_ECC_DIGITS];
    uint64_t *l = p.x;

    char *rpt = c + ECC_CURVE + 1;
    char *respt = ret;

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
