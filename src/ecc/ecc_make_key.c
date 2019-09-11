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

int ecc_make_key(char p_publicKey[ECC_BYTES+1], 
                char p_privateKey[ECC_BYTES])
{
    uint32_t l_private[NUM_ECC_DIGITS];
    EccPoint l_public;
    unsigned l_tries = 0;

    do
    {
        if(!getRandomNumber(l_private) || (l_tries++ >= MAX_TRIES))
        {
            return 0;
        }
        if(vli_isZero(l_private))
        {
            continue;
        }
    
        /* Make sure the private key is in the range [1, n-1].
           For the supported curves, n is always large enough 
           that we only need to subtract once at most. */
        if(vli_cmp(curve_n, l_private) != 1)
        {
            vli_sub(l_private, l_private, curve_n);
        }

        EccPoint_mult_P(&l_public, l_private);
    } while(EccPoint_isZero(&l_public));
    
    #ifdef DEBUG 
    ERROR("private key in key making");
    NUM_PRINT(l_private);
    ERROR("public key in key making");
    NUM_PRINT(l_public.x);
    NUM_PRINT(l_public.y);
    #endif

    if(0 == check(l_public.x, l_public.y)) {
        ERROR(" the point generate is not on the curve");
        goto end;
    }

    ecc_native2bytes(p_privateKey, l_private);
    ecc_native2bytes(p_publicKey + 1, l_public.x);
    p_publicKey[0] = 2 + (l_public.y[0] & 0x01);
    return 1;

end:
    return 0;

}
