#ifndef FIELD_H 
#define FIELD_H 


#ifdef __cplusplus
extern "C"
{
#endif

#define NUM_PRINT(n) {int i; \
    for(i=ECC_BYTES-1; i>=0; i--) { \
        if(i%4==3) printf(" ");    \
        if(i%16==15) printf("\n");    \
        printf("%02x ", n[i]&0xff);       \
    }               \
    printf("\n\n");           \
}

#define LONG_NUM_PRINT(n) {int i; \
    for(i=ECC_BYTES*2-1; i>=0; i--) { \
        if(i%4==3) printf("\n");    \
        printf("%016lx ", n[i]);       \
    }               \
    printf("\n\n");           \
}

uint8_t getRandomNumber(uint8_t *p_vli);
void vli_clear(uint8_t *p_vli);
uint8_t vli_isZero(uint8_t *p_vli);
uint8_t vli_testBit(uint8_t *p_vli, uint16_t p_bit);
uint8_t vli_numDigits(uint8_t *p_vli);
uint16_t vli_numBits(uint8_t *p_vli);
void vli_set(uint8_t *p_dest, uint8_t *p_src);
int8_t vli_cmp(uint8_t *p_left, uint8_t *p_right);
uint8_t vli_lshift(uint8_t *p_result, uint8_t *p_in, uint8_t p_shift);
void vli_rshift1(uint8_t *p_vli);
void vli_xor(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
uint8_t vli_add(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
uint8_t vli_add(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
uint8_t vli_sub(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
void vli_mult(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
void vli_square(uint8_t *p_result, uint8_t *p_left);
void vli_modAdd(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right, uint8_t *p_mod);
void vli_modSub(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right, uint8_t *p_mod);
void vli_mmod_fast(uint8_t *p_result, uint8_t *p_product);
void vli_modMult_fast(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right);
void vli_modMult(uint8_t *p_result, uint8_t *p_left, uint8_t *p_right, uint8_t *p_mod);
void vli_modSquare_fast(uint8_t *p_result, uint8_t *p_left);
void vli_modInv(uint8_t *p_result, uint8_t *p_input, uint8_t *p_mod);
void mod_sqrt(uint8_t a[ECC_BYTES]);


#ifdef __cplusplus
}
#endif

#endif
