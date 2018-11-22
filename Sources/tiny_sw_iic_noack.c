#include "tiny_sw_iic_noack.h"

void IIC_PJ_init(void)
{
  PERJ_PERJ6 = 1;
  PERJ_PERJ7 = 1;
  PIEJ_PIEJ6 = 0;
  PIEJ_PIEJ6 = 0;
 // PPSJ_PPSJ6 = 1;
  //PPSJ_PPSJ7 = 1;
 // PTIJ_PTIJ6 = 1; 
 // PTIJ_PTIJ7 = 1;   
}

void nops(void) 
{
  unsigned char i = 0;
  for(i = 0; i < 5; i++) 
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


/*---------------------*IIC��ʼ*----------------------*/
//�ͳ�ʼλ SDA=1->0
void IIC_Start(void)
{
  SCL_Out;
  SDA_Out;                        //���
	SCL=0;
	SDA=1;
	_asm nop;
	SCL=1;
	nops();
	SDA=0;
	nops();
	SCL=0;
}

/*---------------------*IIC����*----------------------*/
//��ֹͣλ SDA=0->1
void IIC_Stop(void)
{
  SCL_Out;
  SDA_Out;                        //���
	SCL=0;
	_asm nop;
	SDA=0;
	_asm nop;
	SCL=1;
	nops();
	SDA=1;
	nops();
	SCL=0;
}

/*---------------------*�ֽڷ���*---------------------*/
//�ֽڷ��ͳ���
void Send_Byte(unsigned char SendChar)
{
	unsigned char i; 
	SCL_Out;
	SDA_Out;
	for(i=0;i<8;i++)
	{
		SCL=0;
		if((SendChar<<i)&0x80)
		  SDA=1;                        //����1
	  else 
	    SDA=0;                        //����0
		_asm nop;
		SCL=1;
		nops();
		SCL=0;
	}
	nops();
	SDA=1;                            //������8bit���ͷ�����׼������Ӧ��λ
	_asm nop;
	SCL=1;
	nops(); 
	SCL=0; 
}
void Send_Bytes(unsigned char *data, unsigned char len) 
{
  unsigned char i = 0;
  for(i = 0; i < len; i++) 
  {
    Send_Byte(data[i]);
  }
}
/*---------------------*�ֽڶ�ȡ*---------------------*/
//�ֽڽ��ճ���
unsigned char Read_Byte(void)
{
  unsigned char i;
  unsigned char ReadChar;
	ReadChar=0;
	SCL_Out;
	SCL=0;
	_asm nop;
	SDA_Out;
	SDA=1;                            //��������Ϊ���뷽ʽ
	SDA_In;                       
	for(i=0;i<8;i++)
	{
		_asm nop;
		SCL=0;                          //��ʱ����Ϊ�ͣ�׼����������λ
		nops();
		SCL=1;                          //��ʱ����Ϊ�ߣ�ʹ��������������Ч
		_asm nop;
		ReadChar<<=1;
		if(SDA)
		ReadChar+=1;                    //������λ�������յ����ݴ�c
	}
	SDA_Out;
	SCL=0;
	return ReadChar;
}

unsigned int Read_Word(void)
{
  unsigned char i;
  unsigned char low;
  unsigned char high;
  unsigned int ReadWord;
	ReadWord=0;
	SCL_Out;
	SCL=0;
	_asm nop;
	SDA_Out;
	SDA=1;                            //��������Ϊ���뷽ʽ
	SDA_In;                       
	for(i=0;i<8;i++)
	{
		_asm nop;
		SCL=0;                          //��ʱ����Ϊ�ͣ�׼����������λ
		nops();
		SCL=1;                          //��ʱ����Ϊ�ߣ�ʹ��������������Ч
		_asm nop;
		high<<=1;
		if(SDA)
		high+=1;                    //������λ�������յ����ݴ�c
	}
	SDA_Out;
	SCL=0;
	                      
	for(i=0;i<8;i++)
	{
		_asm nop;
		SCL=0;                          //��ʱ����Ϊ�ͣ�׼����������λ
		nops();
		SCL=1;                          //��ʱ����Ϊ�ߣ�ʹ��������������Ч
		_asm nop;
		low<<=1;
		if(SDA)
		low+=1;                    //������λ�������յ����ݴ�c
	}
	SDA_Out;
	SCL=0;
	ReadWord = (unsigned int)high;
	ReadWord = ((unsigned int)ReadWord<<8) + low;
	return ReadWord;
}
void Read_Bytes(unsigned char *data, unsigned char len) 
{
  unsigned char i = 0;
  for(i = 0; i < len; i++) 
  {
    data[i] =  Read_Byte();
  }
}
    
    

/*-------------------*MMA��ȡ�ֽ�*--------------------*/

unsigned char MMA8451_ReadByte(unsigned char Address)
{
  unsigned char Ret = 100;
	IIC_Start();		                  //����
	Send_Byte(0x38);	                //д���豸ID��д�ź�
	Send_Byte(Address);	              //X��ַ
	IIC_Start();		                  //���·��Ϳ�ʼ
	Send_Byte(0x39);	                //д���豸ID������
	Ret=Read_Byte();	                //��ȡһ�ֽ�
	IIC_Stop();
	return Ret;
}

/*-------------------*MMAд���ֽ�*--------------------*/

void MMA8451_WriteByte(unsigned char Address, unsigned char Thedata)
{
	IIC_Start();		                  //����
	Send_Byte(0x38);	                //д���豸ID��д�ź�
	Send_Byte(Address);	              //X��ַ
	Send_Byte(Thedata);	              //д���豸ID������
	IIC_Stop();
}
  
/*----------------*���ٶȴ�������ʼ��*----------------*/

void MMA8451_Init(void) 
{	
//  MMA8451_WriteByte(0x2A,0x06);	
//  nops();  nops();  nops();  
//  nops();  nops();  nops(); 
  MMA8451_WriteByte(0x0E,0x00);     //XYZ�������üĴ�����ַ0x0E,��������Ϊ2g
  nops();  nops();  nops();  
  nops();  nops();  nops(); 
  MMA8451_WriteByte(0x2A,0x07);     //����״̬
  nops();  nops();  nops();  
  nops();  nops();  nops(); 
}
