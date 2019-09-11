#ifndef FIELD_H 
#define FIELD_H 


#ifdef __cplusplus
extern "C"
{
#endif


#define NUM_PRINT(n) {int i; \
    for(i=NUM_ECC_DIGITS-1; i>=0; i--) { \
        if(i%2==1) printf("\n");    \
        printf("%08x ", n[i]);       \
    }               \
    printf("\n\n");           \
}

#define LONG_NUM_PRINT(n) {int i; \
    for(i=NUM_ECC_DIGITS*2-1; i>=0; i--) { \
        if(i%2==1) printf("\n");    \
        printf("%016lx ", n[i]);       \
    }               \
    printf("\n\n");           \
}
typedef unsigned int uint;

int getRandomNumber(uint32_t *p_vli);
void vli_clear(uint32_t *p_vli);
int vli_isZero(uint32_t *p_vli);
uint32_t vli_testBit(uint32_t *p_vli, uint p_bit);
uint vli_numDigits(uint32_t *p_vli);
uint vli_numBits(uint32_t *p_vli);
void vli_set(uint32_t *p_dest, uint32_t *p_src);
int vli_cmp(uint32_t *p_left, uint32_t *p_right);
uint32_t vli_lshift(uint32_t *p_result, uint32_t *p_in, uint p_shift);
void vli_rshift1(uint32_t *p_vli);
void vli_xor(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right);
uint32_t vli_add(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right);
uint32_t vli_sub(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right);
void vli_mult(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right);
void vli_square(uint32_t *p_result, uint32_t *p_left);
void vli_modAdd(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right, uint32_t *p_mod);
void vli_modSub(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right, uint32_t *p_mod);
void vli_mmod_fast(uint32_t *p_result, uint32_t *p_product);
void vli_modMult_fast(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right);
void vli_modMult(uint32_t *p_result, uint32_t *p_left, uint32_t *p_right, uint32_t *p_mod);
void vli_modSquare_fast(uint32_t *p_result, uint32_t *p_left);
void vli_modInv(uint32_t *p_result, uint32_t *p_input, uint32_t *p_mod);
void mod_sqrt(uint32_t a[ECC_BYTES]);
void ecc_bytes2native(uint32_t p_native[NUM_ECC_DIGITS], const char p_bytes[ECC_BYTES]);
void ecc_native2bytes(char p_bytes[ECC_BYTES], const uint32_t p_native[NUM_ECC_DIGITS]);


#ifdef __cplusplus
}
#endif

#endif
