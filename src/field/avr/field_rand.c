#include <avr/curves.h>
#include <avr/field.h>

/* 
    operations on numbers and vectors
 */


uint8_t getRandomNumber(uint8_t *p_vli)
{
    
    uint8_t *l_ptr = (uint8_t *)p_vli;
    uint8_t i;

    for (i=0; i<ECC_BYTES; i++) {
        *l_ptr = random(0xff);
        l_ptr ++;
    }

    return 1;
}


