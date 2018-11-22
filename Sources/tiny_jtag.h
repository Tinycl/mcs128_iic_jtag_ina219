#ifndef __TINY_JTAG_H__
#define __TINY_JTAG_H__
#include <mc9s12dg128.h>

//define pin
#define TRST_M  PORTA_BIT1
#define TMS_M   PORTA_BIT2
#define TCK_M   PORTA_BIT3
#define TDI_M   PORTA_BIT4
#define TDO_M   PORTAD0_PTAD2

//define jtag signal
#define JTAG_0  1      // JTAG signals are inverted  tck_m 1, tck 0
#define JTAG_1  0

#define JTAG_RST_0  0   // TRST is not inverted
#define JTAG_RST_1  1   // TRST is not inverted


enum JTAG_STATE
{
  Undefined = 0,
  Test_Logic_Reset,

  Select_DR_Scan,
  Capture_DR,
  Shift_DR,
  Pause_DR,
  Shift_IR,
  Pause_IR,
  Capture_IR,

  Select_IR_Scan,
  Exit1_DR,
  Exit2_DR,
  Exit1_IR,
  Exit2_IR,

  Update_DR,
  Update_IR,
  Idle
};

static unsigned int jtag_bits_remaining = 0;



void jtag_mcu_io_init(void);
void jtag_clk(void);
void jtag_tms0_clk(void);
void jtag_tms1_clk(void);
void jtag_goto_Test_Logic_Reset(void);
void jtag_goto_Idle(void);
void jtag_goto_Select_DR_Scan(void);
void jtag_goto_Shift_DR(void);
void jtag_goto_Shift_IR(void);
unsigned char jtag_scan_one_bit(unsigned char *byte_in, unsigned char byte_out);
void jtag_scan_bits(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData, unsigned char byte_idx);
void jtag_scan_bits_start(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData);
void jtag_scan_bits_cont(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData);
void jtag_IR(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData);
void jtag_DR(unsigned char *RcvData, unsigned char RcvLen, unsigned char *XmtData);

void tiny_jtag_init(void);
void tiny_jtag_reset(void);
unsigned int tiny_jtag_ir_scan(unsigned int instruction, int num_bits);
unsigned long tiny_jtag_dr_scan(unsigned long data, int num_bits);

#endif