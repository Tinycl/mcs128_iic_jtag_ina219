#include "tiny_adc.h"
#include "tiny_global_var.h"

void ADC_Init(void)     
{
  
  ATD0CTL2=0x40;//禁止外部触发，中断禁止 
  ATD0CTL3=0x80;//0x80;//结果右对齐无符号，转换序列16，无FIFO，冻结模式继续进行 10
  ATD0CTL4=0x03; //4个AD时钟周期，PRE=3，F=F/2*（PRE+1）
  ATD0CTL5=0x30; //禁止特殊通道，连续转换，多通道，AN0 
  //ATD0DIEN=0x00; //禁止数字输入
  ATD0DIEN_IEN5  = 0;  //模拟输入
  ATD0DIEN_IEN6 = 0;   //模拟输入
}

void AD_GetValue(void)     
{
  
  while(!ATD0STAT0_SCF);   
  g_AD_value[1]=ATD0DR5L;  //CPURSTB_M_IN
  g_AD_value[2]=ATD0DR6L;  //PWRGD_CNTRL_M
 
}