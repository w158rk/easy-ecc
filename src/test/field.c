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

    uint32_t src[NUM_ECC_DIGITS] = {0};
    uint32_t dest[NUM_ECC_DIGITS] = {0};
    src[NUM_ECC_DIGITS-1] = 0xff000000;

    /* x 2 */
    int k;
    for (k=1; k<=1; k++) {


        src[0] |= k;
        NUM_PRINT(src);
        vli_modSquare_fast(dest, src);
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

    uint32_t src1[NUM_ECC_DIGITS] = {0};
    uint32_t src2[NUM_ECC_DIGITS] = {0};
    uint32_t dest[NUM_ECC_DIGITS] = {0};
    src1[NUM_ECC_DIGITS-1] = 0xff000000;
    src1[NUM_ECC_DIGITS-2] = 0xff000000;
    src1[NUM_ECC_DIGITS-3] = 0xff000000;
    src2[NUM_ECC_DIGITS-1] = 0xff000000;
    src2[NUM_ECC_DIGITS-2] = 0xff000000;
    src2[NUM_ECC_DIGITS-3] = 0xff000000;

    /* x 2 */
    int k, i;
    for (k=1; k<=1; k++) {
        for(i=k; i<=5; i++) {

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


int main(int argc, char *argv[]) {

    if(0 == test_square()) {

        ERROR("error in square");
        goto end;

    }

    // if(0 == test_multiply()) {

    //     ERROR("error in mult");
    //     goto end;

    // }

    EccPoint p;

    // check(curve_G.x, curve_G.y);

    uint8_t vx[ECC_BYTES+1];
    ecc_native2bytes(vx+1, curve_G.x);
    vx[0] = (curve_G.y[0]) & 0x01 + 2;
    ecc_point_decompress(&p, vx);

    NUM_PRINT(p.x);
    NUM_PRINT(curve_G.x);
    NUM_PRINT(p.y);
    NUM_PRINT(curve_G.y);
    return 0;
end:

    return -1;
}

