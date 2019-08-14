#include <stdio.h>
#include <string.h>

#include <avr/curves.h>
#include <avr/ecc.h>
#include <avr/point.h>
#include <avr/field.h>

#define test_G_32 { \
    {0xF4A13945D898C296ull, 0x77037D812DEB33A0ull, 0xF8BCE6E563A440F2ull, 0x6B17D1F2E12C4247ull}, \
    {0xCBB6406837BF51F5ull, 0x2BCE33576B315ECEull, 0x8EE7EB4A7C0F9E16ull, 0x4FE342E2FE1A7F9Bull}}

#define test_G_2 { \
    {0xA60B48FC47669978ull, 0xC08969E277F21B35ull, 0x8A52380304B51AC3ull, 0x7CF27B188D034F7Eull}, \
    {0x9E04B79D227873D1ull, 0xBA7DADE63CE98229ull, 0x293D9AC69F7430DBull, 0x07775510DB8ED040ull}}

#define test_S_32 { \
    {0xb77cab3989da97c9ull, 0x617519b3221a8fa0ull, 0x82edd27e0f271508ull, 0xde2444bebc8d36e6ull}, \
    {0xb6350b243042a256ull, 0x702de80f53cec576ull, 0xfc01a5aad1e66659ull, 0xc093ae7ff36e5380ull  }}

#define test_T_32 { \
    {0xbb8cf92e35e0986bull, 0x39540dc861c89575ull, 0x62f6b3b25316212eull, 0x55a8b00f8da1d44eull},  \
    {0xb656e9d8c8b24316ull, 0xf61a8a52598b9e7aull, 0x4835d82ac4c3dd90ull, 0x5421c3209c2d6c70ull}}

static EccPoint test_G = CONCAT(test_G_, ECC_CURVE);
static EccPoint test_G_d = test_G_2;

static EccPoint test_S = CONCAT(test_S_, ECC_CURVE);
static EccPoint test_T = CONCAT(test_T_, ECC_CURVE);


int test_mult() 
{

    uint64_t scalar[NUM_ECC_DIGITS] = {2};
    uint64_t one[NUM_ECC_DIGITS] = {1};
    uint64_t two[NUM_ECC_DIGITS] = {2};

    EccPoint p;

    // vli_set(p.x, one);
    // vli_set(p.y, two);
    vli_set(p.x, curve_G.x);
    vli_set(p.y, curve_G.y);

    /* x 1 */

    printf("P : \n");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);

    /* x 2 */
    int k;
    for (k=1; k<=64; k++) {

        vli_clear(scalar);
        scalar[0] = k;
        EccPoint_mult(&p, &p, scalar);

        if(0 == check(p.x, p.y)){
            ERROR("the point calculated is wrong");
            fprintf(stderr, "k=%d\n", k);
            NUM_PRINT(p.x);
            NUM_PRINT(p.y);
            goto end;
        }

    }

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

int NIST_test() 
{

    EccPoint R;
    EccPoint S = CONCAT(test_S_, ECC_CURVE);
    EccPoint T = CONCAT(test_T_, ECC_CURVE);

    uint64_t zero[NUM_ECC_DIGITS] = {0};
    uint64_t one[NUM_ECC_DIGITS] = {1};
    uint64_t two[NUM_ECC_DIGITS] = {2};
    uint64_t three[NUM_ECC_DIGITS] = {3};
    uint64_t n_one[NUM_ECC_DIGITS];

    vli_modSub(n_one, zero, one, curve_p);

    EccPoint_add_jacobian(R.x, R.y, test_S.x, test_S.y, test_T.x, test_T.y);
    // EccPoint_double_jacobian(R.x, R.y, three, n_one);
    // EccPoint_double_jacobian(R.x, R.y, S.x, S.y);

    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("====================================");
    printf("====================================\n");
    NUM_PRINT(R.x);
    NUM_PRINT(R.y);
    printf("====================================");
    printf("====================================\n");

    check(R.x, R.y);


    return 1;

}

int main(int argc, char *argv[]) {

    if(0 == NIST_test()) {

        ERROR("error in NIST test");
        goto end;

    }

    if(0 == test_mult()) {

        ERROR("error in addC");
        goto end;

    }

    // if(0 == test_add()) {

    //     ERROR("error in add");
    //     goto end;

    // }
    printf("pass with no error\n");

    return 0;
end:

    return -1;
}

