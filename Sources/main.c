#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "s12_common.h"
#include "tiny_mcu_config.h"
#include "tiny_sci.h"
#include "tiny_global_var.h"
#include "tiny_rti.h"
#include "tiny_sw_iic.h"
//#include "hw_iic.h"
#include "tiny_ect.h"
#include "tiny_ina219.h"
#include "tiny_jtag.h"
#include "tiny_cpu_button.h"
#include "tiny_test.h"


unsigned long id = 0x0L;
unsigned char bytes_send[8];
unsigned char bytes_get[8];

unsigned int rti_iic_time = 0;
unsigned int read_ina219_time = 0;
unsigned char read_ina219_flag = 0;
unsigned int loopcount = 0;
unsigned long ecttimecount = 0; 
unsigned int global_ina219_config = 0;
unsigned char cmd_ina219_start_flag = 0;
unsigned char cmd_jtag_flag = 0;
unsigned char sci_ina219_start_flag = 0;
unsigned char sci_jtag_start_flag = 0;
unsigned int ctest = 0;
unsigned char g_AD_value[3] = {0};
unsigned char uart_test = 0;
INA219_DATA ina219_vccp_data;
INA219_DATA ina219_vcca_data;
INA219_DATA ina219_vsusa_data;
INA219_DATA ina219_vcc3_3_data;
INA219_DATA ina219_vcore_data;
INA219_DATA ina219_vdd_data;
INA219_DATA ina219_vtt_data;



//read all ina219 sensors all data
void read_all_ina219s_results(void)
{
   ina219_get_all_result(INA219_IIC_7BIT_ADDRESS_VCCP, &ina219_vccp_data);
   ina219_get_all_result(INA219_IIC_7BIT_ADDRESS_VCCA, &ina219_vcca_data);
   ina219_get_all_result(INA219_IIC_7BIT_ADDRESS_VSUSA, &ina219_vsusa_data);
   ina219_get_all_result(INA219_IIC_7BIT_ADDRESS_VCC3_3, &ina219_vcc3_3_data);
   ina219_get_all_result(INA219_IIC_7BIT_ADDRESS_VCORE, &ina219_vcore_data);
   ina219_get_all_result(INA219_IIC_7BIT_ADDRESS_VDD, &ina219_vdd_data);
   ina219_get_all_result(INA219_IIC_7BIT_ADDRESS_VTT, &ina219_vtt_data);
   
}
//通过 ina219 reg 0x00 default value判断ina219是否工作
#pragma MESSAGE DISABLE C12056
void verify_all_ina219s_is_work(void) 
{
    unsigned char res = 0;
    unsigned char showstring1[]={'m','c','u',' ','i','n','i','t', 's','u','c','c','e','s','s','!',' ' ,'s','e','n','s','o','r',' ','i','n','i','t',' ','s','u','c','c','e','s','s','!','\0'};
    unsigned char showstring2[]={'m','c','u',' ','i','n','i','t', 's','u','c','c','e','s','s','!',' ' ,'s','e','n','s','o','r',' ','i','n','i','t',' ','f','a','i','l','!','\0'};
    read_all_ina219s_results();
    if(ina219_vccp_data.ina219_config== global_ina219_config) {
      sci1_send_ina219_is_work(1); 
    } else {
      sci1_send_ina219_is_not_work(1);
      res++;
    }
    
    if(ina219_vcca_data.ina219_config== global_ina219_config) {
       sci1_send_ina219_is_work(2);
    } else {
       sci1_send_ina219_is_not_work(2);
       res++;
    }
    
    if( ina219_vsusa_data.ina219_config== global_ina219_config) {
       sci1_send_ina219_is_work(3);
    } else {
       sci1_send_ina219_is_not_work(3);
       res++;
    }
    
    if( ina219_vcc3_3_data.ina219_config== global_ina219_config) {
       sci1_send_ina219_is_work(4);
    } else {
       sci1_send_ina219_is_not_work(4);
       res++;
    }
    if(ina219_vcore_data.ina219_config == global_ina219_config) {
       sci1_send_ina219_is_work(5);
    } else {
       sci1_send_ina219_is_not_work(5);
       res++;
    }
    
    if(ina219_vdd_data.ina219_config == global_ina219_config) {
       sci1_send_ina219_is_work(6);
    } else {
       sci1_send_ina219_is_not_work(6);
       res++;
    }
    
    if(ina219_vtt_data.ina219_config == global_ina219_config) {
       sci1_send_ina219_is_work(7);
    } else {
       sci1_send_ina219_is_not_work(7);
       res++;
    } 
    if(res == 0)
    {
        SCI1_SendString(showstring1);
        SCI1_SendChar(0x0D);
        SCI1_SendChar(0X0A);
    } else 
    {
        SCI1_SendString(showstring2);
        SCI1_SendChar(0x0D);
        SCI1_SendChar(0X0A);
    }
}

void send_ina219_all_to_pc(void) 
{
    sci1_send_start_splitchar_to_pc();
    sci1_send_ina219_data_vccp_to_pc(0,ina219_vccp_data);
    sci1_send_ina219_data_vccp_to_pc(1,ina219_vcca_data);
    sci1_send_ina219_data_vccp_to_pc(2,ina219_vsusa_data);
    sci1_send_ina219_data_vccp_to_pc(3,ina219_vcc3_3_data);
    sci1_send_ina219_data_vccp_to_pc(4,ina219_vcore_data);
    sci1_send_ina219_data_vccp_to_pc(5,ina219_vdd_data);
    sci1_send_ina219_data_vccp_to_pc(6,ina219_vtt_data);
    sci1_send_end_splitchar_to_pc();
}
  
