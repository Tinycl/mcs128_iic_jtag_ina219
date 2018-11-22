#include "tiny_ina219.h"
#include "tiny_sw_iic.h"
#include "tiny_sci.h"
#include "tiny_global_var.h"

unsigned int ina219_calValue;
unsigned int ina219_current_LSB_mA;
unsigned int ina219_voltage_LSB_mV = 4 ; //ina219 datasheet
unsigned int ina219_power_LSB_mW;
float ina219_shunt_LSB = 0.01; 
/*********************
*vcore r 0.01
*vdd   r 0.01
*vtt   r 0.01
*vccp  r 0.01
*vsusa r 0.01
***********************/

//write slave reg
void ina219_Write_Reg_Word(unsigned char iic_7bit_address, unsigned char reg, unsigned int data) 
{
  unsigned char value[2];
  unsigned char ack = 0;
  value[0] = (unsigned char)(data >> 8);  //high byte
  value[1] = (unsigned char)(data & 0xff); //low byte
  tiny_iic_sw_start();
  tiny_iic_sw_sendbyte(iic_7bit_address << 1);  //iic address
  ack = tiny_iic_sw_receiveACK(); 
  tiny_iic_sw_sendbyte(reg);  //reg
  ack = tiny_iic_sw_receiveACK();
  tiny_iic_sw_sendbyte(value[0]);
  ack = tiny_iic_sw_receiveACK();
  tiny_iic_sw_sendbyte(value[1]);
  ack = tiny_iic_sw_receiveACK();
  tiny_iic_sw_stop();
}
//read slave reg
#pragma MESSAGE DISABLE C12056
unsigned int ina219_Read_Reg_Word(unsigned char iic_7bit_address, unsigned char reg) 
{
   unsigned char value[2];
   unsigned int data;
   unsigned char ack;
   tiny_iic_sw_start();
   tiny_iic_sw_sendbyte(iic_7bit_address << 1);  //iic address , write op
   ack = tiny_iic_sw_receiveACK();
   tiny_iic_sw_sendbyte(reg);  //reg
   ack = tiny_iic_sw_receiveACK();
   tiny_iic_sw_start(); //restart
   tiny_iic_sw_sendbyte((iic_7bit_address << 1) | 0x01);  //iic address , read op
   ack = tiny_iic_sw_receiveACK();
   value[0] = tiny_iic_sw_readbyte();
   tiny_iic_sw_sendACK(ACK);
   value[1] = tiny_iic_sw_readbyte();
   tiny_iic_sw_sendACK(NACK);
   tiny_iic_sw_stop();
   data = (unsigned int)value[0];
   data = ((unsigned int)(data << 8)) + value[1];
   return data;
}

#pragma MESSAGE DISABLE C12056
unsigned int ina219_Get_Config(unsigned char iic_7bit_address) 
{
   unsigned char value[2];
   unsigned char ack = 0;
   unsigned int data = 0; 
   tiny_iic_sw_start();
   tiny_iic_sw_sendbyte(iic_7bit_address << 1);  //iic address , write op
   ack = tiny_iic_sw_receiveACK();
   tiny_iic_sw_sendbyte(INA219_REG_CONFIG);  //reg
   ack = tiny_iic_sw_receiveACK();
   tiny_iic_sw_start(); //restart
   tiny_iic_sw_sendbyte((iic_7bit_address << 1) | 0x01);  //iic address , read 
   ack = tiny_iic_sw_receiveACK();
   value[0] = tiny_iic_sw_readbyte();
   tiny_iic_sw_sendACK(ACK);
   value[1] = tiny_iic_sw_readbyte();
   tiny_iic_sw_sendACK(NACK);
   tiny_iic_sw_stop();
   data = (unsigned int)value[0];
   data = ((unsigned int)(data << 8)) + value[1];
   return data;
}

