#include <stdio.h>
#include <string.h>

#include <avr/curves.h>
#include <avr/ecc.h>
#include <avr/point.h>
#include <avr/field.h>

int test_square() 
{

    printf("------------------------------------\n");
    printf("|           test square            |\n");
    printf("------------------------------------\n");

    uint8_t src[ECC_BYTES] = {0};
    uint8_t dest[ECC_BYTES] = {0};
    src[1] = 1;
    NUM_PRINT(src);

    /* x 2 */
    int k;
    for (k=1; k<=20; k++) {


        src[0] = k;
        vli_modSquare_fast(dest, src);

        NUM_PRINT(src);
        NUM_PRINT(dest);

        printf("\n");

    }

    return 1;

end:
    return 0;    

}

int test_multiply() 
{

    printf("------------------------------------\n");
    printf("|           test multiply            |\n");
    printf("------------------------------------\n");

    uint8_t src1[ECC_BYTES] = {0};
    uint8_t src2[ECC_BYTES] = {0};
    uint8_t dest[ECC_BYTES] = {0};
    src1[1] = 1;

    /* x 2 */
    int k, i;
    for (k=1; k<=20; k++) {
        for(i=k; i<=20; i++) {

        src1[0] = k;
        src2[0] = i;

        vli_modMult_fast(dest, src1, src2);

        NUM_PRINT(src1);
        NUM_PRINT(src2);
        NUM_PRINT(dest);

        printf("\n");

        }

    }


    return 1;

end:
    return 0;    

}

int test_inv() 
{

    printf("------------------------------------\n");
    printf("|           test inversion            |\n");
    printf("------------------------------------\n");

    uint8_t src1[ECC_BYTES] = {0};
    uint8_t dest[ECC_BYTES] = {0};

    /* x 2 */
    int k, i;
    for (k=1; k<=20; k++) {

        src1[0] = k;

        vli_modInv(dest, src1, curve_p);

        ERROR("src");
        NUM_PRINT(src1);
        ERROR("dest");
        NUM_PRINT(dest);

        vli_modMult_fast(dest, dest, src1);

        ERROR("multiply");
        NUM_PRINT(dest);

        printf("\n");


    }


    return 1;

end:
    return 0;    

}


int main(int argc, char *argv[]) {

    if(0 == test_inv()) {

        ERROR("error in inversion");
        goto end;

    }


    if(0 == test_square()) {

        ERROR("error in square");
        goto end;

    }

    // if(0 == test_multiply()) {

    //     ERROR("error in add");
    //     goto end;

    // }

    return 0;
end:

    return -1;
}

