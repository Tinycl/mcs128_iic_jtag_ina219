#include "tiny_rti.h"

void RtiInit(void) 
{
   RTICTL = 0x1F; /* set RTI prescaler */ 
   CRGINT = 0x80; /* enable RTI interrupts */ 
}

#pragma MESSAGE DISABLE C5703
/*  
  DESCRIPTION: wait the specified # of MCU clocks (does not return
                 until time expires)
  The delay will be a minimum of 12 clocks and a maximum of 32767 clocks.
  The delay has a useful increment of 5 clocks (the execution time of the
  inner loop).
  RETURNS: none
  ORIGINATED: 10/10/07 Miner
*/
void delay_mcu_clocks (unsigned int clks) 
{
#asm
  SUBD  #4+2+5-2                ;[2] Adjust for JSR, SUBD, RTS execution times
delay_mcu_loop:                 ;    "-2" adjustment is for 3/1 BPL not taken at end
  SUBD  #2+3                    ;[2] time for SUBD, BPL
  BPL   delay_mcu_loop          ;[3/1]
#endasm
  return;
}