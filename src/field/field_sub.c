#include <curves.h>
#include <field.h>

uint64_t vli_sub(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right)
{
    uint64_t l_borrow = 0;
    uint i;
    for(i=0; i<NUM_ECC_DIGITS; ++i)
    {
        uint64_t l_diff = p_left[i] - p_right[i] - l_borrow;
        if(l_diff != p_left[i])
        {
            l_borrow = (l_diff > p_left[i]);
        }
        p_result[i] = l_diff;
    }
    return l_borrow;
}

/* Computes p_result = (p_left - p_right) % p_mod.
   Assumes that p_left < p_mod and p_right < p_mod, p_result != p_mod. */
void vli_modSub(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right, uint64_t *p_mod)
{
    uint64_t l_borrow = vli_sub(p_result, p_left, p_right);
    if(l_borrow)
    { /* In this case, p_result == -diff == (max int) - diff.
         Since -x % d == d - x, we can get the correct result from p_result + p_mod (with overflow). */
        vli_add(p_result, p_result, p_mod);
    }
}