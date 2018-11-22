#include "tiny_jtag.h"

#define TCKMAX   60
enum JTAG_STATE jtag_current_state = Undefined;

//jtag for mcu init
void jtag_mcu_io_init(void) 
{
   PORTA_BIT1 = 0;
   PORTA_BIT2 = 0;
   PORTA_BIT3 = 0;
   PORTA_BIT4 = 0;
   
   TRST_M = 1; //set TRST high to put a low on the micro  // high Q open, trst-cpu = 0
   TCK_M = 1;  //set TCK high to float pin to the DUT
   
   DDRA_BIT1 = 1;
   DDRA_BIT2 = 1;
   DDRA_BIT3 = 1;
   DDRA_BIT4 = 1;
  
   ATD0DIEN_IEN2 = 1;//set digital input
         PORTAD0_PTAD2=0;
}


/****************************************************************************/
/*  DESCRIPTION: Send one clock to JTAG */
/*****************************************************************************/
void jtag_clk(void)
{
  unsigned char i = 0;
  for(i = 0; i < TCKMAX; i++) {
      TCK_M = JTAG_1;     /* Drive TCK high on the JTAG bus */
  }
    
  for(i = 0; i < TCKMAX; i++) {
      TCK_M = JTAG_0;     /* Drive TCK low on the JTAG bus */ 
  }

}


/****************************************************************************/
/*DESCRIPTION: Set TMS = 0, then send one clock to JTAG */
/*****************************************************************************/
void jtag_tms0_clk(void)
{
  unsigned char i = 0;
  for(i = 0; i < TCKMAX; i++){
    TMS_M = JTAG_0;     
    TMS_M = JTAG_0;     
    TMS_M = JTAG_0;    
    TMS_M = JTAG_0;
  }
  

  for(i = 0; i < TCKMAX; i++) {
      TCK_M = JTAG_1;     /* Drive TCK high on the JTAG bus */
  }
    
  for(i = 0; i < TCKMAX; i++) {
      TCK_M = JTAG_0;     /* Drive TCK low on the JTAG bus */ 
  } 
}


/****************************************************************************/
/*  DESCRIPTION: Set TMS = 1, then send one clock to JTAG 
	
*****************************************************************************/
void jtag_tms1_clk(void)
{
  unsigned char i = 0;
  for(i = 0; i < TCKMAX; i++){
      TMS_M = JTAG_1;    
      TMS_M = JTAG_1;     
      TMS_M = JTAG_1;     
      TMS_M = JTAG_1;
  }
 
  
  for(i = 0; i < TCKMAX; i++) {
      TCK_M = JTAG_1;     /* Drive TCK high on the JTAG bus */
  }
    
  for(i = 0; i < TCKMAX; i++) {
      TCK_M = JTAG_0;     /* Drive TCK low on the JTAG bus */ 
  } 
}


/****************************************************************************/
/*  DESCRIPTION: Force JTAG into the reset state

*****************************************************************************/
void jtag_goto_Test_Logic_Reset(void)
{
  TRST_M = JTAG_RST_0;   /* Drive TRST low */
  jtag_tms1_clk();
  TRST_M = JTAG_RST_1;   /* Drive TRST high */
  jtag_clk();
  TRST_M = JTAG_RST_0; 
  jtag_clk();          /* Above should be enough - do these 5 clocks to make certain. */
  jtag_clk();
  jtag_clk();
  jtag_clk();
  jtag_clk();

  jtag_current_state = Test_Logic_Reset;
  return;
}


/****************************************************************************/
/*  DESCRIPTION: Moves the JTAG state machine from the current state to Idle
*****************************************************************************/
void jtag_goto_Idle(void)
{
  switch (jtag_current_state)
  {
  case Select_DR_Scan:
  case Capture_DR:
  case Shift_DR:
  case Pause_DR:
  case Capture_IR:
  case Shift_IR:
  case Pause_IR:
    jtag_tms1_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Select_IR_Scan:
  case Exit1_DR:
  case Exit2_DR:
  case Exit1_IR:
  case Exit2_IR:
    jtag_tms1_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Test_Logic_Reset:
  case Update_DR:
  case Update_IR:
    jtag_tms0_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Idle:            /* Already in Idle, nothing to do */
    break;

  default:              /* safety - should never get here. */
    jtag_goto_Test_Logic_Reset ();
    jtag_tms0_clk ();
    break;
  }
  
  jtag_current_state = Idle;
  return;
}


/****************************************************************************/
/*  DESCRIPTION: Moves the JTAG state machine from the current state to Select_DR_Scan
*****************************************************************************/
void jtag_goto_Select_DR_Scan(void)
{
  switch (jtag_current_state)
  {
  case Capture_DR:
  case Shift_DR:
  case Pause_DR:
  case Capture_IR:
  case Shift_IR:
  case Pause_IR:
    jtag_tms1_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Select_IR_Scan:
  case Exit1_DR:
  case Exit2_DR:
  case Exit1_IR:
  case Exit2_IR:
    jtag_tms1_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Idle:          
  case Update_DR:
  case Update_IR:
    jtag_tms1_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Select_DR_Scan:  /* Already in Select_DR_Scan, nothing to do */
    break;

  default:              /* safety - should never get here. */
   jtag_goto_Test_Logic_Reset ();
    /* do NOT "break" here - instead fall through to next case: */

  case Test_Logic_Reset:
    jtag_tms0_clk ();
    jtag_tms1_clk ();
    break;
  }

  jtag_current_state = Select_DR_Scan;
  return;
}