void ina219_SetCalibration_16V_32A(unsigned char iic_7bit_address)
{
  unsigned int configValue;
  // By default we use a pretty huge range for the input voltage,
  // which probably isn't the most appropriate choice for system
  // that don't use a lot of power.  But all of the calculations
  // are shown below if you want to change the settings.  You will
  // also need to change any relevant register settings, such as
  // setting the VBUS_MAX to 16V instead of 32V, etc.
  
  // VBUS_MAX     = 16V   (Assumes 16V, can also be set to 32V)
  // VSHUNT_MAX   = 0.32  (Assumes Gain 8, 320mV, can also be 0.16, 0.08, 0.04)
  // RSHUNT       = 0.01   (Resistor value in ohms)
  
  // 1. Determine max possible current
  // MaxPossible_I = VSHUNT_MAX / RSHUNT
  // MaxPossible_I = 32A
  
  // 2. Determine max expected current
  // MaxExpected_I = 32A
  
  // 3. Calculate possible range of LSBs (Min = 15-bit, Max = 12-bit)
  // MinimumLSB = MaxExpected_I/32767
  // MinimumLSB = 0.00097            (0.97mA per bit)
  // MaximumLSB = MaxExpected_I/4096
  // MaximumLSB = 0.0078            (7.8mA per bit)
  
  // 4. Choose an LSB between the min and max values
  //    (Preferrably a roundish number close to MinLSB)
  // CurrentLSB = 0.001            (1000uA per bit) 1mA
   ina219_current_LSB_mA = 1;     // Current LSB = 1000uA per bit
  // 5. Compute the calibration register
  // Cal = trunc (0.04096 / (Current_LSB * RSHUNT))
  // Cal = 4096 (0x1000)
  
  ina219_calValue = 0x1000;
  
  // 6. Calculate the power LSB
  // PowerLSB = 20 * CurrentLSB
  // PowerLSB = 0.02 (20mW per bit)
  ina219_power_LSB_mW = 20;
  // 7. Compute the maximum current and shunt voltage values before overflow
  //
  // Max_Current = Current_LSB * 32767
  // Max_Current = 32.767A before overflow
  //
  // If Max_Current > Max_Possible_I then
  //    Max_Current_Before_Overflow = MaxPossible_I
  // Else
  //    Max_Current_Before_Overflow = Max_Current
  // End If
  //
  // Max_ShuntVoltage = Max_Current_Before_Overflow * RSHUNT
  // Max_ShuntVoltage = 0.32767V
  //
  // If Max_ShuntVoltage >= VSHUNT_MAX
  //    Max_ShuntVoltage_Before_Overflow = VSHUNT_MAX
  // Else
  //    Max_ShuntVoltage_Before_Overflow = Max_ShuntVoltage
  // End If
  
  // 8. Compute the Maximum Power
  // MaximumPower = Max_Current_Before_Overflow * VBUS_MAX
  // MaximumPower = 32.767 * 16V
  // MaximumPower = 524W
  
  
  // Set Calibration register to 'Cal' calculated above
  ina219_Write_Reg_Word(iic_7bit_address, INA219_REG_CALIBRATIOHN, ina219_calValue);
  
  // Set Config register to take into account the settings above
  configValue = (INA219_CFG_BVOLT_RANGE_16V | INA219_CFG_SVOLT_RANGE_320MV | INA219_CFG_BADCRES_12BIT_16S_8MS | INA219_CFG_SADCRES_12BIT_16S_8MS | INA219_CFG_MODE_SANDBVOLT_CONTINUOUS );
  global_ina219_config = configValue; 
  ina219_Write_Reg_Word(iic_7bit_address, INA219_REG_CONFIG, configValue);
}

void ina219_config(void) 
{
     ina219_SetCalibration_16V_32A(INA219_IIC_7BIT_ADDRESS_VCCP);
     ina219_SetCalibration_16V_32A(INA219_IIC_7BIT_ADDRESS_VCCA);
     ina219_SetCalibration_16V_32A(INA219_IIC_7BIT_ADDRESS_VSUSA);
     ina219_SetCalibration_16V_32A(INA219_IIC_7BIT_ADDRESS_VCC3_3);
     ina219_SetCalibration_16V_32A(INA219_IIC_7BIT_ADDRESS_VCORE);
     ina219_SetCalibration_16V_32A(INA219_IIC_7BIT_ADDRESS_VDD);
     ina219_SetCalibration_16V_32A(INA219_IIC_7BIT_ADDRESS_VTT);
}
int ina219_get_one_result_raw(unsigned char iic_7bit_address, unsigned char reg) 
{
   int value;
   unsigned int temp;
 
   switch(reg) 
   {
      case INA219_REG_SHUNTVOLTAGE: 
      {
        temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_SHUNTVOLTAGE);
        break;
      }
      case INA219_REG_BUSVOLTAGE: 
      {
        temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_BUSVOLTAGE);
        temp >>= 3; //shift 3 to drop CNVR OVF
        break;
      }
      case INA219_REG_POWER: 
      {
        ina219_Write_Reg_Word(iic_7bit_address, INA219_REG_CALIBRATIOHN, ina219_calValue);
        temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_POWER);
        break;
      }
      case INA219_REG_CURRENT: 
      {
        ina219_Write_Reg_Word(iic_7bit_address, INA219_REG_CALIBRATIOHN, ina219_calValue);
        temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_CURRENT);
        break;
      } 
      default: break;
   }
  
   value = (int)temp;
   return value;
}


