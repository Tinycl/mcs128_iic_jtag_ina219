#include "tiny_sci.h"
/******************ps0/ps1 disable interrupt***********************/
void SCI0_Init(void) 
{         
  SCI0CR1 = 0x00;                      //设置SCI0为正常模式，八位数据位，无奇偶校验
  SCI0CR2 = 0x0c;                      //允许发送数据，禁止中断功能   
}

void SCI0_SendChar(unsigned char data) 
{
  
  while(!SCI0SR1_TDRE);
  SCI0DRH=0;               
  SCI0DRL = data;
   
}

void SCI0_SendString(unsigned char *putchar) 
{
  while(*putchar!='\0')                
  {
    SCI0_SendChar(*putchar++);  
  }
}

unsigned char SCI0_ReceiveChar(void) 
{
  while(!SCI0SR1_RDRF);              
  return(SCI0DRL);
} 



/******************ps2/ps3 disable interrupt***********************/

void SCI1_Init(void) 
{         
  SCI1CR1 = 0x00;                      
  SCI1CR2 = 0x0c;                     
}


void SCI1_SendChar(unsigned char data) 
{
    while(!SCI1SR1_TDRE);
    SCI1DRH=0;               
    SCI1DRL = data;
}

void SCI1_SendString(unsigned char *putchar) 
{
  while(*putchar!='\0')                
  {
    SCI1_SendChar(*putchar++);  
  }
}

unsigned char SCI1_ReceiveChar(void) 
{
  while(!SCI1SR1_RDRF);               
  return(SCI1DRL);
} 

/*****************************************/

#pragma MESSAGE DISABLE C12056
void SCI1_SendDec(long data) 
{
  if(data == 0) 
  {
    SCI1_SendChar('0');
    return;
  }
  if(data < 0) 
  {
     SCI1_SendChar('-');
     data = 0 - data;
  }
  if(data < 10) 
  {
     SCI1_SendChar(data + '0');
  } 
  else if(data >= 10 && data < 99) 
  {
     SCI1_SendChar(data/10%10 + '0');
     SCI1_SendChar(data%10 + '0');
  } 
  else if(data >= 100 && data < 999)
  {
     SCI1_SendChar(data/100%10 + '0');
     SCI1_SendChar(data/10%10 + '0');
     SCI1_SendChar(data%10 + '0');
  } 
  else if(data >= 1000 && data < 9999) 
  {
     SCI1_SendChar(data/1000%10 + '0');
     SCI1_SendChar(data/100%10 + '0');
     SCI1_SendChar(data/10%10 + '0');
     SCI1_SendChar(data%10 + '0');
  }
  else if(data >= 10000 && data < 99999) 
  {
     SCI1_SendChar(data/10000%10 + '0');
     SCI1_SendChar(data/1000%10 + '0');
     SCI1_SendChar(data/100%10 + '0');
     SCI1_SendChar(data/10%10 + '0');
     SCI1_SendChar(data%10 + '0');
  }
  else if(data >= 100000 && data < 999999) 
  {
     SCI1_SendChar(data/100000%10 + '0');
     SCI1_SendChar(data/10000%10 + '0');
     SCI1_SendChar(data/1000%10 + '0');
     SCI1_SendChar(data/100%10 + '0');
     SCI1_SendChar(data/10%10 + '0');
     SCI1_SendChar(data%10 + '0');
  }
   else if(data >= 1000000 && data < 9999999) 
  {
     SCI1_SendChar(data/1000000%10 + '0');
     SCI1_SendChar(data/100000%10 + '0');
     SCI1_SendChar(data/10000%10 + '0');
     SCI1_SendChar(data/1000%10 + '0');
     SCI1_SendChar(data/100%10 + '0');
     SCI1_SendChar(data/10%10 + '0');
     SCI1_SendChar(data%10 + '0');
  } 
  else 
  {
     SCI1_SendChar('e');
     SCI1_SendChar('r');
     SCI1_SendChar('r');
     SCI1_SendChar('o');
     SCI1_SendChar('r');
  }
}