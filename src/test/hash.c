#include<stdio.h>
#include<string.h>

#include "hash.h"

/* https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdf */

int test_sha_256() {

    char message[BLOCK_BYTE_SIZE] = "abc";
    size_t len = strlen(message);
    char *digest = sha_256_new(len);

    sha_256(digest, message, len);
    
    int i;
    for (i=0; i<256/8; i++) {
        if(i%4==0) printf(" ");
        if(i%16==0) printf("\n");
        printf("%02x", digest[i] & 0xff);
    }
    printf("\n");


    sha_256_free(digest);


}

int main() {

    test_sha_256();



    return 0;

end:
    return -1;

}