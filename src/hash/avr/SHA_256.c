/**
 * @file SHA_256.c 
 * @author Wang Ruikai 
 * @date July 31st, 2019 
 * @brief SHA_256
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<hash.h>

#undef DEBUG

int hash_process_256(int *dest, int *src, size_t num_block) 
{

    size_t i;
    int W[64] = { 0 }, t;
    int work[8];
    int *current_block = src;
    int T1, T2;

    /** @todo various hash functions */
    int H[8] = CONCAT(H0_,32);

    for (i=0; i<num_block; i++) {

        for (t=0; t<16; t++) {
            W[t] = current_block[t];
            #ifdef DEBUG 
            printf("W[%d] = %08x\n", t, W[t]);
            #endif
        }

        for ( ; t<64; t++) {
            W[t] = sigma_1_256(W[t-2]);
            W[t] += W[t-7];
            W[t] += sigma_0_256(W[t-15]);
            W[t] += W[t-16];
        }

        for (t=0; t<8; t++) {
            work[t] = H[t];
        }

        current_block += BLOCK_WORD_SIZE;

        for (t=0; t<64; t++) {

            T1 = work[7] + Sigma_1_256(work[4]) + CH(work[4], work[5], work[6]) + K[t] + W[t];
            T2 = Sigma_0_256(work[0]) + MAJ(work[0], work[1], work[2]);
            work[7] = work[6];
            work[6] = work[5];
            work[5] = work[4];
            work[4] = work[3] + T1;
            work[3] = work[2];
            work[2] = work[1];
            work[1] = work[0];
            work[0] = T1 + T2;

        }

        for (t=0; t<8; t++) {
            H[t] = work[t] + H[t];
        }

    }

    memcpy(dest, H, 8 * sizeof(int));
}


char* sha_256_new(size_t message_len ) {

    int64_t k = (448/8 - 1 - (int64_t)message_len%BLOCK_BYTE_SIZE);
    int num_block = (k+BLOCK_BYTE_SIZE-1) / BLOCK_BYTE_SIZE;

    char *rtn = NULL;
    if(k>=0) {
        rtn = (char *)malloc(num_block * BLOCK_BYTE_SIZE * sizeof(char)); 
    }
    else {
        rtn = (char *)malloc( (num_block+1) * BLOCK_BYTE_SIZE * sizeof(char));
    }

    return rtn;
}

int sha_256_free(char *digest) {
    free(digest);
    return 1;
}

int sha_256(char *dest, char *src, size_t src_len) {


    int64_t k = (448/8 - 1 - (int64_t)src_len % BLOCK_BYTE_SIZE);
    int num_block = (k+BLOCK_BYTE_SIZE-1) / BLOCK_BYTE_SIZE;

    if(k<0) {
        num_block ++;
    }
    uint32_t *tmp1 = (uint32_t *)malloc( (num_block) * BLOCK_WORD_SIZE * sizeof(uint32_t));
    uint32_t *tmp2 = (uint32_t *)malloc( (num_block) * BLOCK_WORD_SIZE * sizeof(uint32_t));

    pad_message(dest, src, src_len);
    parse_c2i(tmp1, dest, num_block*BLOCK_BYTE_SIZE);
    hash_process_256(tmp2, tmp1, num_block);
    parse_i2c(dest, tmp2, num_block*BLOCK_BYTE_SIZE);

    free(tmp1);
    free(tmp2);

    return 1;

}

