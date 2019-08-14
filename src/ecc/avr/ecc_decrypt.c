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


uint8_t ecc_decrypt(uint8_t **m, size_t *m_len, uint8_t *c, 
                        size_t c_len, uint8_t *key)
{

    size_t num_digit = c_len - ECC_BYTES - 1;
    uint8_t *ret = (uint8_t *)malloc(num_digit);

    uint8_t *sk;
    sk = key;

    EccPoint p;
    ecc_point_decompress(&p, c);         /* get the k*G */

    EccPoint_mult(&p, &p, sk);   /* get the s*kG */

    #ifdef DEBUG 
    ERROR("decrypted sk*k*G");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);
    #endif

    uint8_t *lpt = p.x;
    uint8_t *rpt = c + ECC_BYTES + 1;
    uint8_t *respt = ret;

    uint8_t i=0;
    
    for(i=0; i<num_digit; ++i) 
    {

        vli_xor(respt, lpt, rpt);
        rpt += ECC_BYTES;
        respt += ECC_BYTES;

    }

    *m = ret;
    *m_len = num_digit;
    return 1;

end:
    free(ret);
    return 0;
    


}