/****************************************************************************/
/*  DESCRIPTION: Moves the JTAG state machine from the current state to Shift_DR
*****************************************************************************/
void jtag_goto_Shift_DR(void)
{
  switch (jtag_current_state)
  {
  default:              /* all other cases */
    jtag_goto_Select_DR_Scan ();
    /* do NOT "break" here - instead fall through to next case: */

  case Select_DR_Scan:
    jtag_tms0_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Capture_DR:
    jtag_tms0_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Shift_DR:        /* Already in Shift_DR, nothing to do */
    break;
  }
  
  jtag_current_state = Shift_DR;
  return;
}


/****************************************************************************/
/*  DESCRIPTION: Moves the JTAG state machine from the current state to Shift_IR
*****************************************************************************/
void jtag_goto_Shift_IR(void)
{
  switch (jtag_current_state)
  {
  default:              /* all other cases */
    jtag_goto_Select_DR_Scan ();
    /* do NOT "break" here - instead fall through to next case: */

  case Select_DR_Scan:
    jtag_tms1_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Select_IR_Scan:
    jtag_tms0_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Capture_IR:
    jtag_tms0_clk ();
    /* do NOT "break" here - instead fall through to next case: */

  case Shift_IR:        /* Already in Shift_IR, nothing to do */
    break;
  }
  
  jtag_current_state = Shift_IR;
  return;
}

/****************************************************************************/
/*  DESCRIPTION: Shifts one bit to/from the JTAG
*****************************************************************************/
unsigned char jtag_scan_one_bit(unsigned char *byte_in, unsigned char byte_out)
{
  unsigned char i = 0;
  if(*byte_in & 1)
  { 
    for(i = 0; i < TCKMAX; i++) {
      TDI_M = JTAG_1;   /* input bit is a 1 so set TDI high on the bus */
    }
  } 
  else 
  { 
    for(i = 0; i < TCKMAX; i++) {
       TDI_M = JTAG_0;   /* input bit is a 0 so set TDI low on the bus */
    }
  }
  *byte_in >>= 1;
  byte_out >>= 1;
  byte_out &= 0x7F;

  if (TDO_M == JTAG_1) /* High on TDO_IN means it is high on the bus */
  { 
    byte_out |=  0x80;
  }

  jtag_clk ();
  return byte_out;
}


/****************************************************************************/
/*  DESCRIPTION: Shifts bits to/from the JTAG
*****************************************************************************/
void jtag_scan_bits(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData, unsigned char byte_idx)
{
  unsigned char bit_count;
  unsigned char scan_byte_rcv;
  unsigned char scan_byte_xmt;

  /* FIXME - Verify proper values for jtag_bits_remaining and RcvLen: */
  /*  if (jtag_bits_remaining <= 48)                             */
  /*     {  if ( (((RcvLen-byte_idx) * 8) < jtag_bits_remaining)        */
  /*          || (((RcvLen-byte_idx-1) * 8) >= jtag_bits_remaining) )     */
  /*           then error...   But - what to do??  return FALSE? */
  /*     }                                                       */
  /*     else // jtag_bits_remaining > 48                        */
  /*     {  if (RcvLen != 8)                                     */
  /*           then error...   But - what to do??  return FALSE? */
  /*     }                                                       */


  while (jtag_bits_remaining >= 1  &&  byte_idx < RcvLen)
  {
    scan_byte_rcv = RcvData[byte_idx];
    scan_byte_xmt = 0;
    for (bit_count = 0;  
         bit_count < 8  &&  jtag_bits_remaining > 1;  
         bit_count++, jtag_bits_remaining--)
    {
      scan_byte_xmt = jtag_scan_one_bit (&scan_byte_rcv, scan_byte_xmt);
    }
    if (bit_count >=8)
    {
      XmtData[byte_idx] = scan_byte_xmt;
      byte_idx++;
    }
    else  /* must have terminated because jtag_bits_remaining <= 1 */
    {
      bit_count++;
      jtag_bits_remaining--;
      /* Transition to Exit1_*R JTAG state. */
      TMS_M = JTAG_1;     /* set TMS high on the bus */
      TMS_M = JTAG_1;     /* extra time delay */
      TMS_M = JTAG_1;     /* extra time delay */
      scan_byte_xmt = jtag_scan_one_bit (&scan_byte_rcv, scan_byte_xmt);
      /* Shift value if required so it is right justified */
      XmtData[byte_idx] = scan_byte_xmt >> (8 - bit_count);

      jtag_current_state = Exit1_DR;    /*  == Exit1_IR  */
      jtag_goto_Idle ();
    }
  }
  return;
}


