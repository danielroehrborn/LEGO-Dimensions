#include "LEGOchar.h"

void flipBytes(uint32_t* n) {
  uint8_t i = 0;
  for (i = 0; i < sizeof(n); i++) {
    n[i] = BSWAP_32(n[i]);
  }
}

void pwdgen(uint8_t* uid,uint8_t* PWDOUT) {
  int i;
  uint32_t b;
  uint32_t pwd = 0;
  uint32_t pwd1[2];
  uint8_t base[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x28,
    0x63, 0x29, 0x20, 0x43, 0x6f, 0x70, 0x79, 0x72,
    0x69, 0x67, 0x68, 0x74, 0x20, 0x4c, 0x45, 0x47,
    0x4f, 0x20, 0x32, 0x30, 0x31, 0x34, 0xaa, 0xaa
  };
  memcpy(base, uid, 7);
  for (i = 0; i < 32; i += 4) {
    b = *(uint32_t *)(base + i);
    pwd = b + ROTR(pwd, 25) + ROTR(pwd, 10) - pwd;
  }
  //pwd1[0]=BSWAP_32(pwd);
    pwd1[0]=(pwd);
   memcpy(PWDOUT,(uint8_t*)(pwd1),4);
//   return pwd;
}

uint32_t scramble(uint8_t* uid, uint8_t cnt) {
  uint8_t base[24] = {0xFF, 0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF, 0xb7,	0xd5, 0xd7, 0xe6, 0xe7, 0xba, 0x3c, 0xa8, 0xd8, 0x75, 0x47, 0x68, 0xcf, 0x23, 0xe9, 0xfe, 0xaa};
  uint32_t v2 = 0, b = 0;
  uint8_t i = 0;
  memcpy(base, uid, 7);
  base[(cnt * 4) - 1] = 0xaa;
  for (i = 0; i < cnt; i++) {
    b = *(uint32_t *)(base + (i * 4));
    v2 = b + ROTR(v2, 25) + ROTR(v2, 10) - v2;
  }
  return BSWAP_32(v2);
}
void genkey(uint8_t* uid, uint32_t* key) {
  key[0] = scramble(uid, 3);
  key[1] = scramble(uid, 4);
  key[2] = scramble(uid, 5);
  key[3] = scramble(uid, 6);
  flipBytes(key);
}

uint8_t  decryptID(uint8_t* uid, uint8_t* data) {
  uint32_t key[4];
  uint32_t* data32;
  data32=(uint32_t*) data;
  genkey(uid, key);
  decryptBlock(data32, key);
  flipBytes(data32);
  return (BSWAP_32(data32[0]) & 0xff);
}

void encryptID(uint8_t* uid, uint8_t charid, uint8_t* data) {
  uint32_t key[4], buf[2];
  uint32_t * data32;
  data32=(uint32_t*) data;
  genkey(uid, key);
  buf[0] = charid;
  buf[1] = charid;
  flipBytes(buf);
  encryptBlock(buf, key);
  //flipBytes(buf);
  memcpy(data32, buf, 16);
}
