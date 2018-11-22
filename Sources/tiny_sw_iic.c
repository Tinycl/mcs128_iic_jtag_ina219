#include "tiny_sw_iic.h"

void nops(unsigned char count) 
{
  unsigned char i = 0;
  for(i = 0; i < count; i++) 
  {
     _asm nop;
  }
} 
 
void delay_us(unsigned int time)
{
    unsigned int i,j;
    for(i=time;i>0;i--)
        for(j=16;j>0;j--);
}

void tiny_iic_sw_start(void)
{
  SCL_Out;
  SDA_Out;
                          
	SDA=1;
	SCL=1;
	nops(2);
	SDA=0;
	nops(2);
	SCL=0;
}


void tiny_iic_sw_stop(void)
{
  SCL_Out;
  SDA_Out;
                        
	SCL=0;
	SDA=0;
  nops(2);
	SCL=1;
	nops(2);
	SDA=1;
}


void tiny_iic_sw_sendbyte(unsigned char SendChar)
{
	unsigned char i; 
	SCL_Out;
	SDA_Out;
	
	SCL = 0;
	SDA = 0;
	for(i=0;i<8;i++)
	{
		if(SendChar & 0x80){
		  
		  SDA=1; 
		}
	  else{
	    
	    SDA=0;
	  }
	  SendChar <<= 1;
		nops(2);
		SCL=1;
		nops(2);
		SCL=0;
	}
  SDA = 0; 
}


unsigned char tiny_iic_sw_readbyte(void)
{
  unsigned char i;
  unsigned char ReadChar;
	ReadChar=0;

  SDA=1;
	SDA_In;                           
	SCL_Out;
	SCL=0;                      
	for(i=0;i<8;i++)
	{
		ReadChar<<=1;
		if(SDA) {
	  	ReadChar++;                   
		}
		nops(2);
		SCL =1;
		nops(2);
		SCL= 0;
		
	}
	SDA = 0;
	SDA_Out;
	return ReadChar;
}

unsigned char tiny_iic_sw_receiveACK(void) 
{
   unsigned char ack;
   SCL_Out;
	 SDA_Out;
	 
   SDA = 1;
   SDA_In;
   nops(2);
   SCL = 1;
   nops(2);
   ack = SDA;  //IN
   SCL = 0;
   SDA = 0; 
   SDA_Out;
   return ack;
}


void tiny_iic_sw_sendACK(unsigned char ack)
{
   SDA_Out;
   SCL_Out;
   SCL = 0;
   if(ack ==ACK) {
    SDA = 0;
   } 
   else {
    SDA = 1;
   }
   nops(2);
   SCL = 1;
   nops(2);
   SCL = 0;  
}
