#include <stdio.h>
#include <string.h>

#include <curves.h>
#include <ecc.h>
#include <point.h>
#include <field.h>

#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s\n", __FILE__, \
                __LINE__, __func__, info) 


uint8_t p_publicKey[ECC_BYTES+1] = {0}, p_privateKey[ECC_BYTES] = {0};
uint8_t message[ECC_CURVE] = "hello world";
size_t m_len = ECC_CURVE;
uint8_t *c, *dm;
size_t c_len, dm_len;

int test_multi() 
{
    uint64_t a[NUM_ECC_DIGITS] = {0}, b[NUM_ECC_DIGITS] = {0};
    EccPoint p;
    do
    {
        // if(!getRandomNumber(a))
        // {
        //     ERROR("Can't get a valid random number");
        //     goto end;
        // }
        // if(!getRandomNumber(b))
        // {
        //     ERROR("Can't get a valid random number");
        //     goto end;
        // }
        // if(vli_isZero(a) || vli_isZero(b))
        // {
        //     continue;
        // }
        int j;

        a[0] = 2; 
        b[0] = 1;
    
        if(vli_cmp(curve_n, a) != 1)
        {

            vli_sub(a, a, curve_n);

        }

        if(vli_cmp(curve_n, b) != 1)
        {

            vli_sub(b, b, curve_n);

        }
    
        /* pk = k * pk*/
        /* p = k * G*/
        EccPoint_mult(&p, &curve_G, a);
        EccPoint_mult(&p, &p, b);

        EccPoint_mult(&p, &curve_G, b);
        EccPoint_mult(&p, &p, a);

        vli_modMult_fast(a, a, b);

        EccPoint_mult(&p, &curve_G, a);

    } while(vli_isZero(p.x));

    return 1;

end:
    return 0;    

}

int test_make_key()
{
    /* code */
    ecc_make_key(p_publicKey, p_privateKey);

    return 1;
}

int test_encrypt()
{

    if(0 == ecc_encrypt(&c, &c_len, message, m_len, p_publicKey)) {

        ERROR("encrypt error");
        goto end;

    }

    
    #ifdef DEBUG
    {
        printf("plain message:\n\t");
        printf("%s\n", message);
        printf("cipher length: %ld\n", c_len);
        printf("encrypted cipher:");

        int i;
        for(i=0; i<c_len; i++) {
            if(0 == i%16) printf("\n\t");
            if(0 == i%4) printf(" ");
            printf("%02x", c[i]);
        }
        printf("\n");
    }
    #endif

    return 1;

end:
    return 0;

}

int test_decrypt()
{

    if(0 == ecc_decrypt(&dm, &dm_len, c, c_len, p_privateKey)) {

        ERROR("decrypt error");
        goto end;

    }

    // verify the result 
    if(dm_len != m_len) 
    {

        ERROR("the lengths of the message and the decrypted message differ");
        goto end;

    }

    int i;
    for(i=0; i<m_len; ++i) {

        if(dm[i] != message[i]) 
        {

            ERROR("the contents of the message and the decrypted message differ");
            goto end;

        }
    
    }

    #ifdef DEBUG
    {
        printf("cipher length: %ld\n", c_len);
        int i;
        printf("cipher:");
        for(i=0; i<c_len; i++) {
            if(0 == i%16) printf("\n\t");
            if(0 == i%4) printf(" ");
            printf("%02x", c[i]);
        }
        printf("\n");
        
        printf("decrypted message:\n\t");
        printf("%s\n", dm);
    }
    #endif
 
    return 1;

end:
    return 0;

}




int main(int argc, char *argv[]) {

    int count;
    if(1 == argc) {
        count = 1;
    }

    else {
        count = atoi(argv[1]);
    }

    int i;
    for(i=0; i<count; i++) {
        if(0 == test_make_key()) {

            ERROR("error in make_key");
            goto end;

        }

        if(0 == test_encrypt()) {

            ERROR("error in encrypt");
            goto end;

        }

        if(0 == test_decrypt()) {

            ERROR("error in decrypt");
            goto end;

        }

    }



    return 0;
end:

    return -1;
}

