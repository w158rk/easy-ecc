#include <stdio.h>
#include <string.h>

#include <curves.h>
#include <ecc.h>
#include <point.h>
#include <field.h>

#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s\n", __FILE__, \
                __LINE__, __func__, info) 
#define NUM_PRINT(n) {int i; \
    for(i=NUM_ECC_DIGITS-1; i>=0; i--) { \
        printf("%ld ", n[i]);       \
    }               \
    printf("\n");       \
}

int test_square() 
{

    printf("------------------------------------\n");
    printf("|           test square            |\n");
    printf("------------------------------------\n");

    uint64_t src[NUM_ECC_DIGITS] = {0};
    uint64_t dest[NUM_ECC_DIGITS] = {0};
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

    uint64_t src1[NUM_ECC_DIGITS] = {0};
    uint64_t src2[NUM_ECC_DIGITS] = {0};
    uint64_t dest[NUM_ECC_DIGITS] = {0};
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


int main(int argc, char *argv[]) {

    if(0 == test_square()) {

        ERROR("error in square");
        goto end;

    }

    if(0 == test_multiply()) {

        ERROR("error in add");
        goto end;

    }

    return 0;
end:

    return -1;
}

