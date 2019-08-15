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

#include "avr/curves.h"
#include "avr/field.h"
#include "avr/ecc.h"
#include "avr/point.h"

#define UMAX(a, b) a>b ? a : b
#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s", __FILE__, \
                __LINE__, __func__, info) 
 

uint8_t ecc_encrypt(uint8_t **c, size_t *c_len, 
                uint8_t *m, size_t m_len, uint8_t *key) {

    if(0 != m_len % ECC_BYTES) {
        ERROR("the length of the message must be a multiple of ECC_BYTES");
        goto end;
    }

    /* choose a random k */
    uint8_t k[ECC_BYTES];
    uint8_t l_tmp[ECC_BYTES];
    uint8_t l_s[ECC_BYTES];
    EccPoint p;
    EccPoint pk_point;
    uint8_t l_tries = 0;
    uint8_t j;
    
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
        if( 0 == check(pk_point.x, pk_point.y)) {
            ERROR(" the pk_point is not on the curve");
            goto end;
        }

        #ifdef DEBUG 
        ERROR("public key used");
        NUM_PRINT(pk_point.x);
        NUM_PRINT(pk_point.y);
        #endif


        EccPoint_mult(&pk_point, &pk_point, k);

        if( 0 == check(pk_point.x, pk_point.y)) {
            ERROR(" k * pk_point is not on the curve");
            goto end;
        }

        EccPoint_mult(&p, &curve_G, k);

        if( 0 == check(p.x, p.y)) {
            ERROR(" p is not on the curve");
            goto end;
        }
    
    } while(vli_isZero(p.x) || vli_isZero(pk_point.x));

    #ifdef DEBUG 
    ERROR("k * pk");
    NUM_PRINT(pk_point.x);
    NUM_PRINT(pk_point.y);
    ERROR("k * G");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);
    #endif

    uint8_t digit_cnt = m_len / ECC_BYTES;
    uint8_t i=0;

    uint8_t * ret = (uint8_t *)malloc(ECC_BYTES + m_len + 1);
    
    /* cipher and the random point */
    ret[0] = 2 + (p.y[0] & 0x01);
    memcpy(ret+1, p.x, ECC_BYTES);
    
    /* copy the cipher */ 
    uint8_t *lpt = pk_point.x;
    uint8_t *respt = ret + ECC_BYTES + 1;
    uint8_t *rpt = m;
    
    for(i=1; i<=digit_cnt; ++i) 
    {
        
        vli_xor(respt, lpt, rpt);
        
        rpt += ECC_BYTES;
        respt += ECC_BYTES ;
    }

    *c_len = m_len + ECC_BYTES + 1;
    *c = ret;
    return 1;

end:
    free(ret);
    return 0;
}

