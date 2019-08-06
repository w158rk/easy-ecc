#include<stdio.h>

#include <hash.h>

int parse_c2i(int *dest, char *src, size_t len) {

    int k, i;

    #ifdef DEBUG 
    printf("\nlength : %d\n", len/4);
    #endif

    for (k=0; k<len/4; k++) {

        dest[k] = 0;
        for (i=0; i<4; i++) {
            /* let the high end be the low end */
            int shift_bit = (3-i)*8;
            dest[k] += ( ((int)(src[4*k + i] << shift_bit)) & ( 0xff << shift_bit) );
        }

    }

    return 1;

}

int parse_i2c(char *dest, int *src, size_t len) {

    int k, i;

    #ifdef DEBUG 
    printf("\nlength : %d\n", len/4);
    #endif

    for (k=0; k<len/4; k++) {

        for (i=0; i<4; i++) {
            /* let the high end be the low end */
            dest[k*4+i] = (char) (src[k] >> (3-i)*8);
        }

    }

    return 1;

}