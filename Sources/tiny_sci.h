#ifndef _TINY_SCI_H_
#define _TINY_SCI_H_
#include <mc9s12dg128.h>

#define BUS_CLOCK_16M  16000000
#define BUS_CLOCK_24M  24000000
#define BUS_CLOCK_32M  32000000

#define BAUD_RATE_9600   9600
#define BAUD_RATE_19200  19200
#define BAUD_RATE_115200 115200

#define  EnableSCI0_BUS16M_BAUD_RATE_9600 (SCI0BD = BUS_CLOCK_16M/16/BAUD_RATE_9600);
#define  EnableSCI0_BUS16M_BAUD_RATE_19200 (SCI0BD = BUS_CLOCK_16M/16/BAUD_RATE_19200)
#define  EnableSCI0_BUS16M_BAUD_RATE_115200 (SCI0BD = BUS_CLOCK_16M/16/BAUD_RATE_115200);
#define  EnableSCI0_BUS24M_BAUD_RATE_9600 (SCI0BD = BUS_CLOCK_24M/16/BAUD_RATE_9600);
#define  EnableSCI0_BUS24M_BAUD_RATE_19200 (SCI0BD = BUS_CLOCK_24M/16/BAUD_RATE_19200);
#define  EnableSCI0_BUS24M_BAUD_RATE_115200 (SCI0BD = BUS_CLOCK_24M/16/BAUD_RATE_115200);
#define  EnableSCI0_BUS32M_BAUD_RATE_9600 (SCI0BD = BUS_CLOCK_32M/16/BAUD_RATE_9600);
#define  EnableSCI0_BUS32M_BAUD_RATE_19200 (SCI0BD = BUS_CLOCK_32M/16/BAUD_RATE_19200);
#define  EnableSCI0_BUS32M_BAUD_RATE_115200 (SCI0BD = BUS_CLOCK_32M/16/BAUD_RATE_115200);

#define  EnableSCI1_BUS16M_BAUD_RATE_9600 (SCI1BD = BUS_CLOCK_16M/16/BAUD_RATE_9600);
#define  EnableSCI1_BUS16M_BAUD_RATE_19200 (SCI1BD = BUS_CLOCK_16M/16/BAUD_RATE_19200);
#define  EnableSCI1_BUS16M_BAUD_RATE_115200 (SCI1BD = BUS_CLOCK_16M/16/BAUD_RATE_115200);
#define  EnableSCI1_BUS24M_BAUD_RATE_9600 (SCI1BD = BUS_CLOCK_24M/16/BAUD_RATE_9600);
#define  EnableSCI1_BUS24M_BAUD_RATE_19200 (SCI1BD = BUS_CLOCK_24M/16/BAUD_RATE_19200);
#define  EnableSCI1_BUS24M_BAUD_RATE_115200 (SCI1BD = BUS_CLOCK_24M/16/BAUD_RATE_115200);
#define  EnableSCI1_BUS32M_BAUD_RATE_9600 (SCI1BD = BUS_CLOCK_32M/16/BAUD_RATE_9600);
#define  EnableSCI1_BUS32M_BAUD_RATE_19200 (SCI1BD = BUS_CLOCK_32M/16/BAUD_RATE_19200);
#define  EnableSCI1_BUS32M_BAUD_RATE_115200 (SCI1BD = BUS_CLOCK_32M/16/BAUD_RATE_115200);

#define EnableSCI0ReInterrupt  SCI0CR2 |= 0x20;     
#define DisableSCI0ReInterrupt SCI0CR2 &= 0xDF;
 
#define EnableSCI1ReInterrupt  SCI1CR2 |= 0x20;     
#define DisableSCI1ReInterrupt SCI1CR2 &= 0xDF;
  

void SCI0_Init(void);
void SCI0_SendChar(unsigned char data);
void SCI0_SendString(unsigned char *putchar);
unsigned char SCI0_ReceiveChar(void);


void SCI1_Init(void);
void SCI1_SendChar(unsigned char data);
void SCI1_SendString(unsigned char *putchar);
unsigned char SCI1_ReceiveChar(void);

void SCI1_SendDec(long data);

#endif