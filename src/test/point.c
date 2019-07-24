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
        if(i%4==3) printf("\n");    \
        printf("%016lx ", n[i]);       \
    }               \
    printf("\n\n");           \
}

#define test_G_32 { \
    {0xF4A13945D898C296ull, 0x77037D812DEB33A0ull, 0xF8BCE6E563A440F2ull, 0x6B17D1F2E12C4247ull}, \
    {0xCBB6406837BF51F5ull, 0x2BCE33576B315ECEull, 0x8EE7EB4A7C0F9E16ull, 0x4FE342E2FE1A7F9Bull}}

#define test_G_2 { \
    {0xA60B48FC47669978ull, 0xC08969E277F21B35ull, 0x8A52380304B51AC3ull, 0x7CF27B188D034F7Eull}, \
    {0x9E04B79D227873D1ull, 0xBA7DADE63CE98229ull, 0x293D9AC69F7430DBull, 0x07775510DB8ED040ull}}

static EccPoint test_G = CONCAT(test_G_, ECC_CURVE);
static EccPoint test_G_d = test_G_2;

int test_mult() 
{

    uint64_t Rx[2][NUM_ECC_DIGITS];
    uint64_t Ry[2][NUM_ECC_DIGITS];

    uint64_t scalar[NUM_ECC_DIGITS] = {2};
    uint64_t one[NUM_ECC_DIGITS] = {1};

    EccPoint p;

    vli_set(Rx[1], test_G.x);
    vli_set(Ry[1], test_G.y);

    /* x 1 */

    EccPoint_mult(&p, &curve_G, one);

    printf("P : \n");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);

    /* x 2 */
    int k;
    for (k=1; k<=6; k++) {

        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("====================================");
        printf("====================================\n");

        vli_clear(scalar);
        scalar[0] = k;
        // vli_modAdd(scalar, scalar, curve_n, curve_p);
        EccPoint_mult(&p, &curve_G, scalar);

        printf("[%d]P : \n", k);
        NUM_PRINT(p.x);
        NUM_PRINT(p.y);
        check(p.x, p.y);
        printf("====================================");
        printf("====================================\n");

    }

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("====================================");
    printf("====================================\n");

    scalar[0] = 2;
    EccPoint_mult(&p, &curve_G, scalar);
    // EccPoint_mult(&p, &p, scalar);
    scalar[0] = 3;
    EccPoint_mult(&p, &p, scalar);

    printf("[%d]P : \n", 6);
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);
    printf("====================================");
    printf("====================================\n");

    // XYcZ_addC(Rx[0],Ry[0],Rx[1],Ry[1]);

    // printf("P-Q : \n");
    // NUM_PRINT(Rx[0]);
    // NUM_PRINT(Ry[0]);

    // printf("P+Q : \n");
    // NUM_PRINT(Rx[1]);
    // NUM_PRINT(Ry[1]);


    return 1;

end:
    return 0;    

}

int test_add() 
{

    // uint64_t Rx[2][NUM_ECC_DIGITS];
    // uint64_t Ry[2][NUM_ECC_DIGITS];

    // vli_set(Rx[0], curve_G.x);
    // vli_set(Rx[1], curve_G.x);
    // vli_set(Ry[0], curve_G.y);
    // vli_set(Ry[1], curve_G.y);

    // printf("P or Q : \n");
    // NUM_PRINT(Rx[0]);
    // NUM_PRINT(Ry[0]);

    // XYcZ_add(Rx[0],Ry[0],Rx[1],Ry[1]);

    // printf("P : \n");
    // NUM_PRINT(Rx[0]);
    // NUM_PRINT(Ry[0]);

    // printf("P+Q : \n");
    // NUM_PRINT(Rx[1]);
    // NUM_PRINT(Ry[1]);
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("====================================");
    printf("====================================\n");

    EccPoint p;
    EccPoint_add_jacobian(p.x, p.y, test_G.x, test_G.y, test_G_d.x, test_G_d.y);    

    printf("[%d]P : \n", 3);
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);
    printf("====================================");
    printf("====================================\n");


    return 1;

end:
    return 0;    

}


int main(int argc, char *argv[]) {

    if(0 == test_mult()) {

        ERROR("error in addC");
        goto end;

    }

    if(0 == test_add()) {

        ERROR("error in add");
        goto end;

    }

    return 0;
end:

    return -1;
}

