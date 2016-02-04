#include "tea.h"

/* encryptBlock
 *   Encrypts byte array data of length len with key key using TEA
 * Arguments:
 *   data - pointer to 8 bit data array to be encrypted - SEE NOTES
 *   len - length of array
 *   key - Pointer to four integer array (16 bytes) holding TEA key
 * Returns:
 *   data - encrypted data held here
 *   len - size of the new data array
 * Side effects:
 *   Modifies data and len
 * NOTES:
 * data size must be equal to or larger than ((len + 7) / 8) * 8 + 8
 * TEA encrypts in 8 byte blocks, so it must include enough space to
 * hold the entire data to pad out to an 8 byte boundary, plus another
 * 8 bytes at the end to give the length to the decrypt algorithm.
 *
 *  - Shortcut - make sure that data is at least len + 15 bytes in size.
 */
void encryptBlock(uint8_t * data, uint32_t * key)
{
   uint32_t i,blocks;
   uint32_t* data32;
   data32 = (uint32_t *) data;
  flipBytes(data32);
   for(i = 0; i< 2; i++)
   {

      encrypt(&data32[i*2], key);
   }
}

/* decryptBlock
 *   Decrypts byte array data of length len with key key using TEA
 * Arguments:
 *   data - pointer to 8 bit data array to be decrypted - SEE NOTES
 *   len - length of array
 *   key - Pointer to four integer array (16 bytes) holding TEA key
 * Returns:
 *   data - decrypted data held here
 *   len - size of the new data array
 * Side effects:
 *   Modifies data and len
 * NOTES:
 *   None
 */
void decryptBlock(uint8_t * data, uint32_t * key)
{
   uint32_t i,blocks;
   uint32_t* data32;
   data32 = (uint32_t *) data;
  flipBytes(data32);
   for(i = 0; i< 2; i++)
   {
      decrypt(&data32[i*2], key);
   }
}

/* encrypt
 *   Encrypt 64 bits with a 128 bit key using TEA
 *   From http://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm
 * Arguments:
 *   v - array of two 32 bit uints to be encoded in place
 *   k - array of four 32 bit uints to act as key
 * Returns:
 *   v - encrypted result
 * Side effects:
 *   None
 */
void encrypt (uint32_t* v, uint32_t* k)
{
    /* set up */
    uint32_t v0 = v[0];
    uint32_t v1 = v[1];
    uint32_t sum = 0;
    uint32_t i;

    /* a key schedule constant */
    uint32_t delta = 0x9e3779b9;

    /* cache key */
    uint32_t k0 = k[0];
    uint32_t k1 = k[1];
    uint32_t k2 = k[2];
    uint32_t k3 = k[3];

    /* basic cycle start */
    for (i = 0; i < 32; i++)
    {
        sum += delta;
        v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
    }
    /* end cycle */

    v[0] = v0;
    v[1] = v1;
}

void decrypt (uint32_t* v, uint32_t* k)
{
    /* set up */
    uint32_t v0 = v[0];
    uint32_t v1 = v[1];
    uint32_t sum = 0xC6EF3720;
    uint32_t i;

    /* a key schedule constant */
    uint32_t delta = 0x9e3779b9;

    /* cache key */
    uint32_t k0 = k[0];
    uint32_t k1 = k[1];
    uint32_t k2 = k[2];
    uint32_t k3 = k[3];

    /* basic cycle start */
    for (i = 0; i < 32; i++)
    {
        v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
        v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
        sum -= delta;
    }
    /* end cycle */

    v[0] = v0;
    v[1] = v1;
}


