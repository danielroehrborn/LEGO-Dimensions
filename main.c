#include <stdio.h>
#include <stdlib.h>
#include "tea.h"
#include "LEGOchar.h"



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
