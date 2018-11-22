/*****************hard ware iic without ack******************************/
#ifndef __TINY_HW_IIC_H__
#define __TINY_HW_IIC_H__
#include <mc9s12dg128.h>

void tiny_hwiic_Init(void);   
unsigned char tiny_hw_iic_Readbyte(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char *Data);
unsigned char tiny_hw_iic_Writebyte(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data);
unsigned char tiny_hw_iic_Readbytes(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data[], unsigned char N);
unsigned char tiny_hw_iic_Writebytes(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data[], unsigned char N);

#endif


