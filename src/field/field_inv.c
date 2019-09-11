#include <curves.h>
#include <field.h>

#define EVEN(vli) (!(vli[0] & 1))
/* Computes p_result = (1 / p_input) % p_mod. All VLIs are the same size.
   See "From Euclid's GCD to Montgomery Multiplication to the Great Divide"
   https://labs.oracle.com/techrep/2001/smli_tr-2001-95.pdf */
void vli_modInv(uint8_t *p_result, uint8_t *p_input, uint8_t *p_mod)
{
    uint8_t one[ECC_BYTES] = {1};
    uint8_t a[ECC_BYTES] = {1}, b[ECC_BYTES] = {0}; 
    uint8_t u[ECC_BYTES], v[ECC_BYTES];
    vli_set(u, p_input);
    vli_set(v, p_mod);
    
    while ( (vli_cmp(u, one)!=0) && (vli_cmp(v, one)!=0) ) {

        while(EVEN(u)) {

            vli_rshift1(u);
            if(EVEN(a)) {
                vli_rshift1(a);
            } else {
                uint8_t carry = vli_add(a, a, p_mod);
                vli_rshift1(a);
                a[ECC_BYTES-1] |= (carry << 7);
            }

        }

        while(EVEN(v)) {

            vli_rshift1(v);
            if(EVEN(b)) {
                vli_rshift1(b);
            } else {
                uint8_t carry = vli_add(b, b, p_mod);
                vli_rshift1(b);
                b[ECC_BYTES-1] |= (carry << 7);
            }

        }

        if(vli_cmp(u, v) >= 0) {
            vli_modSub(u, u, v, p_mod);
            vli_modSub(a, a, b, p_mod);
        }
        else {
            vli_modSub(v, v, u, p_mod);
            vli_modSub(b, b, a, p_mod);
        }

    } // while 

    if (0 == vli_cmp(u, one)) {
        vli_set(p_result, a);
    } 
    else {
        vli_set(p_result, b);
    }

}
