#include <curves.h>
#include <field.h>


#include<stdio.h>
/* Compute a = sqrt(a) (mod curve_p). */
void mod_sqrt(uint8_t a[ECC_BYTES])
{
    uint16_t i;
    uint8_t p1[ECC_BYTES] = {1};
    uint8_t l_result[ECC_BYTES] = {1};
    
    /* Since curve_p == 3 (mod 4) for all supported curves, we can
       compute sqrt(a) = a^((curve_p + 1) / 4) (mod curve_p). */
    vli_add(p1, curve_p, p1); /* p1 = curve_p + 1 */
    for(i = vli_numBits(p1) - 1; i > 1; --i)
    {
        #ifdef DEBUG
        if(i==351) {
            ERROR("351");
            NUM_PRINT(l_result);
        }        
        #endif
        vli_modSquare_fast(l_result, l_result);
        #ifdef DEBUG
        if(i==351) {
            NUM_PRINT(l_result);
        }        
        #endif
        if(vli_testBit(p1, i))
        {
            vli_modMult_fast(l_result, l_result, a);
            #ifdef DEBUG
            if(i==351) {
                NUM_PRINT(l_result);
            }
            #endif        
        }
    }
    vli_set(a, l_result);
}
