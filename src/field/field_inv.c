#include <curves.h>
#include <field.h>

#define EVEN(vli) (!(vli[0] & 1))
/* Computes p_result = (1 / p_input) % p_mod. All VLIs are the same size.
   See "From Euclid's GCD to Montgomery Multiplication to the Great Divide"
   https://labs.oracle.com/techrep/2001/smli_tr-2001-95.pdf */
void vli_modInv(uint64_t *p_result, uint64_t *p_input, uint64_t *p_mod)
{
    uint64_t one[NUM_ECC_DIGITS] = {1};
    uint64_t a[NUM_ECC_DIGITS] = {1}, b[NUM_ECC_DIGITS] = {0}; 
    uint64_t u[NUM_ECC_DIGITS], v[NUM_ECC_DIGITS];
    vli_set(u, p_input);
    vli_set(v, p_mod);
    
    while ( (vli_cmp(u, one)!=0) && (vli_cmp(v, one)!=0) ) {

        while(EVEN(u)) {

            vli_rshift1(u);
            if(EVEN(a)) {
                vli_rshift1(a);
            } else {
                uint64_t carry = vli_add(a, a, p_mod);
                vli_rshift1(a);
                a[NUM_ECC_DIGITS-1] |= (carry << 63);
            }

        }

        while(EVEN(v)) {

            vli_rshift1(v);
            if(EVEN(b)) {
                vli_rshift1(b);
            } else {
                uint64_t carry = vli_add(b, b, p_mod);
                vli_rshift1(b);
                b[NUM_ECC_DIGITS-1] |= (carry << 63);
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

void vli_modInv_origin(uint64_t *p_result, uint64_t *p_input, uint64_t *p_mod)
{
    uint64_t a[NUM_ECC_DIGITS], b[NUM_ECC_DIGITS], u[NUM_ECC_DIGITS], v[NUM_ECC_DIGITS];
    uint64_t l_carry;
    int l_cmpResult;
    
    if(vli_isZero(p_input))
    {
        vli_clear(p_result);
        return;
    }

    vli_set(a, p_input);
    vli_set(b, p_mod);
    vli_clear(u);
    u[0] = 1;
    vli_clear(v);
    
    while((l_cmpResult = vli_cmp(a, b)) != 0)
    {
        l_carry = 0;
        if(EVEN(a))
        {
            vli_rshift1(a);
            if(!EVEN(u))
            {
                l_carry = vli_add(u, u, p_mod);
            }
            vli_rshift1(u);
            if(l_carry)         /* overflow, so the first bit should be 1 because there is a carry ahead of the number */
            {
                u[NUM_ECC_DIGITS-1] |= (1 << 63);
            }
        }
        else if(EVEN(b))
        {
            vli_rshift1(b);
            if(!EVEN(v))
            {
                l_carry = vli_add(v, v, p_mod);
            }
            vli_rshift1(v);
            if(l_carry)
            {
                v[NUM_ECC_DIGITS-1] |= 0x8000000000000000ull;
            }
        }
        else if(l_cmpResult > 0)
        {
            vli_sub(a, a, b);
            vli_rshift1(a);
            if(vli_cmp(u, v) < 0)
            {
                vli_add(u, u, p_mod);
            }
            vli_sub(u, u, v);
            if(!EVEN(u))
            {
                l_carry = vli_add(u, u, p_mod);
            }
            vli_rshift1(u);
            if(l_carry)
            {
                u[NUM_ECC_DIGITS-1] |= 0x8000000000000000ull;
            }
        }
        else
        {
            vli_sub(b, b, a);
            vli_rshift1(b);
            if(vli_cmp(v, u) < 0)
            {
                vli_add(v, v, p_mod);
            }
            vli_sub(v, v, u);
            if(!EVEN(v))
            {
                l_carry = vli_add(v, v, p_mod);
            }
            vli_rshift1(v);
            if(l_carry)
            {
                v[NUM_ECC_DIGITS-1] |= 0x8000000000000000ull;
            }
        }
    }
    
    vli_set(p_result, u);
}