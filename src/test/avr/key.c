#include <stdio.h>
#include <string.h>

#include <avr/curves.h>
#include <avr/ecc.h>
#include <avr/point.h>
#include <avr/field.h>

#define ERROR(info) fprintf(stderr, "[%s:%d]%s\n    %s\n", __FILE__, \
                __LINE__, __func__, info) 


uint8_t p_publicKey[ECC_BYTES+1] = {0}, p_privateKey[ECC_BYTES] = {0};
uint8_t message[ECC_CURVE] = "Lorem ipsum dolor sit amet.";
uint8_t * signature;
size_t m_len = ECC_CURVE;
uint8_t *c, *dm;
size_t c_len, dm_len;


int test_make_key()
{
    /* code */
    ecc_make_key(p_publicKey, p_privateKey);

    #ifdef DEBUG 

    EccPoint debug_p;
    ecc_point_decompress(&debug_p, p_publicKey);
    ERROR("genereated public key");
    NUM_PRINT(debug_p.x);
    NUM_PRINT(p_privateKey);

    #endif

    return 1;
}

int test_encrypt()
{
    ERROR("test");

    if(0 == ecc_encrypt(&c, &c_len, message, m_len, p_publicKey)) {

        ERROR("encrypt error");
        goto end;

    }

    ERROR("encrypt finish");

    
    #ifndef ANALYZE
    {
        printf("plain message:\n\t");
        printf("%s\n", message);
        printf("cipher length: %ld\n", c_len);
        printf("encrypted cipher:");

        int i;
        for(i=0; i<c_len; i++) {
            if(0 == i%16) printf("\n\t");
            if(0 == i%4) printf(" ");
            printf("%02x", c[i]&0xff);
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
    #ifndef ANALYZE
    {
        printf("cipher length: %ld\n", c_len);
        int i;
        printf("cipher:");
        for(i=0; i<c_len; i++) {
            if(0 == i%16) printf("\n\t");
            if(0 == i%4) printf(" ");
            printf("%02x", c[i]&0xff);
        }
        printf("\n");

    }
    #endif

    #ifdef DEBUG 
    ERROR("private key before decryption");
    NUM_PRINT(p_privateKey);
    #endif
    if(0 == ecc_decrypt(&dm, &dm_len, c, c_len, p_privateKey)) {

        ERROR("decrypt error");
        goto end;

    }
    #ifdef DEBUG 
    ERROR("private key after decryption");
    NUM_PRINT(p_privateKey);
    #endif

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

    #ifndef ANALYZE
    {
        
        printf("decrypted message:\n\t");
        printf("%s\n", dm);
    }
    #endif
 
    return 1;

end:
    return 0;

}

int test_sign()
{

    signature = ecdsa_sign_new();
    if (0 == ecdsa_sign(signature, p_privateKey, message, m_len)) {
        ERROR("errors in sign");
        goto end;
    }

    #ifndef ANALYZE 
    printf(" the digest is following :\n");
    ecdsa_sign_print(signature);
    #endif 

    return 1;
end:
    return 0;

}

int test_verify() 
{
    #ifndef ANALYZE 
    printf(" the digest is following :\n");
    ecdsa_sign_print(signature);
    #endif 

    if (0 == ecdsa_verify(p_publicKey, message, m_len, signature)) {
        ERROR("error in verifying");
        goto end;
    }

    return 1;
end:
    return 0;

}



int main(int argc, uint8_t *argv[]) {

    int count;
    if(1 == argc) {
        count = 1;
    }

    else {
        count = atoi(argv[1]);
    }

    int i;
    for(i=0; i<count; i++) {

        #ifndef ANALYZE 
        printf("\n[ test ] make key\n\n");
        #endif

        if(0 == test_make_key()) {

            ERROR("error in make_key");
            goto end;

        }

        // #ifndef ANALYZE 
        // printf("\n[ test ] encrypt\n\n");
        // #endif

        // if(0 == test_encrypt()) {

        //     ERROR("error in encrypt");
        //     goto end;

        // }

        // #ifndef ANALYZE 
        // printf("\n[ test ] decrypt\n\n");
        // #endif

        // if(0 == test_decrypt()) {

        //     ERROR("error in decrypt");
        //     goto end;

        // }

        #ifndef ANALYZE 
        printf("\n[ test ] sign\n\n");
        #endif

        if(0 == test_sign()) {

            ERROR("error in sign");
            goto end;

        }

        #ifndef ANALYZE 
        printf("\n[ test ] verify\n\n");
        #endif

        if(0 == test_verify()) {

            ERROR("error in verify");
            goto end;

        }


    }



    return 0;
end:

    return -1;
}

