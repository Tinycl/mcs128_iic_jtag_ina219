#include "tiny_hw_iic.h"    

void tiny_hwiic_Init(void)
{
    IBFD = 0x85;//����Ƶ��:20MHz,IIC����Ƶ��:62.5KHz,SDA����ʱ��:2.45us  85   60
         
    IBAD = 0xa0;      //D7-D0λ��MCU��Ϊ�ӻ�ʱ�ĵ�ַ,���λ��ʹ��  a0       f0
	//������Ӧ���ź�
           //����ģʽ
           //�ӻ�ģʽ
           //��ֹIIC�ж�
           //ʹ��IICģ��
    IBCR = 0x88;     
}

//SendSignal:��IIC�����Ϸ�����ͣ�ź�---------------------------------------*
//�� ��:������Ҫ������ʼ��ֹͣ�ź�                                         *
//�� ��:Signal = 'S'(Start),������ʼ�ź�;Signal = 'O'(Over),����ֹͣ�ź�   *
//�� ��:��                                                                 *
//-------------------------------------------------------------------------*
void SendSignal(unsigned char Signal)
{
    if (Signal == 'S')    
        IBCR |= 0x20;     //����ģʽѡ��λMST��0��Ϊ1,���Բ�����ʼ�ź�
    else if (Signal == 'O')
        IBCR &= ~0x20;    //����ģʽѡ��λMST��1��Ϊ0,���Բ���ֹͣ�ź�
}

//Wait:�ȴ�Ӧ���һ���ֽ����ݵĴ������------------------------------------*
//����:��ʱ����,ѭ��������Ӧ���־λ,������ɱ�־λ,�ж�MCU�Ƿ���յ�Ӧ*
//     ���źŻ�һ���ֽ��Ƿ����������ϴ������                              *
//����:x = 'A'(Ack),�ȴ�Ӧ��;x = 'T'(Transmission),�ȴ�һ���ֽ����ݴ������*
//����:0:�յ�Ӧ���źŻ�һ���ֽڴ������;1:δ�յ�Ӧ���źŻ�һ���ֽ�û������ *
//-------------------------------------------------------------------------*
unsigned char Wait(unsigned char x)
{
    unsigned char ErrTime, i;
    ErrTime = 255;    //�����ѯ��ʱʱ��
    for (i = 0;i < ErrTime;i++)
    {
        if (x == 'A')          //�ȴ�Ӧ���ź�
        {
            if ((IBSR & 0x01) == 0)   
                return 0;    //������һ���ֽں�,�յ��˴ӻ���Ӧ���ź�
        }
        else if (x == 'T')     //�ȴ��������һ���ֽ��ź�
        {
            if ((IBSR & 0x02) != 0)    
            {
                IBSR |= 0x02;    //��IICIF��־λ
                return 0;        //�ɹ�������һ���ֽ�
            }       
        }
    }
    if (i >= ErrTime)
        return 1;     //��ʱ,û���յ�Ӧ���źŻ�����һ���ֽ�
}

//IICread1:�Ӵӻ���1���ֽ�����---------------------------------------------*
//��    ��:�Ӵӻ���1���ֽ�����                                             *
//��    ��:                                                                *
//    (1) DeviceAddr:�豸��ַ                                              *
//    (2) AccessAddr:���ʵ�ַ                                              *
//    (3) Data:�����յ���һ���ֽ�����                                      *
//��    ��:Ϊ0,�ɹ���һ���ֽ�;Ϊ1,��һ���ֽ�ʧ��                           *
//�ڲ�����:SendSignal,Wait                                                 *
//-------------------------------------------------------------------------*
unsigned char tiny_hw_iic_Readbyte(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char *Data)
{      
    IBCR |= 0x10;                //TX = 1,MCU����Ϊ����ģʽ
    SendSignal('S');             //���Ϳ�ʼ�ź�    
    IBDR = DeviceAddr & 0xfe;    //�����豸��ַ,��֪ͨ�ӻ���������    
    if (Wait('T'))               //�ȴ�һ���ֽ����ݴ������        
        return 1;                //û�д��ͳɹ�,��һ���ֽ�ʧ��    
    if (Wait('A'))               //�ȴ��ӻ�Ӧ���ź�        
        return 1;                //û�еȵ�Ӧ���ź�,��һ���ֽ�ʧ��    
    IBDR = AccessAddr;           //���ͷ��ʵ�ַ    
   if (Wait('T'))               //�ȴ�һ���ֽ����ݴ������    
        return 1;                //û�д��ͳɹ�,��һ���ֽ�ʧ��
    if (Wait('A'))               //�ȴ��ӻ�Ӧ���ź�    
        return 1;                //û�еȵ�Ӧ���ź�,��һ���ֽ�ʧ��       
    IBCR |= 0x04;                //����ģʽ��,RSTAλ��1,�����ظ���ʼ�ź�    
    IBDR = DeviceAddr | 0x01;    //֪ͨ�ӻ���Ϊ��������    
    if (Wait('T'))               //�ȴ�һ���ֽ����ݴ������    
        return 1;                //û�д��ͳɹ�,��һ���ֽ�ʧ��    
    if (Wait('A'))               //�ȴ��ӻ�Ӧ���ź�   
        return 1;                //û�еȵ�Ӧ���ź�,��һ���ֽ�ʧ��
    IBCR &= 0xef;                //TX = 0,MCU����Ϊ����ģʽ   
    *Data = IBDR;                //����IBDR,׼���������� 
       
    if (Wait('T'))               //�ȴ��ӻ�Ӧ���ź�   
       return 1;                //û�еȵ�Ӧ���ź�,��һ���ֽ�ʧ��   
    SendSignal('O');             //����ֹͣ�ź�    
    *Data = IBDR;                //�������յ���һ������
        
    return 0;                    //��ȷ���յ�һ���ֽ�����
    
    
}

