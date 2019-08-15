#include <hash.h>

#undef DEBUG

int sigma_0_256(int x) {
    int rtn = 0;
    rtn ^= ROTR(7,x);
    rtn ^= ROTR(18,x);
    rtn ^= SHR(3,x);

    return rtn;
}


int sigma_1_256(int x) {
    int rtn = 0;
    rtn ^= ROTR(17,x);
    rtn ^= ROTR(19,x);
    rtn ^= SHR(10,x);

    return rtn;
}

int Sigma_0_256(int x) {
    int rtn = 0;
    rtn ^= ROTR(2, x);
    rtn ^= ROTR(13, x);
    rtn ^= ROTR(22, x);

    return rtn;
}

int Sigma_1_256(int x) {

    int rtn = 0;
    #ifdef DEBUG 
    printf("rtn = %08x\n", x);
    #endif
    rtn ^= ROTR(6, x);
    #ifdef DEBUG 
    printf("rtn = %08x\n", rtn);
    #endif
    rtn ^= ROTR(11, x);
    #ifdef DEBUG 
    printf("rtn = %08x\n", rtn);
    #endif
    rtn ^= ROTR(25, x);
    #ifdef DEBUG 
    printf("rtn = %08x\n", rtn);
    #endif

    return rtn;
}