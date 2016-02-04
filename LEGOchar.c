#include "LEGOchar.h"

void flipBytes(uint32_t* n){
uint8_t i=0;
    for(i = 0; i<sizeof(n); i++){
            n[i]=BSWAP_32(n[i]);
        }

}

	uint32_t scramble(uint8_t* uid,uint8_t cnt){
		uint8_t base[24] = {0xFF, 0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF, 0xb7,	0xd5, 0xd7, 0xe6, 0xe7,0xba, 0x3c, 0xa8, 0xd8,0x75, 0x47, 0x68, 0xcf,0x23, 0xe9, 0xfe, 0xaa};
		uint32_t v2 = 0,b=0;
		uint8_t i=0;
	 	memcpy(base, uid, 7);
		base[(cnt * 4) - 1] = 0xaa;

        for (i = 0; i < cnt; i++) {
		b = *(uint32_t *)(base + (i*4));
		v2 = b + ROTR(v2, 25) + ROTR(v2, 10) - v2;
	}
	return BSWAP_32(v2);
	}
	void genkey(uint8_t* uid,uint32_t* key){
		key[0] = scramble(uid,3);
		key[1] =scramble(uid,4);
		key[2] =scramble(uid,5);
		key[3] =scramble(uid,6);
		printf("gen key: %02x-%x-%x-%x\n",key[0],key[1],key[2],key[3]);
		flipBytes(key);
		printf("gen key: %02x-%x-%x-%x\n",key[0],key[1],key[2],key[3]);
	}
uint8_t  decr(uint8_t* uid,uint32_t* data){
        uint32_t key[4];
        genkey(uid,key);
		decryptBlock(data,key);
        printf("decr %x-%x\n",data[0],data[1]);
        flipBytes(data);
		return (BSWAP_32(data[0]) & 0xff);
	}
void encr(uint8_t* uid,uint8_t charid,uint32_t* data){
        uint32_t key[4],buf[2];
        genkey(uid,key);

        buf[0]=charid;
        buf[1]=charid;
        flipBytes(buf);
        printf("befor enc out:%x-%x\n",buf[0],buf[1],buf[2]);
        encryptBlock(buf,key);
        flipBytes(buf);
        printf("enc out:%x-%x\n",buf[0],buf[1]);
        memcpy(data,buf,16);
}
