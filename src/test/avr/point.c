#include <stdio.h>
#include <string.h>

#include <avr/curves.h>
#include <avr/ecc.h>
#include <avr/point.h>
#include <avr/field.h>

#define NIST_S_32 {{ \
    0xc9, 0x97, 0xda, 0x89, 0x39, 0xab, 0x7c, 0xb7, \
    0xa0, 0x8f, 0x1a, 0x22, 0xb3, 0x19, 0x75, 0x61, \
    0x08, 0x15, 0x27, 0x0f, 0x7e, 0xd2, 0xed, 0x82, \
    0xe6, 0x36, 0x8d, 0xbc, 0xbe, 0x44, 0x24, 0xde},{\
    0x56, 0xa2, 0x42, 0x30, 0x24, 0x0b, 0x35, 0xb6, \
    0x76, 0xc5, 0xce, 0x53, 0x0f, 0xe8, 0x2d, 0x70, \
    0x59, 0x66, 0xe6, 0xd1, 0xaa, 0xa5, 0x01, 0xfc, \
    0x80, 0x53, 0x6e, 0xf3, 0x7f, 0xae, 0x93, 0xc0}}
#define NIST_T_32 {{ \
    0x6b, 0x98, 0xe0, 0x35, 0x2e, 0xf9, 0x8c, 0xbb, \
    0x75, 0x95, 0xc8, 0x61, 0xc8, 0x0d, 0x54, 0x39, \
    0x2e, 0x21, 0x16, 0x53, 0xb2, 0xb3, 0xf6, 0x62, \
    0x4e, 0xd4, 0xa1, 0x8d, 0x0f, 0xb0, 0xa8, 0x55},{\
    0x16, 0x43, 0xb2, 0xc8, 0xd8, 0xe9, 0x56, 0xb6, \
    0x7a, 0x9e, 0x8b, 0x59, 0x52, 0x8a, 0x1a, 0xf6, \
    0x90, 0xdd, 0xc3, 0xc4, 0x2a, 0xd8, 0x35, 0x48, \
    0x70, 0x6c, 0x2d, 0x9c, 0x20, 0xc3, 0x21, 0x54}}

EccPoint NIST_S = CONCAT(NIST_S_, ECC_CURVE);
EccPoint NIST_T = CONCAT(NIST_T_, ECC_CURVE);

int test_mult() 
{

    uint8_t scalar[ECC_BYTES] = {2};
    uint8_t one[ECC_BYTES] = {1};
    uint8_t two[ECC_BYTES] = {2};

    EccPoint p;

    vli_set(p.x, one);
    vli_set(p.y, two);

    /* x 1 */

    printf("P : \n");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);

    /* x 2 */
    int k;
    for (k=1; k<=1; k++) {

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

    // uint8_t Rx[2][ECC_BYTES];
    // uint8_t Ry[2][ECC_BYTES];

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
    // EccPoint_add_jacobian(p.x, p.y, test_G.x, test_G.y, test_G_d.x, test_G_d.y);    

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

    uint8_t scalar[ECC_BYTES] = {2};
    uint8_t one[ECC_BYTES] = {1};
    uint8_t two[ECC_BYTES] = {2};

    EccPoint p;

    vli_set(p.x, curve_G.x);
    vli_set(p.y, curve_G.y);

    /* x 1 */

    printf("P : \n");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);

    /* x 2 */
    int k;
    for (k=1; k<=20; k++) {

        vli_clear(scalar);
        scalar[0] = k;
        EccPoint_mult(&p, &p, scalar);

        if(0 == check(p.x, p.y)){
            ERROR("the point calculated is wrong");
            goto end;
        }

        fprintf(stderr, "k=%d\n", k);
        NUM_PRINT(p.x);
        NUM_PRINT(p.y);


    }

    EccPoint_mult(&p, &curve_G, curve_n);
    ERROR("n * G");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);
    

    if(0 == check(NIST_S.x, NIST_S.y)){
        ERROR("NIST_S is wrong");
        NUM_PRINT(NIST_S.x);
        NUM_PRINT(NIST_S.y);
        goto end;
    }

    if(0 == check(NIST_T.x, NIST_T.y)){
        ERROR("NIST_T is wrong");
        NUM_PRINT(NIST_T.x);
        NUM_PRINT(NIST_T.y);
        goto end;
    }

    EccPoint_add_jacobian(p.x, p.y, NIST_S.x, NIST_S.y, NIST_T.x, NIST_T.y);
    ERROR("R = S + T");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);

    EccPoint_double_jacobian(p.x, p.y, NIST_S.x, NIST_S.y);
    ERROR("R = 2S");
    NUM_PRINT(p.x);
    NUM_PRINT(p.y);

    return 1;

end:
    return 0;    


}

int main(int argc, uint8_t *argv[]) {

    if(0 == NIST_test()) {

        ERROR("error in NIST test");
        goto end;

    }

    // if(0 == test_mult()) {

    //     ERROR("error in addC");
    //     goto end;

    // }

    // if(0 == test_add()) {

    //     ERROR("error in add");
    //     goto end;

    // }
    printf("pass with no error\n");

    return 0;
end:

    return -1;
}

