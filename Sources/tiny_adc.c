#include "tiny_adc.h"
#include "tiny_global_var.h"

void ADC_Init(void)     
{
  
  ATD0CTL2=0x40;//��ֹ�ⲿ�������жϽ�ֹ 
  ATD0CTL3=0x80;//0x80;//����Ҷ���w�޷��ţ�ת������16����FIFO������ģʽ�������� 10
  ATD0CTL4=0x03; //4��ADʱ�����ڣ�PRE=3��F=F/2*��PRE+1��
  ATD0CTL5=0x30; //��ֹ����ͨ��������ת������ͨ����AN0 
  //ATD0DIEN=0x00; //��ֹ��������
  ATD0DIEN_IEN5  = 0;  //ģ������
  ATD0DIEN_IEN6 = 0;   //ģ������
}

void AD_GetValue(void)     
{
  
  while(!ATD0STAT0_SCF);   
  g_AD_value[1]=ATD0DR5L;  //CPURSTB_M_IN
  g_AD_value[2]=ATD0DR6L;  //PWRGD_CNTRL_M
 
}