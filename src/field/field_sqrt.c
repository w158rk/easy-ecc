#include <curves.h>
#include <field.h>

#ifdef DEBUG 
#include<stdio.h>
#endif
/* Compute a = sqrt(a) (mod curve_p). */
void mod_sqrt(uint32_t a[NUM_ECC_DIGITS])
{
    unsigned i;
    uint32_t p1[NUM_ECC_DIGITS] = {1};
    uint32_t l_result[NUM_ECC_DIGITS] = {1};
    
    /* Since curve_p == 3 (mod 4) for all supported curves, we can
       compute sqrt(a) = a^((curve_p + 1) / 4) (mod curve_p). */
    vli_add(p1, curve_p, p1); /* p1 = curve_p + 1 */
    for(i = vli_numBits(p1) - 1; i > 1; --i)
    {
        vli_modSquare_fast(l_result, l_result);
        if(vli_testBit(p1, i))
        {
            vli_modMult_fast(l_result, l_result, a);
        }
    }
    vli_set(a, l_result);
}