//IICwrite1:��ӻ�д1���ֽ�����--------------------------------------------*
//��     ��:��ӻ�д1���ֽ�����                                            *
//��     ��:                                                               *
//    (1) DeviceAddr:�豸��ַ                                              *
//    (2) AccessAddr:���ʵ�ַ                                              *
//    (3) Data:Ҫ�����ӻ���1���ֽ�����                                     *
//��     ��:Ϊ0,�ɹ�дһ���ֽ�;Ϊ1,дһ���ֽ�ʧ��                          *
//�ڲ����� :Start, SendByte, WaitAck, Stop                                 *
//-------------------------------------------------------------------------*
unsigned char tiny_hw_iic_Writebyte(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data)
{
    IBCR |= 0x10;                //TX = 1,MCU����Ϊ����ģʽ
    SendSignal('S');             //���Ϳ�ʼ�ź�
    IBDR = DeviceAddr & 0xfe;    //�����豸��ַ,��֪ͨ�ӻ���������
    if (Wait('T'))               //�ȴ�һ���ֽ����ݴ������
        return 1;                //û�д��ͳɹ�,дһ���ֽ�ʧ��
    if (Wait('A'))               //�ȴ��ӻ�Ӧ���ź�
        return 1;                //û�еȵ�Ӧ���ź�,дһ���ֽ�ʧ��   
    IBDR = AccessAddr;           //���ͷ��ʵ�ַ
    if (Wait('T'))               //�ȴ�һ���ֽ����ݴ������
        return 1;                //û�д��ͳɹ�,дһ���ֽ�ʧ��
    if (Wait('A'))               //�ȴ��ӻ�Ӧ���ź�
        return 1;                //û�еȵ�Ӧ���ź�,дһ���ֽ�ʧ��   
    IBDR = Data;                 //д����
    if (Wait('T'))               //�ȴ�һ���ֽ����ݴ������
        return 1;                //û�д��ͳɹ�,дһ���ֽ�ʧ��
    if (Wait('A'))               //�ȴ��ӻ�Ӧ���ź�
        return 1;                //û�еȵ�Ӧ���ź�,дһ���ֽ�ʧ��   
    SendSignal('O');             //����ֹͣ�ź�
    return 0;
}


unsigned char tiny_hw_iic_Readbytes(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data[], unsigned char N)
{
    unsigned char i, j;
    for (i = 0;i < N;i++)
    {
        for(j = 0;j < 15;j++);    //��С��ʱ(���͵�ÿ���ֽ�֮��Ҫ��ʱ����)
        if (tiny_hw_iic_Readbyte(DeviceAddr, AccessAddr+i, &Data[i]))  
            return 1;    //����һ���ֽ�û�н��յ�,����ʧ�ܱ�־:1
    }
    if (i >= N)
        return 0;        //�ɹ�����N������,���سɹ���־:0
}


unsigned char tiny_hw_iic_Writebytes(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data[], unsigned char N)
{
    unsigned char i, j;
    for (i = 0;i < N;i++)
    {
        for(j = 0;j < 15;j++);    //��С��ʱ(���͵�ÿ���ֽ�֮��Ҫ��ʱ����)
        if (tiny_hw_iic_Writebyte(DeviceAddr, AccessAddr + i, Data[i]))
            return 1;    //����һ���ֽ�û�з��ͳ�ȥ,����ʧ�ܱ�־:1
    }
    if (i >= N)
        return 0;        //�ɹ�����N������,���سɹ���־:0
}



