#include <curves.h>
#include <field.h>

#define EVEN(vli) (!(vli[0] & 1))
/* Computes p_result = (1 / p_input) % p_mod. All VLIs are the same size.
   See "From Euclid's GCD to Montgomery Multiplication to the Great Divide"
   https://labs.oracle.com/techrep/2001/smli_tr-2001-95.pdf */
void vli_modInv(uint32_t *p_result, uint32_t *p_input, uint32_t *p_mod)
{
    uint32_t one[NUM_ECC_DIGITS] = {1};
    uint32_t a[NUM_ECC_DIGITS] = {1}, b[NUM_ECC_DIGITS] = {0}; 
    uint32_t u[NUM_ECC_DIGITS], v[NUM_ECC_DIGITS];
    vli_set(u, p_input);
    vli_set(v, p_mod);
    
    while ( (vli_cmp(u, one)!=0) && (vli_cmp(v, one)!=0) ) {

        while(EVEN(u)) {

            vli_rshift1(u);
            if(EVEN(a)) {
                vli_rshift1(a);
            } else {
                uint32_t carry = vli_add(a, a, p_mod);
                vli_rshift1(a);
                a[NUM_ECC_DIGITS-1] |= (carry << 31);
            }

        }

        while(EVEN(v)) {

            vli_rshift1(v);
            if(EVEN(b)) {
                vli_rshift1(b);
            } else {
                uint32_t carry = vli_add(b, b, p_mod);
                vli_rshift1(b);
                b[NUM_ECC_DIGITS-1] |= (carry << 31);
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

