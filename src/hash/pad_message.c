/**
 * @file "pad_message.c"
 * @author Wang Ruikai 
 * @date July 31st, 2019 
 * @brief pad message for hash functions
 */

#include <stdio.h>
#include <string.h>

#include <hash.h>

int pad_message_small (char *result, const char* message, uint64_t m_len) {

    int64_t last_valid_len = (int64_t) (m_len % BLOCK_BYTE_SIZE) ;
    uint64_t num_block = (m_len + BLOCK_BYTE_SIZE - 1) / BLOCK_BYTE_SIZE;

    int64_t k = (448/8 - 1 - last_valid_len);
    #ifdef DEBUG 
    printf("k : %d\n", k);
    #endif

    char *last_block = NULL;
    char *last_64 = NULL;

    memcpy(result, message, m_len);

    if (k >= 0) {

        // find the last block 
        last_block = result + num_block*BLOCK_BYTE_SIZE - BLOCK_BYTE_SIZE;
        // find the start of the last 64 bits 
        last_64 = result + num_block*BLOCK_BYTE_SIZE - 8;

    } else {

        k += BLOCK_BIT_SIZE;
        last_block = result + num_block - BLOCK_BYTE_SIZE;
        last_64 = result + num_block - 8;

    }

    // pad k 0s from the last_64 toward left side 
    int i;
    char *p = last_64 - 1;
    
    #ifdef DEBUG 
    printf("p-result : %ld", p-result);
    #endif

    for (i=0; i<k; i++, p--) {
        *p = 0;
    }
    *p = 0x80;


    // copy the 64-bit length here 
    uint64_t length = m_len * 8;
    #ifdef DEBUG 
    printf("bbit length : %08x", length);
    #endif

    for(i=0; i<8; i++) {
        last_64[i] = (char)((length >> (7-i)*8) & 0xff);  /* The original bug seems to cause overflow ? */
    }


}

int pad_message(char *result, const char *message, unsigned int m_len)
{

    pad_message_small(result, message, m_len);

}
