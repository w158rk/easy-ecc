#include <curves.h>
#include <field.h>

/* 
    operations on numbers and vectors
 */

#if (defined(_WIN32) || defined(_WIN64))
/* Windows */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincrypt.h>

int getRandomNumber(uint32_t *p_vli)
{
    HCRYPTPROV l_prov;
    if(!CryptAcquireContext(&l_prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        return 0;
    }

    CryptGenRandom(l_prov, ECC_BYTES, (BYTE *)p_vli);
    CryptReleaseContext(l_prov, 0);
    
    return 1;
}

#elif (defined(__unix__)) /* _WIN32 */

/* Assume that we are using a POSIX-like system with /dev/urandom or /dev/random. */
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef O_CLOEXEC
    #define O_CLOEXEC 0
#endif

int getRandomNumber(uint32_t *p_vli)
{
    int l_fd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
    if(l_fd == -1)
    {
        l_fd = open("/dev/random", O_RDONLY | O_CLOEXEC);
        if(l_fd == -1)
        {
            return 0;
        }
    }
    
    char *l_ptr = (char *)p_vli;
    size_t l_left = ECC_BYTES;
    while(l_left > 0)
    {
        int l_read = read(l_fd, l_ptr, l_left);
        if(l_read <= 0)
        { // read failed
            close(l_fd);
            return 0;
        }
        l_left -= l_read;
        l_ptr += l_read;
    }
    
    close(l_fd);
    return 1;
}

#else 



int getRandomNumber(uint32_t *p_vli)
{
    
    char *l_ptr = (char *)p_vli;
    int i;

    for (i=0; i<ECC_BYTES; i++) {
        *l_ptr = random(0xff);
        l_ptr ++;
    }

    return 1;
}


#endif /* _WIN32 */

