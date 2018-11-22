#include "tiny_hw_iic.h"    

void tiny_hwiic_Init(void)
{
    IBFD = 0x85;//总线频率:20MHz,IIC总线频率:62.5KHz,SDA保持时间:2.45us  85   60
         
    IBAD = 0xa0;      //D7-D0位是MCU作为从机时的地址,最低位不使用  a0       f0
	//不发送应答信号
           //接收模式
           //从机模式
           //禁止IIC中断
           //使能IIC模块
    IBCR = 0x88;     
}

//SendSignal:在IIC总线上发送起停信号---------------------------------------*
//功 能:根据需要产生开始或停止信号                                         *
//参 数:Signal = 'S'(Start),产生开始信号;Signal = 'O'(Over),产生停止信号   *
//返 回:无                                                                 *
//-------------------------------------------------------------------------*
void SendSignal(unsigned char Signal)
{
    if (Signal == 'S')    
        IBCR |= 0x20;     //主机模式选择位MST由0变为1,可以产生开始信号
    else if (Signal == 'O')
        IBCR &= ~0x20;    //主机模式选择位MST由1变为0,可以产生停止信号
}

//Wait:等待应答或一个字节数据的传送完成------------------------------------*
//功能:在时限内,循环检测接收应答标志位,或传送完成标志位,判断MCU是否接收到应*
//     答信号或一个字节是否已在总线上传送完毕                              *
//参数:x = 'A'(Ack),等待应答;x = 'T'(Transmission),等待一个字节数据传输完成*
//返回:0:收到应答信号或一个字节传送完毕;1:未收到应答信号或一个字节没传送完 *
//-------------------------------------------------------------------------*
unsigned char Wait(unsigned char x)
{
    unsigned char ErrTime, i;
    ErrTime = 255;    //定义查询超时时限
    for (i = 0;i < ErrTime;i++)
    {
        if (x == 'A')          //等待应答信号
        {
            if ((IBSR & 0x01) == 0)   
                return 0;    //传送完一个字节后,收到了从机的应答信号
        }
        else if (x == 'T')     //等待传送完成一个字节信号
        {
            if ((IBSR & 0x02) != 0)    
            {
                IBSR |= 0x02;    //清IICIF标志位
                return 0;        //成功发送完一个字节
            }       
        }
    }
    if (i >= ErrTime)
        return 1;     //超时,没有收到应答信号或发送完一个字节
}

//IICread1:从从机读1个字节数据---------------------------------------------*
//功    能:从从机读1个字节数据                                             *
//参    数:                                                                *
//    (1) DeviceAddr:设备地址                                              *
//    (2) AccessAddr:访问地址                                              *
//    (3) Data:带回收到的一个字节数据                                      *
//返    回:为0,成功读一个字节;为1,读一个字节失败                           *
//内部调用:SendSignal,Wait                                                 *
//-------------------------------------------------------------------------*
unsigned char tiny_hw_iic_Readbyte(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char *Data)
{      
    IBCR |= 0x10;                //TX = 1,MCU设置为发送模式
    SendSignal('S');             //发送开始信号    
    IBDR = DeviceAddr & 0xfe;    //发送设备地址,并通知从机接收数据    
    if (Wait('T'))               //等待一个字节数据传送完成        
        return 1;                //没有传送成功,读一个字节失败    
    if (Wait('A'))               //等待从机应答信号        
        return 1;                //没有等到应答信号,读一个字节失败    
    IBDR = AccessAddr;           //发送访问地址    
   if (Wait('T'))               //等待一个字节数据传送完成    
        return 1;                //没有传送成功,读一个字节失败
    if (Wait('A'))               //等待从机应答信号    
        return 1;                //没有等到应答信号,读一个字节失败       
    IBCR |= 0x04;                //主机模式下,RSTA位置1,产生重复开始信号    
    IBDR = DeviceAddr | 0x01;    //通知从机改为发送数据    
    if (Wait('T'))               //等待一个字节数据传送完成    
        return 1;                //没有传送成功,读一个字节失败    
    if (Wait('A'))               //等待从机应答信号   
        return 1;                //没有等到应答信号,读一个字节失败
    IBCR &= 0xef;                //TX = 0,MCU设置为接收模式   
    *Data = IBDR;                //读出IBDR,准备接收数据 
       
    if (Wait('T'))               //等待从机应答信号   
       return 1;                //没有等到应答信号,读一个字节失败   
    SendSignal('O');             //发送停止信号    
    *Data = IBDR;                //读出接收到的一个数据
        
    return 0;                    //正确接收到一个字节数据
    
    
}

//IICwrite1:向从机写1个字节数据--------------------------------------------*
//功     能:向从机写1个字节数据                                            *
//参     数:                                                               *
//    (1) DeviceAddr:设备地址                                              *
//    (2) AccessAddr:访问地址                                              *
//    (3) Data:要发给从机的1个字节数据                                     *
//返     回:为0,成功写一个字节;为1,写一个字节失败                          *
//内部调用 :Start, SendByte, WaitAck, Stop                                 *
//-------------------------------------------------------------------------*
unsigned char tiny_hw_iic_Writebyte(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data)
{
    IBCR |= 0x10;                //TX = 1,MCU设置为发送模式
    SendSignal('S');             //发送开始信号
    IBDR = DeviceAddr & 0xfe;    //发送设备地址,并通知从机接收数据
    if (Wait('T'))               //等待一个字节数据传送完成
        return 1;                //没有传送成功,写一个字节失败
    if (Wait('A'))               //等待从机应答信号
        return 1;                //没有等到应答信号,写一个字节失败   
    IBDR = AccessAddr;           //发送访问地址
    if (Wait('T'))               //等待一个字节数据传送完成
        return 1;                //没有传送成功,写一个字节失败
    if (Wait('A'))               //等待从机应答信号
        return 1;                //没有等到应答信号,写一个字节失败   
    IBDR = Data;                 //写数据
    if (Wait('T'))               //等待一个字节数据传送完成
        return 1;                //没有传送成功,写一个字节失败
    if (Wait('A'))               //等待从机应答信号
        return 1;                //没有等到应答信号,写一个字节失败   
    SendSignal('O');             //发送停止信号
    return 0;
}


unsigned char tiny_hw_iic_Readbytes(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data[], unsigned char N)
{
    unsigned char i, j;
    for (i = 0;i < N;i++)
    {
        for(j = 0;j < 15;j++);    //最小延时(发送的每个字节之间要有时间间隔)
        if (tiny_hw_iic_Readbyte(DeviceAddr, AccessAddr+i, &Data[i]))  
            return 1;    //其中一个字节没有接收到,返回失败标志:1
    }
    if (i >= N)
        return 0;        //成功接收N个数据,返回成功标志:0
}


unsigned char tiny_hw_iic_Writebytes(unsigned char DeviceAddr, unsigned char AccessAddr, unsigned char Data[], unsigned char N)
{
    unsigned char i, j;
    for (i = 0;i < N;i++)
    {
        for(j = 0;j < 15;j++);    //最小延时(发送的每个字节之间要有时间间隔)
        if (tiny_hw_iic_Writebyte(DeviceAddr, AccessAddr + i, Data[i]))
            return 1;    //其中一个字节没有发送出去,返回失败标志:1
    }
    if (i >= N)
        return 0;        //成功发送N个数据,返回成功标志:0
}



