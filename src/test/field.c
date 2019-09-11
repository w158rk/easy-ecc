#include <stdio.h>
#include <string.h>

#include <curves.h>
#include <ecc.h>
#include <point.h>
#include <field.h>

int test_square() 
{

    printf("------------------------------------\n");
    printf("|           test square            |\n");
    printf("------------------------------------\n");

    uint8_t src[ECC_BYTES] = {0};
    uint8_t dest[ECC_BYTES] = {0};
    src[ECC_BYTES-1] = 0xff;
    src[ECC_BYTES-9] = 0xff;
    src[ECC_BYTES-17] = 0xff;
    src[ECC_BYTES-25] = 0xff;
    src[ECC_BYTES-33] = 0xff;
    src[ECC_BYTES-41] = 0xff;

    /* x 2 */
    int k;
    for (k=1; k<=1; k++) {


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

    /* x 2 */
    int k, i;
    for (k=1; k<=20; k++) {
        for(i=k; i<=20; i++) {

        src1[0] = k;
        src2[0] = i;

        vli_modMult_fast(dest, src1, src2);

        ERROR("multiply");
        NUM_PRINT(src1);
        ERROR("with");
        NUM_PRINT(src2);
        ERROR("get");
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
    int lowerbound = 3;
    int upperbound = 4;
    for (k=lowerbound; k<upperbound; k++) {

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

    // if(0 == test_square()) {

    //     ERROR("error in square");
    //     goto end;

    // }

    uint8_t v[ECC_BYTES+1];
    vli_set(v+1, curve_G.x);
    v[0] = curve_G.y[0] & 0x01 + 2;
    EccPoint p;
    ecc_point_decompress(&p, v);

    NUM_PRINT(p.x);
    NUM_PRINT(curve_G.x);
    NUM_PRINT(p.y);
    NUM_PRINT(curve_G.y);

    // if(0 == test_multiply()) {

    //     ERROR("error in multiplication");
    //     goto end;

    // }

    // if(0 == test_inv()) {

    //     ERROR("error in inversion");
    //     goto end;

    // }

    return 0;
end:

    return -1;
}

