#include <stdio.h>
#include <string.h>

#include <curves.h>
#include <ecc.h>
#include <point.h>
#include <field.h>

#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif


#define ERROR(info) { Serial.print("wrong on line "); Serial.println(__LINE__); Serial.println(info);}



char p_publicKey[ECC_BYTES+1] = {0}, p_privateKey[ECC_BYTES] = {0};
char message[ECC_CURVE] = "Lorem ipsum.";
char * signature;
size_t m_len = ECC_CURVE;
char *c, *dm;
size_t c_len, dm_len;

unsigned long time;


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

    
    #ifndef ANALYZE
    {
        Serial.println("plain message:\n\t");
        Serial.println((char *)message);

        // Serial.println("encrypted cipher:");
        // int i;
        // for(i=0; i<c_len; i++) {
        //     if(0 == i%16) Serial.println("\n\t");
        //     if(0 == i%4) Serial.println(" ");
        //     Serial.println(c[i], HEX);
        // }
        // Serial.println("\n");
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

    #ifndef ANALYZE
    {
        // int i;
        // Serial.println("cipher:");
        // for(i=0; i<c_len; i++) {
        //     if(0 == i%16) Serial.println("\n\t");
        //     if(0 == i%4) Serial.println(" ");
        //     Serial.println(c[i], HEX);
        // }
        // Serial.println("\n");
        
        Serial.println("decrypted message:\n\t");
        Serial.println((char *)dm);
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
    // Serial.println(" the digest is following :\n");
    // int i;
    // for (i=0; i<2 * ECC_BYTES; i++) {
    //     if(i%4==0) Serial.println(" ");
    //     if(i%16==0) Serial.println(" \n");
    //     Serial.println(signature[i] & 0xff, HEX);
    // }
    // Serial.println("\n\n");
    #endif 

    return 1;
end:
    return 0;

}

int test_verify() 
{

    if (0 == ecdsa_verify(p_publicKey, message, m_len, signature)) {
        ERROR("error in verifying");
        goto end;
    }

    return 1;
end:
    return 0;

}



int test(int count) {

    int i;
    for(i=0; i<count; i++) {

        #ifndef ANALYZE 
        Serial.println("\n[ test ] make key\n\n");
        #endif

        if(0 == test_make_key()) {

            ERROR("error in make_key");
            goto end;

        }

        Serial.print("Time: ");
        time = micros();
        Serial.println(time); //prints time since program started

        #ifndef ANALYZE 
        Serial.println("\n[ test ] encrypt\n\n");
        #endif

        if(0 == test_encrypt()) {

            ERROR("error in encrypt");
            goto end;

        }

        Serial.print("Time: ");
        time = micros();
        Serial.println(time); //prints time since program started

        #ifndef ANALYZE 
        Serial.println("\n[ test ] decrypt\n\n");
        #endif

        if(0 == test_decrypt()) {

            ERROR("error in decrypt");
            goto end;

        }
        Serial.print("Time: ");
        time = micros();
        Serial.println(time); //prints time since program started

        #ifndef ANALYZE 
        Serial.println("\n[ test ] sign\n\n");
        #endif

        if(0 == test_sign()) {

            ERROR("error in sign");
            goto end;

        }
        Serial.print("Time: ");
        time = micros();
        Serial.println(time); //prints time since program started

        #ifndef ANALYZE 
        Serial.println("\n[ test ] verify\n\n");
        #endif

        if(0 == test_verify()) {

            ERROR("error in verify");
            goto end;

        }
        Serial.print("Time: ");
        time = micros();
        Serial.println(time); //prints time since program started


    }



    return 0;
end:

    return -1;
}


void setup() {
    Serial.begin(9600);
    while(!Serial) {

    }
    Serial.println("begin : ");
}

void loop() {
    test(1);
}
