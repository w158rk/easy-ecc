/*
 *  the curves recommended in NIST
 */

#ifndef CURVES_H 
#define CURVES_H

#include <stdint.h>

/* Curve selection options. */
#define secp128r1 16
#define secp192r1 24
#define secp256r1 32
#define secp384r1 48
#ifndef ECC_CURVE
    #define ECC_CURVE secp256r1
#endif

#if (ECC_CURVE != secp128r1 && ECC_CURVE != secp192r1 && ECC_CURVE != secp256r1 && ECC_CURVE != secp384r1)
    #error "Must define ECC_CURVE to one of the available curves"
#endif

#define ECC_BYTES ECC_CURVE

#ifdef __cplusplus
extern "C"
{
#endif

#define NUM_ECC_DIGITS (ECC_BYTES/8)

typedef struct EccPoint
{
    uint64_t x[NUM_ECC_DIGITS];
    uint64_t y[NUM_ECC_DIGITS];
} EccPoint;

#define CONCAT1(a, b) a##b
#define CONCAT(a, b) CONCAT1(a, b)

#define Curve_P_16 {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFDFFFFFFFF}
#define Curve_P_24 {0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFEull, 0xFFFFFFFFFFFFFFFFull}
#define Curve_P_32 {0xFFFFFFFFFFFFFFFFull, 0x00000000FFFFFFFFull, 0x0000000000000000ull, 0xFFFFFFFF00000001ull}
#define Curve_P_48 {0x00000000FFFFFFFF, 0xFFFFFFFF00000000, 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}

#define Curve_A_32 {0xFFFFFFFFFFFFFFFCull, 0x00000000FFFFFFFFull, 0x0000000000000000ull, 0xFFFFFFFF00000001ull}

#define Curve_B_16 {0xD824993C2CEE5ED3, 0xE87579C11079F43D}
#define Curve_B_24 {0xFEB8DEECC146B9B1ull, 0x0FA7E9AB72243049ull, 0x64210519E59C80E7ull}
#define Curve_B_32 {0x3BCE3C3E27D2604Bull, 0x651D06B0CC53B0F6ull, 0xB3EBBD55769886BCull, 0x5AC635D8AA3A93E7ull}
#define Curve_B_48 {0x2A85C8EDD3EC2AEF, 0xC656398D8A2ED19D, 0x0314088F5013875A, 0x181D9C6EFE814112, 0x988E056BE3F82D19, 0xB3312FA7E23EE7E4}

#define Curve_G_16 { \
    {0x0C28607CA52C5B86, 0x161FF7528B899B2D}, \
    {0xC02DA292DDED7A83, 0xCF5AC8395BAFEB13}}

#define Curve_G_24 { \
    {0xF4FF0AFD82FF1012ull, 0x7CBF20EB43A18800ull, 0x188DA80EB03090F6ull}, \
    {0x73F977A11E794811ull, 0x631011ED6B24CDD5ull, 0x07192B95FFC8DA78ull}}
    
#define Curve_G_32 { \
    {0xF4A13945D898C296ull, 0x77037D812DEB33A0ull, 0xF8BCE6E563A440F2ull, 0x6B17D1F2E12C4247ull}, \
    {0xCBB6406837BF51F5ull, 0x2BCE33576B315ECEull, 0x8EE7EB4A7C0F9E16ull, 0x4FE342E2FE1A7F9Bull}}

#define Curve_G_48 { \
    {0x3A545E3872760AB7, 0x5502F25DBF55296C, 0x59F741E082542A38, 0x6E1D3B628BA79B98, 0x8EB1C71EF320AD74, 0xAA87CA22BE8B0537}, \
    {0x7A431D7C90EA0E5F, 0x0A60B1CE1D7E819D, 0xE9DA3113B5F0B8C0, 0xF8F41DBD289A147C, 0x5D9E98BF9292DC29, 0x3617DE4A96262C6F}}

#define Curve_N_16 {0x75A30D1B9038A115, 0xFFFFFFFE00000000}
#define Curve_N_24 {0x146BC9B1B4D22831ull, 0xFFFFFFFF99DEF836ull, 0xFFFFFFFFFFFFFFFFull}
#define Curve_N_32 {0xF3B9CAC2FC632551ull, 0xBCE6FAADA7179E84ull, 0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFF00000000ull}
#define Curve_N_48 {0xECEC196ACCC52973, 0x581A0DB248B0A77A, 0xC7634D81F4372DDF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF}

static uint64_t curve_p[NUM_ECC_DIGITS] = CONCAT(Curve_P_, ECC_CURVE);
static uint64_t curve_b[NUM_ECC_DIGITS] = CONCAT(Curve_B_, ECC_CURVE);
static EccPoint curve_G = CONCAT(Curve_G_, ECC_CURVE);
static uint64_t curve_n[NUM_ECC_DIGITS] = CONCAT(Curve_N_, ECC_CURVE);
static uint64_t curve_a[NUM_ECC_DIGITS] = CONCAT(Curve_A_, ECC_CURVE);

#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif