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

uint8_t ecc_make_key(uint8_t p_publicKey[ECC_BYTES+1], 
                uint8_t p_privateKey[ECC_BYTES])
{
    uint8_t l_private[ECC_BYTES];
    EccPoint l_public;
    uint8_t l_tries = 0;
    
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

        EccPoint_mult(&l_public, &curve_G, l_private);
    } while(EccPoint_isZero(&l_public));
    
    if(0 == check(l_public.x, l_public.y)) {
        ERROR(" the point generate is not on the curve");
        goto end;
    }

    #ifdef DEBUG 
    vli_clear(l_private);
    l_private[0] = 1;
    EccPoint_mult(&l_public, &curve_G, l_private);
    #endif

    memcpy(p_privateKey, l_private, ECC_BYTES);
    memcpy(p_publicKey + 1, l_public.x, ECC_BYTES);
    p_publicKey[0] = 2 + (l_public.y[0] & 0x01);
    return 1;

end:
    return 0;

}