void send_cpuid_to_pc(void) 
{
    unsigned char showstring[]={'c','p','u','i','d',' ','i','s',':', ' ','\0'};
   	jtag_goto_Test_Logic_Reset();
    jtag_goto_Shift_IR();
    bytes_send[0] =  0;
    bytes_send[1] = 6;
    bytes_send[2] = 0x02;
    jtag_scan_bits_start (bytes_send, 3, bytes_get);
    Bus16M_Delay_1ms() ;
 
    jtag_goto_Shift_DR();
    bytes_send[0] =  0;    
    bytes_send[1] = 0x20;   //32
    bytes_send[2] = 0x00;
    bytes_send[3] = 0x00;
   
    jtag_scan_bits_start (bytes_send, 6, bytes_get);
    
    SCI1_SendString(showstring);
    SCI1_SendChar('0');
    SCI1_SendChar('X');
    SendHex(bytes_get[5]);
    SendHex(bytes_get[4]);
    SendHex(bytes_get[3]);
    SendHex(bytes_get[2]);
    SCI1_SendChar(0x0D);
    SCI1_SendChar(0X0A);
}

void send_cmd_help(void) 
{
   unsigned char cmdstring[]={'c','m','d',' ','h','e','l','p',':',0x0d,0x0a,
                             'r','-','r','u','n',' ','a','n','d',' ','s','t','a','r','t',' ','r','e','a','d',' ','s','e','n','s','o','r',0x0d,0x0a,
                             's','-','s','t','o','p',' ','r','e','a','d',' ','s','e','n','s','o','r',0x0d,0x0a,
                             'j','-','j','t','a','g',' ','s','t','a','r','t',0x0d,0x0a,
                             'e','-','j','t','a','g',' ','s','t','o','p',0x0d,0x0a,'\0'};
   SCI1_SendString(cmdstring);
}
void main(void) 
{


                             
  DisableInterrupts;
  Pll_16M_Init();
  EnableSCI1_BUS16M_BAUD_RATE_9600;
  SCI1_Init();
  ECT_Init();
  RtiInit();
  jtag_mcu_io_init();
  cpu_button_io_init();
	this_tool_state();
	EnableSCI1ReInterrupt;
	EnableInterrupts;
	ina219_config();
  verify_all_ina219s_is_work();
  send_cpuid_to_pc();
  send_cmd_help();
  
  for(;;) 
  {
     if(uart_test == 'r') 
     {
        break;
     }
  }
 
  for(;;) 
  {
    ctest++ ;
    if(ctest == 60000) 
    {  
       ctest = 0;
    }
   
    
    if(read_ina219_flag == 1) 
    {
      //读ina219时间间隔
       read_all_ina219s_results();
       read_ina219_flag = 0;
    }
   
    
    if(uart_test == 'r') 
    {
       cmd_ina219_start_flag = 1;
       uart_test = 0x00;
    } 
    if(uart_test == 's') 
    {
       cmd_ina219_start_flag = 0;
       uart_test = 0x00;
    }
    
    if(uart_test == 'j') 
    {
       cmd_jtag_flag = 1;
       uart_test = 0x00;
    }
    if(uart_test == 'e') 
    {
       cmd_jtag_flag = 0;
       uart_test = 0x00;
    }
     
    
    if(cmd_ina219_start_flag) 
    {
        //ina219发送时间间隔
        if(sci_ina219_start_flag == 1)
        {
          send_ina219_all_to_pc();
          sci_ina219_start_flag = 0;
        }
    }
   
    if(cmd_jtag_flag) 
    {
      if(sci_jtag_start_flag) 
      {
         send_cpuid_to_pc();
         sci_jtag_start_flag = 0;
      }
    }
    
     _FEED_COP(); /* feeds the dog */
  } 
  
}




/************SCI0 receive isr*****************/

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 20 void SCI0Receive_ISR(void)
{
         		           
    DisableInterrupts;                   
    
    EnableInterrupts;                     
}
#pragma CODE_SEG DEFAULT

/**************SCI1 receive isr **********************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 21 void SCI1Receive_ISR(void)
{
         		           
    DisableInterrupts;                    
    uart_test = SCI1_ReceiveChar();
    //SCI1_SendChar(uart_test);
    EnableInterrupts;                     
}
#pragma CODE_SEG DEFAULT

/**************SCI1 receive isr **********************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 16 void ECT_timer_overflow_ISR(void)
{
         		           
    DisableInterrupts;
   
    ecttimecount ++;
    if(ecttimecount == 8)
    {
       sci_ina219_start_flag = 1;
       sci_jtag_start_flag = 1;
       ecttimecount = 0;
    }
   
    TFLG2_TOF = 1;
    EnableInterrupts;                     
}
#pragma CODE_SEG DEFAULT

/***************RTI timer****************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt 7 void RTI_ISR(void) 
{
  DisableInterrupts;  
  rti_iic_time++;
  if(rti_iic_time == 60000)
  {
     rti_iic_time = 0;
  }
  read_ina219_time++;
  if(read_ina219_time == 20) 
  {
     read_ina219_time = 0;
     read_ina219_flag = 1;
  }
  CRGFLG |= 0x80;
  EnableInterrupts;  
}
#pragma CODE_SEG DEFAULT