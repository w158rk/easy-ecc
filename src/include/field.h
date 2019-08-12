#ifndef FIELD_H 
#define FIELD_H 


#ifdef __cplusplus
extern "C"
{
#endif

/*
    type defines
 */
typedef unsigned int uint;

#if defined(__SIZEOF_INT128__) || ((__clang_major__ * 100 + __clang_minor__) >= 302)
    #define SUPPORTS_INT128 1
#else
    #define SUPPORTS_INT128 0
#endif

#if SUPPORTS_INT128
typedef unsigned __int128 uint128_t;
#else

typedef struct
{
    uint64_t m_low;
    uint64_t m_high;
} uint128_t;

uint128_t mul_64_64(uint64_t p_left, uint64_t p_right);

#endif

#ifdef DEBUG 
#define NUM_PRINT(n) {int i; \
    for(i=NUM_ECC_DIGITS-1; i>=0; i--) { \
        if(i%4==3) printf("\n");    \
        printf("%016lx ", n[i]);       \
    }               \
    printf("\n\n");           \
}

#define LONG_NUM_PRINT(n) {int i; \
    for(i=NUM_ECC_DIGITS*2-1; i>=0; i--) { \
        if(i%4==3) printf("\n");    \
        printf("%016lx ", n[i]);       \
    }               \
    printf("\n\n");           \
}
#endif

int getRandomNumber(uint64_t *p_vli);
void vli_clear(uint64_t *p_vli);
int vli_isZero(uint64_t *p_vli);
uint64_t vli_testBit(uint64_t *p_vli, uint p_bit);
uint vli_numDigits(uint64_t *p_vli);
uint vli_numBits(uint64_t *p_vli);
void vli_set(uint64_t *p_dest, uint64_t *p_src);
int vli_cmp(uint64_t *p_left, uint64_t *p_right);
uint64_t vli_lshift(uint64_t *p_result, uint64_t *p_in, uint p_shift);
void vli_rshift1(uint64_t *p_vli);
void vli_xor(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right);
uint64_t vli_add(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right);
uint64_t vli_add(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right);
uint64_t vli_sub(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right);
void vli_mult(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right);
void vli_square(uint64_t *p_result, uint64_t *p_left);
void vli_modAdd(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right, uint64_t *p_mod);
void vli_modSub(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right, uint64_t *p_mod);
void vli_mmod_fast(uint64_t *p_result, uint64_t *p_product);
void vli_modMult_fast(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right);
void vli_modMult(uint64_t *p_result, uint64_t *p_left, uint64_t *p_right, uint64_t *p_mod);
void vli_modSquare_fast(uint64_t *p_result, uint64_t *p_left);
void vli_modInv(uint64_t *p_result, uint64_t *p_input, uint64_t *p_mod);
void mod_sqrt(uint64_t a[NUM_ECC_DIGITS]);
void ecc_bytes2native(uint64_t p_native[NUM_ECC_DIGITS], const uint8_t p_bytes[ECC_BYTES]);
void ecc_native2bytes(uint8_t p_bytes[ECC_BYTES], const uint64_t p_native[NUM_ECC_DIGITS]);
uint128_t add_128_128(uint128_t a, uint128_t b);


#ifdef __cplusplus
}
#endif

#endif
