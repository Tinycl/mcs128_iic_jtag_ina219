/******************soft ware iic with ack********************************/
#ifndef _TINY_SW_IIC_H_
#define _TINY_SW_IIC_H_
#include <hidef.h>     
#include "derivative.h"

#define SDA               PTJ_PTJ6
#define SCL               PTJ_PTJ7
#define SDA_Out           DDRJ_DDRJ6 = 1;
#define SDA_In            DDRJ_DDRJ6 = 0;
#define SCL_Out           DDRJ_DDRJ7 = 1;

#define ACK 0x00
#define NACK 0x01

void nops(unsigned char count);
void delay_us(unsigned int time);
void tiny_iic_sw_start(void);
void tiny_iic_sw_stop(void);
void tiny_iic_sw_sendbyte(unsigned char SendChar);
unsigned char tiny_iic_sw_readbyte(void);
unsigned char tiny_iic_sw_receiveACK(void);
void tiny_iic_sw_sendACK(unsigned char ack);

#endif