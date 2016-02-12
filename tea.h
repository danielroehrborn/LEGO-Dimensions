#ifndef __TEA.H__
#define __TEA.H__

#include <stdint.h>

void encrypt (uint32_t* v, uint32_t* k);
void decrypt (uint32_t* v, uint32_t* k);
void encryptBlock(uint8_t * data, uint32_t * key);
void decryptBlock(uint8_t * data, uint32_t * key);
#endif
#ifndef ROTR
# define ROTR(x,n) (((uintmax_t)(x) >> (n)) | ((uintmax_t)(x) << ((sizeof(x) * 8) - (n))))
#endif
#ifndef MIN
# define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef BSWAP_32
#define BSWAP_32(x) \
  ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >>  8) | \
   (((x) & 0x0000ff00) <<  8) | (((x) & 0x000000ff) << 24))
#endif

#ifndef BSWAP_16
#define BSWAP_16(x) \
  (((x) & 0xff00) >> 8) | (((x) & 0x00ff) << 8)
#endif