void ina219_get_one_result(unsigned char iic_7bit_address, unsigned char reg, INA219_DATA *pina219data) 
{
  
     switch(reg) 
     {
        case INA219_REG_SHUNTVOLTAGE: 
        {
           pina219data->shunt_raw = ina219_get_one_result_raw(iic_7bit_address, INA219_REG_SHUNTVOLTAGE);
           pina219data->shunt =  pina219data->shunt_raw * ina219_shunt_LSB;
           break;
        }
        case INA219_REG_BUSVOLTAGE: 
        {
          pina219data->voltage_raw = ina219_get_one_result_raw(iic_7bit_address, INA219_REG_BUSVOLTAGE);
          pina219data->voltage = (long)pina219data->voltage_raw * ina219_voltage_LSB_mV;
          
          break;
        }
        case INA219_REG_POWER: 
        {
          pina219data->power_raw = ina219_get_one_result_raw(iic_7bit_address, INA219_REG_POWER);
          pina219data->power = (long)pina219data->power_raw * ina219_power_LSB_mW;
          break;
        }
        case INA219_REG_CURRENT: 
        {
          pina219data->current_raw = ina219_get_one_result_raw(iic_7bit_address, INA219_REG_CURRENT);
          pina219data->current = (long)pina219data->current_raw * ina219_current_LSB_mA;
          break;
        } 
        default: break;
     }
    return; 
}

void ina219_get_all_result(unsigned char iic_7bit_address, INA219_DATA *pina219data) 
{
   int value;
   unsigned int temp;
   // config
   temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_CONFIG);
   pina219data->ina219_config = temp;
   //shunt 
   temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_SHUNTVOLTAGE);
   value = (int)temp;
   pina219data->shunt_raw = value;
   pina219data->shunt = value * ina219_shunt_LSB;
   //voltage  
   temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_BUSVOLTAGE);
   temp >>= 3; //shift 3 to drop CNVR OVF
   value = (int)temp;
   pina219data->voltage_raw = value;
   pina219data->voltage = (long)value * ina219_voltage_LSB_mV;
   //power
   ina219_Write_Reg_Word(iic_7bit_address, INA219_REG_CALIBRATIOHN, ina219_calValue);
   temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_POWER);
   value = (int)temp;
   pina219data->power_raw = value;
   pina219data->power = (long)value * ina219_power_LSB_mW;
   //current
   ina219_Write_Reg_Word(iic_7bit_address, INA219_REG_CALIBRATIOHN, ina219_calValue);
   temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_CURRENT);
   value = (int)temp;
   pina219data->current_raw = value;
   pina219data->current = (long)value * ina219_current_LSB_mA; 
   //calibration
   temp = ina219_Read_Reg_Word(iic_7bit_address, INA219_REG_CALIBRATIOHN);
   pina219data->ina219_calbr = temp;
  
}



void sci1_send_start_splitchar_to_pc(void) 
{
   unsigned char kk = 0;
   unsigned char showstring[]={'s','t','a','r','t',' ','r','e','a','d',' ','s','e','n','s','o','r',' ' ,'d','a','t','a','\0'};
   for(kk = 0; kk < 30; kk++) 
   {
      SCI1_SendChar('*');
   }
 
   SCI1_SendString(showstring);
   for(kk = 0; kk < 30; kk++) 
   {
      SCI1_SendChar('*');
   }
   SCI1_SendChar(0x0D);
   SCI1_SendChar(0X0A);
}

