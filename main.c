#include <stdio.h>
#include <stdlib.h>
#include "tea.h"
#include "LEGOchar.h"
uint32_t pwdgen(uint8_t* uid){
	uint32_t pwd = 0;
	uint8_t base[] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x28,
		0x63, 0x29, 0x20, 0x43, 0x6f, 0x70, 0x79, 0x72,
		0x69, 0x67, 0x68, 0x74, 0x20, 0x4c, 0x45, 0x47,
		0x4f, 0x20, 0x32, 0x30, 0x31, 0x34, 0xaa, 0xaa
	};

	memcpy(base, uid, 7);

	for (int i = 0; i < 32; i += 4) {
		uint32_t b = *(uint32_t *)(base + i);
		pwd = b + ROTR(pwd, 25) + ROTR(pwd, 10) - pwd;
	}
	return BSWAP_32(pwd);
}


int main()
{
    uint32_t data[2];
    uint8_t charid;
    //UID from NTAG
    uint8_t uid[7]={0x04,0x0b,0x49,0x22,0xa3,0x48,0x81};
    //USER ID 
    charid=42;



encr(uid,charid,&data);
printf("Block 36-37 %x-%x\n",data[0],data[1]);
printf("CharID %x",decr(uid, data));

    printf("\n");
    return 0;
}
