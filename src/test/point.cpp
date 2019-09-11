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



uint8_t p_publicKey[ECC_BYTES+1] = {0}, p_privateKey[ECC_BYTES] = {0};

unsigned long time;
unsigned long time1;


int test_make_key()
{
    /* code */
    ecc_make_key(p_publicKey, p_privateKey);

    return 1;
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

        EccPoint p;
        ecc_point_decompress(&p, p_publicKey);
        
        Serial.println("\n[ test ] no-fixed\n\n");
        Serial.print("Time: ");

        time = micros();
        EccPoint_mult(&p, &p, p_privateKey);
        time1 = micros();
        Serial.println(time1-time); //prints time since program started

/*        Serial.println("\n[ test ] fixed\n\n");
        Serial.print("Time: ");

        time = time1;
        EccPoint_mult_P(&p, l_privateKey);
        time1 = micros();
        Serial.println(time1-time); //prints time since program started
*/
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