void sci1_send_end_splitchar_to_pc(void) 
{
   unsigned char kk = 0;
   unsigned char showstring[]={'e','n','d',' ','r','e','a','d',' ','s','e','n','s','o','r',' ' ,'d','a','t','a','\0'};
   for(kk = 0; kk < 30; kk++) 
   {
      SCI1_SendChar('*');
   }
   SCI1_SendString(showstring);
   for(kk = 0; kk < 30; kk++) 
   {
      SCI1_SendChar('*');
   }
   SCI1_SendChar(0x0D);
   SCI1_SendChar(0X0A);
}

#pragma MESSAGE DISABLE C12056
void sci1_send_ina219_data_vccp_to_pc(unsigned char flag, INA219_DATA ina219_data)
{
   unsigned char showstring1[]={'s','e','n','s','o','r','1','-','V','C','C','P',' ',' ','\0'};
   unsigned char showstring2[]={'s','e','n','s','o','r','2','-','V','C','C','A',' ',' ','\0'};
   unsigned char showstring3[]={'s','e','n','s','o','r','3','-','V','S','U','S','A',' ','\0'};
   unsigned char showstring4[]={'s','e','n','s','o','r','4','-','V','C','C','3','V','3','\0'};
   unsigned char showstring5[]={'s','e','n','s','o','r','5','-','V','C','O','R','E',' ','\0'};
   unsigned char showstring6[]={'s','e','n','s','o','r','6','-','V','D','D',' ',' ',' ','\0'};
   unsigned char showstring7[]={'s','e','n','s','o','r','7','-','V','T','T',' ',' ',' ','\0'};
   if(flag == 0) 
   {
     SCI1_SendString(showstring1);
   } 
   else if(flag == 1) 
   {
     SCI1_SendString(showstring2);
   }
   else if(flag == 2) 
   {
     SCI1_SendString(showstring3);
   }
   else if(flag == 3) 
   {
     SCI1_SendString(showstring4);
   }
   else if(flag == 4) 
   {
     SCI1_SendString(showstring5);
   }
   else if(flag == 5) 
   {
     SCI1_SendString(showstring6);
   }
   else if(flag == 6) 
   {
     SCI1_SendString(showstring7);
   } 
   else 
   {
    return;
   }
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar('v');
   SCI1_SendChar('o');
   SCI1_SendChar('l');
   SCI1_SendChar('t');
   SCI1_SendChar('a');
   SCI1_SendChar('g');
   SCI1_SendChar('e');
   SCI1_SendChar(':');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendDec(ina219_data.voltage);
   SCI1_SendChar('m');
   SCI1_SendChar('V');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar('c');
   SCI1_SendChar('u');
   SCI1_SendChar('r');
   SCI1_SendChar('r');
   SCI1_SendChar('e');
   SCI1_SendChar('n');
   SCI1_SendChar('t');
   SCI1_SendChar(':');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendDec(ina219_data.current);
   SCI1_SendChar('m');
   SCI1_SendChar('A');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendChar('p');
   SCI1_SendChar('o');
   SCI1_SendChar('w');
   SCI1_SendChar('e');
   SCI1_SendChar('r');
   SCI1_SendChar(':');
   SCI1_SendChar(' ');
   SCI1_SendChar(' ');
   SCI1_SendDec(ina219_data.power);
   SCI1_SendChar('m');
   SCI1_SendChar('W');
   SCI1_SendChar(0x0D);
   SCI1_SendChar(0X0A);
}

void sci1_send_ina219_is_work(unsigned char kk) 
{
    unsigned char showstring1[]={'s','e','n','s','o','r',' ','\0'};
    unsigned char showstring2[]={' ','i','s',' ','w','o','r','k','!','\0'};
    SCI1_SendString(showstring1);
    SCI1_SendChar(kk + '0'); 
    SCI1_SendString(showstring2); 
    SCI1_SendChar(0x0D); 
    SCI1_SendChar(0X0A);
}

void sci1_send_ina219_is_not_work(unsigned char kk) 
{
    unsigned char showstring1[]={'s','e','n','s','o','r',' ','\0'};
    unsigned char showstring2[]={' ','i','s',' ','n','o','t',' ','w','o','r','k','!','\0'};
    SCI1_SendString(showstring1);
    SCI1_SendChar(kk + '0'); 
    SCI1_SendString(showstring2); 
    SCI1_SendChar(0x0D); 
    SCI1_SendChar(0X0A);
}