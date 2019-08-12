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
 

int ecc_encrypt(char **c, size_t *c_len, 
                char *m, size_t m_len, char *key) {

    if(0 != m_len % ECC_CURVE) {
        ERROR("the length of the message must be a multiple of ECC_CURVE");
        goto end;
    }

    /* choose a random k */
    uint64_t k[NUM_ECC_DIGITS];
    uint64_t l_tmp[NUM_ECC_DIGITS];
    uint64_t l_s[NUM_ECC_DIGITS];
    EccPoint p;
    EccPoint pk_point;
    unsigned l_tries = 0;
    int j;
    
    do
    {
        if(!getRandomNumber(k) || (l_tries++ >= MAX_TRIES))
        {
            ERROR("Can't get a valid random number");
            goto end;
        }
        if(vli_isZero(k))
        {
            continue;
        }
    
        if(vli_cmp(curve_n, k) != 1)
        {
            vli_sub(k, k, curve_n);
        }
    
        /* pk = k * pk*/
        /* p = k * G*/
        ecc_point_decompress(&pk_point, key);        

        EccPoint_mult(&pk_point, &pk_point, k);

        if( 0 == check(pk_point.x, pk_point.y)) {
            ERROR(" the pk_point is not on the curve");
            goto end;
        }

        EccPoint_mult(&p, &curve_G, k);

        if( 0 == check(pk_point.x, pk_point.y)) {
            ERROR(" p is not on the curve");
            goto end;
        }

        #ifdef DEBUG 
        printf("pk_point : ");
        NUM_PRINT(pk_point.x);
        NUM_PRINT(pk_point.y);
        printf("p : ");
        NUM_PRINT(p.x);
        NUM_PRINT(p.y);
        #endif

    
    } while(vli_isZero(p.x) || vli_isZero(pk_point.x));

    int digit_cnt = m_len / ECC_CURVE;
    int i=0;

    uint64_t r[NUM_ECC_DIGITS];
    uint64_t res[NUM_ECC_DIGITS];
    uint64_t *l = pk_point.x;

    char * ret = (char *)malloc(ECC_CURVE + m_len + 1);
    
    char * mask = (char *)malloc(ECC_CURVE);
    
    /* cipher and the random point */
    ecc_native2bytes(ret+1, p.x);
    ret[0] = 2 + (p.y[0] & 0x01);
    
    /* copy the cipher */ 
    char *respt = ret + ECC_CURVE + 1;
    char *rpt = m;
    
    for(i=1; i<=digit_cnt; ++i) 
    {
        
        ecc_bytes2native(r, rpt);
        vli_xor(res, l, r);
        ecc_native2bytes(respt, res);
        
        rpt += ECC_CURVE;
        respt += ECC_CURVE;
    }

    *c_len = m_len + ECC_CURVE;
    *c = ret;
    return 1;

end:
    free(ret);
    return 0;
}

