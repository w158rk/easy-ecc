#ifndef FIELD_H 
#define FIELD_H 


#ifdef __cplusplus
extern "C"
{
#endif

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
    char m_low;
    char m_high;
} uint128_t;

uint128_t mul_64_64(char p_left, char p_right);

#endif

#ifdef DEBUG 
#define NUM_PRINT(n) {int i; \
    for(i=ECC_BYTES-1; i>=0; i--) { \
        if(i%4==3) printf("\n");    \
        printf("%016lx ", n[i]);       \
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
#endif

int getRandomNumber(char *p_vli);
void vli_clear(char *p_vli);
int vli_isZero(char *p_vli);
char vli_testBit(char *p_vli, char p_bit);
char vli_numDigits(char *p_vli);
char vli_numBits(char *p_vli);
void vli_set(char *p_dest, char *p_src);
int vli_cmp(char *p_left, char *p_right);
char vli_lshift(char *p_result, char *p_in, char p_shift);
void vli_rshift1(char *p_vli);
void vli_xor(char *p_result, char *p_left, char *p_right);
char vli_add(char *p_result, char *p_left, char *p_right);
char vli_add(char *p_result, char *p_left, char *p_right);
char vli_sub(char *p_result, char *p_left, char *p_right);
void vli_mult(char *p_result, char *p_left, char *p_right);
void vli_square(char *p_result, char *p_left);
void vli_modAdd(char *p_result, char *p_left, char *p_right, char *p_mod);
void vli_modSub(char *p_result, char *p_left, char *p_right, char *p_mod);
void vli_mmod_fast(char *p_result, char *p_product);
void vli_modMult_fast(char *p_result, char *p_left, char *p_right);
void vli_modMult(char *p_result, char *p_left, char *p_right, char *p_mod);
void vli_modSquare_fast(char *p_result, char *p_left);
void vli_modInv(char *p_result, char *p_input, char *p_mod);
void mod_sqrt(char a[ECC_BYTES]);


#ifdef __cplusplus
}
#endif

#endif
