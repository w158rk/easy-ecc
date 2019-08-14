#include <stdio.h>
#include <string.h>

#include <avr/curves.h>
#include <avr/ecc.h>
#include <avr/point.h>
#include <avr/field.h>

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

