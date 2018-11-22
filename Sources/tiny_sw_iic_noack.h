/********************soft ware iic with out ack ******************************/
#ifndef __TINY_SW_IIC_NO_ACK_H__
#define __TINY_SW_IIC_NO_ACK_H__
#include <hidef.h>     
#include "derivative.h"

/******************************************************/
/**********************  IIC–≠“È  *********************/
/******************************************************/

#define SDA               PTJ_PTJ6
#define SCL               PTJ_PTJ7
#define SDA_Out           DDRJ_DDRJ6 = 1;
#define SDA_In            DDRJ_DDRJ6 = 0;
#define SCL_Out           DDRJ_DDRJ7 = 1;


void IIC_PJ_init(void);
void nops(void);
void delay_us(unsigned int time);
void IIC_Start(void);
void IIC_Stop(void);
void Send_Byte(unsigned char SendChar);
void Send_Bytes(unsigned char *data, unsigned char len);
unsigned char Read_Byte(void);
void Read_Bytes(unsigned char *data, unsigned char len);
unsigned int Read_Word(void);
unsigned char MMA8451_ReadByte(unsigned char Address);
void MMA8451_WriteByte(unsigned char Address, unsigned char Thedata);
void MMA8451_Init(void);
#endif