/****************************************************************************/
/*  DESCRIPTION: Shifts the first 48 (or less) bits to/from the JTAG
*****************************************************************************/
void jtag_scan_bits_start(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData)
{
  jtag_bits_remaining = ((unsigned int) RcvData[0] << 8) | RcvData[1];

  jtag_scan_bits (RcvData, RcvLen, XmtData, 2);
  return;
}


/****************************************************************************/
/*  DESCRIPTION: Shifts the remaining bits to/from the JTAG
*****************************************************************************/
void jtag_scan_bits_cont(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData)
{
  jtag_scan_bits (RcvData, RcvLen, XmtData, 0);
  return;
}


/****************************************************************************/
/*  DESCRIPTION: Shifts the remaining bits to/from the JTAG
	RETURNS: none
	ORIGINATED: 01/18/08, Dan Miner
*****************************************************************************/
void jtag_IR(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData)
{
  jtag_goto_Shift_IR();
  jtag_scan_bits_start (RcvData, RcvLen, XmtData);
}


/****************************************************************************/
/*  DESCRIPTION: Shifts the remaining bits to/from the JTAGr
*****************************************************************************/
void jtag_DR(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData)
{
  jtag_goto_Shift_DR();
  jtag_scan_bits_start (RcvData, RcvLen, XmtData);
}


/*****************************排除硬件设计因素**********************/

void tiny_jtag_init(void)
{
   PORTA_BIT1 = 0;
   PORTA_BIT2 = 0;
   PORTA_BIT3 = 0;
   PORTA_BIT4 = 0;
   
   TRST_M = 1; //set TRST high to put a low on the micro  // high Q open, trst-cpu = 0
   
   
   DDRA_BIT1 = 1;
   DDRA_BIT2 = 1;
   DDRA_BIT3 = 1;
   DDRA_BIT4 = 1;
  
   ATD0DIEN_IEN2 = 1;//set digital input
}
//发送选通脉冲，置高电平之后，在变为低电平
void tiny_jtag_strobetck(void)
{
   TCK_M = 1;
   TCK_M = 0;
}
//TMS保持高电平，向TCK发送5个选通脉冲，处于逻辑复位状态。 然后保持在run_test/idle状态
void tiny_jtag_reset(void)
{
   TMS_M = 1;
   tiny_jtag_strobetck();
   tiny_jtag_strobetck();
   tiny_jtag_strobetck();
   tiny_jtag_strobetck();
   tiny_jtag_strobetck();
   TMS_M = 0;
   tiny_jtag_strobetck();
}
/*将长度为num_bits 的指令instruction 装入jtag指令寄存器， 停在test/idle 状态， 返回值从IR读到的值 。假定jtag状态机从run_test/idle state开始运行*/
unsigned int tiny_jtag_ir_scan(unsigned int instruction, int num_bits) 
{
   unsigned int retval;  //指令读
   int i;                //ir 计数器
   retval = 0x0;
   
   TMS_M = 1;
   tiny_jtag_strobetck();  //selectdr
  
   TMS_M = 1;
   tiny_jtag_strobetck();  //selectir
   
   TMS_M = 0;              
   tiny_jtag_strobetck();  //captureir
   
   TMS_M = 0;
   tiny_jtag_strobetck();  //shiftir state
   for(i = 0; i < num_bits; i++)
   {
      TDI_M = (instruction & 0x01);
      instruction = instruction >> 1;         //移位ir lsb先移
      retval = retval >> 1;
      if(TDO_M)
      {
          retval |= (0x01 << (num_bits - 1));
      }
      if(i == (num_bits - 1)) 
      {
          TMS_M = 1;                        //转到 exit_ir状态
      }
      tiny_jtag_strobetck();
   }
   TMS_M = 1;
   tiny_jtag_strobetck();         //update_ir
   
   TMS_M = 0;
   tiny_jtag_strobetck(); //rti state
   
   return retval;    
}
/*将长度为num_bits的数据data移入数据寄存器，返回DR寄存器的数据。使状态机停在run_test/idle状态。假定jtag状态机从run_test/idle开始运行*/
unsigned long tiny_jtag_dr_scan(unsigned long data, int num_bits) 
{
   unsigned long retval;           //返回值
   int i;                           //计数器
   retval = 0x0L;
  
   TMS_M = 1;
   tiny_jtag_strobetck();    //selectdr
   
   TMS_M = 0;
   tiny_jtag_strobetck();    //capturedr
   
   TMS_M = 0;
   tiny_jtag_strobetck();   //shiftdr
   for(i = 0; i < num_bits; i++) 
   {
   
      TDI_M = (data & 0x01);          //移位dr lsb
      data = data >> 1;
      retval = retval >> 1;
      if(TDO_M) 
      {
         retval |= (0x01L << (num_bits - 1));
      }
      if(i == (num_bits -1)) 
      {
         TMS_M = 1;            //exit1 dr
      }
      tiny_jtag_strobetck();
   }
   TMS_M = 1;
   tiny_jtag_strobetck();   //updatedr
   
   TMS_M = 0;
   tiny_jtag_strobetck();  //rti
   return retval; 
}