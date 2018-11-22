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


/*---------------------*IIC开始*----------------------*/
//送初始位 SDA=1->0
void IIC_Start(void)
{
  SCL_Out;
  SDA_Out;                        //输出
	SCL=0;
	SDA=1;
	_asm nop;
	SCL=1;
	nops();
	SDA=0;
	nops();
	SCL=0;
}

/*---------------------*IIC结束*----------------------*/
//送停止位 SDA=0->1
void IIC_Stop(void)
{
  SCL_Out;
  SDA_Out;                        //输出
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

/*---------------------*字节发送*---------------------*/
//字节发送程序
void Send_Byte(unsigned char SendChar)
{
	unsigned char i; 
	SCL_Out;
	SDA_Out;
	for(i=0;i<8;i++)
	{
		SCL=0;
		if((SendChar<<i)&0x80)
		  SDA=1;                        //发送1
	  else 
	    SDA=0;                        //发送0
		_asm nop;
		SCL=1;
		nops();
		SCL=0;
	}
	nops();
	SDA=1;                            //发送完8bit，释放总线准备接收应答位
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
/*---------------------*字节读取*---------------------*/
//字节接收程序
unsigned char Read_Byte(void)
{
  unsigned char i;
  unsigned char ReadChar;
	ReadChar=0;
	SCL_Out;
	SCL=0;
	_asm nop;
	SDA_Out;
	SDA=1;                            //置数据线为输入方式
	SDA_In;                       
	for(i=0;i<8;i++)
	{
		_asm nop;
		SCL=0;                          //置时钟线为低，准备接收数据位
		nops();
		SCL=1;                          //置时钟线为高，使数据线上数据有效
		_asm nop;
		ReadChar<<=1;
		if(SDA)
		ReadChar+=1;                    //读数据位，将接收的数据存c
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
	SDA=1;                            //置数据线为输入方式
	SDA_In;                       
	for(i=0;i<8;i++)
	{
		_asm nop;
		SCL=0;                          //置时钟线为低，准备接收数据位
		nops();
		SCL=1;                          //置时钟线为高，使数据线上数据有效
		_asm nop;
		high<<=1;
		if(SDA)
		high+=1;                    //读数据位，将接收的数据存c
	}
	SDA_Out;
	SCL=0;
	                      
	for(i=0;i<8;i++)
	{
		_asm nop;
		SCL=0;                          //置时钟线为低，准备接收数据位
		nops();
		SCL=1;                          //置时钟线为高，使数据线上数据有效
		_asm nop;
		low<<=1;
		if(SDA)
		low+=1;                    //读数据位，将接收的数据存c
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
    
    

/*-------------------*MMA读取字节*--------------------*/

unsigned char MMA8451_ReadByte(unsigned char Address)
{
  unsigned char Ret = 100;
	IIC_Start();		                  //启动
	Send_Byte(0x38);	                //写入设备ID及写信号
	Send_Byte(Address);	              //X地址
	IIC_Start();		                  //重新发送开始
	Send_Byte(0x39);	                //写入设备ID及读信
	Ret=Read_Byte();	                //读取一字节
	IIC_Stop();
	return Ret;
}

/*-------------------*MMA写入字节*--------------------*/

void MMA8451_WriteByte(unsigned char Address, unsigned char Thedata)
{
	IIC_Start();		                  //启动
	Send_Byte(0x38);	                //写入设备ID及写信号
	Send_Byte(Address);	              //X地址
	Send_Byte(Thedata);	              //写入设备ID及读信
	IIC_Stop();
}
  
/*----------------*加速度传感器初始化*----------------*/

void MMA8451_Init(void) 
{	
//  MMA8451_WriteByte(0x2A,0x06);	
//  nops();  nops();  nops();  
//  nops();  nops();  nops(); 
  MMA8451_WriteByte(0x0E,0x00);     //XYZ数据设置寄存器地址0x0E,设置量程为2g
  nops();  nops();  nops();  
  nops();  nops();  nops(); 
  MMA8451_WriteByte(0x2A,0x07);     //激活状态
  nops();  nops();  nops();  
  nops();  nops();  nops(); 
}
