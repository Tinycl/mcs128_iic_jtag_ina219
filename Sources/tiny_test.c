#include <mc9s12dg128.h>
#include "tiny_test.h"
#include "tiny_sci.h"

void Bus_128k_Delay_1ms(void)               
{
  unsigned int i;
  for(i=0;i<128;i++) {
         _asm(nop);
  }
}

void Bus16M_Delay_1ms(void)               
{
  unsigned int i,j;
  for(i=0;i<16;i++) {
      for(j=0;j<1000;j++) {
        
         _asm(nop);
      }
  }
}

void Bus24M_Delay_1ms(void)               
{
  unsigned int i,j;
  for(i=0;i<24;i++) {
      for(j=0;j<1000;j++){
         _asm(nop);
      }
  }
}

void Bus32M_Delay_1ms(void)               
{
  unsigned int i,j;
  for(i=0;i<32;i++) {
      for(j=0;j<1000;j++){
         _asm(nop);
      }
  }
}

#pragma MESSAGE DISABLE C12056
void this_tool_state(void) 
{
   unsigned char showstring[]={'T','h','i','s',' ','s','o','t','w','a','r','e',' ','u','s','e',' ','f','o','r',' ',
   'z','x','c',' ','c','p','u',' ','t','e','s','t',' ','b','y',' ','Z','h','a','o','X','i','n','!','\0'};
   SCI1_SendString(showstring);
   SCI1_SendChar(0x0D);
   SCI1_SendChar(0X0A);
}

void SendHex(unsigned char value) 
{
  unsigned char temp;
  temp = value >> 4;
  if(temp < 10) 
  {
   SCI1_SendChar(temp + '0');
  } 
  else 
  {
    SCI1_SendChar(temp - 10 + 'A');
  }
  temp = value & 0x0F;
  if(temp < 10) 
  {
    SCI1_SendChar(temp + '0');
  } 
  else 
  {
    SCI1_SendChar(temp - 10 + 'A');
  }
}