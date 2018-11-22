#ifndef __TINY_INA219_H__
#define __TINY_INA219_H__

//five INA219 devices address




#define INA219_IIC_7BIT_ADDRESS_VCCP  0x40
#define INA219_IIC_7BIT_ADDRESS_VCCA  0x41
#define INA219_IIC_7BIT_ADDRESS_VSUSA 0x42
#define INA219_IIC_7BIT_ADDRESS_VCC3_3  0x43
#define INA219_IIC_7BIT_ADDRESS_VCORE 0x44
#define INA219_IIC_7BIT_ADDRESS_VDD 0x45
#define INA219_IIC_7BIT_ADDRESS_VTT 0x46

//INA219 REG
#define INA219_REG_CONFIG 0x00        //rw
#define INA219_REG_SHUNTVOLTAGE 0x01  //r
#define INA219_REG_BUSVOLTAGE   0x02  //r
#define INA219_REG_POWER  0x03         //r
#define INA219_REG_CURRENT 0x04       //r
#define INA219_REG_CALIBRATIOHN 0x05  //rw

//config REG bit operation
#define INA219_CFGB_RESET(x)  ((x & 0x01) << 15)   //reset bit
#define INA219_CFGB_BUSV_RANGE(x) ((x & 0x01) << 13)  //bus voltage range
#define INA219_CFGB_PGA_RANGE(x)  ((x & 0x03) << 11)  //shunt voltage range
#define INA219_CFGB_BADC_RES_AVG(x) ((x & 0x0f) << 7) //bus adc resolution/averaging
#define INA219_CFGB_SADC_RES_AVG(x) ((x & 0X0f ) << 3)//shunt adc resolution/averaging
#define INA219_CFGB_MODE(x) (x & 0x07)//operating mode

//configuration register
#define INA219_CFG_RESET                        INA219_CFGB_RESET(1)            // Reset Bit

#define INA219_CFG_BVOLT_RANGE_MASK             INA219_CFGB_BUSV_RANGE(1)       // Bus Voltage Range Mask
#define INA219_CFG_BVOLT_RANGE_16V              INA219_CFGB_BUSV_RANGE(0)       // 0-16V Range
#define INA219_CFG_BVOLT_RANGE_32V              INA219_CFGB_BUSV_RANGE(1)       // 0-32V Range (default)

#define INA219_CFG_SVOLT_RANGE_MASK             INA219_CFGB_PGA_RANGE(3)        // Shunt Voltage Range Mask
#define INA219_CFG_SVOLT_RANGE_40MV             INA219_CFGB_PGA_RANGE(0)        // Gain 1, 40mV Range
#define INA219_CFG_SVOLT_RANGE_80MV             INA219_CFGB_PGA_RANGE(1)        // Gain 2, 80mV Range
#define INA219_CFG_SVOLT_RANGE_160MV            INA219_CFGB_PGA_RANGE(2)        // Gain 4, 160mV Range
#define INA219_CFG_SVOLT_RANGE_320MV            INA219_CFGB_PGA_RANGE(3)        // Gain 8, 320mV Range (default)

#define INA219_CFG_BADCRES_MASK                 INA219_CFGB_BADC_RES_AVG(15)    // Bus ADC Resolution and Averaging Mask
#define INA219_CFG_BADCRES_9BIT_1S_84US         INA219_CFGB_BADC_RES_AVG(0)     // 1 x 9-bit Bus sample
#define INA219_CFG_BADCRES_10BIT_1S_148US       INA219_CFGB_BADC_RES_AVG(1)     // 1 x 10-bit Bus sample
#define INA219_CFG_BADCRES_11BIT_1S_276US       INA219_CFGB_BADC_RES_AVG(2)     // 1 x 11-bit Bus sample
#define INA219_CFG_BADCRES_12BIT_1S_532US       INA219_CFGB_BADC_RES_AVG(3)     // 1 x 12-bit Bus sample (default)
#define INA219_CFG_BADCRES_12BIT_2S_1MS         INA219_CFGB_BADC_RES_AVG(9)     // 2 x 12-bit Bus samples averaged together
#define INA219_CFG_BADCRES_12BIT_4S_2MS         INA219_CFGB_BADC_RES_AVG(10)    // 4 x 12-bit Bus samples averaged together
#define INA219_CFG_BADCRES_12BIT_8S_4MS         INA219_CFGB_BADC_RES_AVG(11)    // 8 x 12-bit Bus samples averaged together
#define INA219_CFG_BADCRES_12BIT_16S_8MS        INA219_CFGB_BADC_RES_AVG(12)    // 16 x 12-bit Bus samples averaged together
#define INA219_CFG_BADCRES_12BIT_32S_17MS       INA219_CFGB_BADC_RES_AVG(13)    // 32 x 12-bit Bus samples averaged together
#define INA219_CFG_BADCRES_12BIT_64S_34MS       INA219_CFGB_BADC_RES_AVG(14)    // 64 x 12-bit Bus samples averaged together
#define INA219_CFG_BADCRES_12BIT_128S_68MS      INA219_CFGB_BADC_RES_AVG(15)    // 128 x 12-bit Bus samples averaged together

#define INA219_CFG_SADCRES_MASK                 INA219_CFGB_SADC_RES_AVG(15)    // Shunt ADC Resolution and Averaging Mask
#define INA219_CFG_SADCRES_9BIT_1S_84US         INA219_CFGB_SADC_RES_AVG(0)     // 1 x 9-bit Shunt sample
#define INA219_CFG_SADCRES_10BIT_1S_148US       INA219_CFGB_SADC_RES_AVG(1)     // 1 x 10-bit Shunt sample
#define INA219_CFG_SADCRES_11BIT_1S_276US       INA219_CFGB_SADC_RES_AVG(2)     // 1 x 11-bit Shunt sample
#define INA219_CFG_SADCRES_12BIT_1S_532US       INA219_CFGB_SADC_RES_AVG(3)     // 1 x 12-bit Shunt sample (default)
#define INA219_CFG_SADCRES_12BIT_2S_1MS         INA219_CFGB_SADC_RES_AVG(9)     // 2 x 12-bit Shunt samples averaged together
#define INA219_CFG_SADCRES_12BIT_4S_2MS         INA219_CFGB_SADC_RES_AVG(10)    // 4 x 12-bit Shunt samples averaged together
#define INA219_CFG_SADCRES_12BIT_8S_4MS         INA219_CFGB_SADC_RES_AVG(11)    // 8 x 12-bit Shunt samples averaged together
#define INA219_CFG_SADCRES_12BIT_16S_8MS        INA219_CFGB_SADC_RES_AVG(12)    // 16 x 12-bit Shunt samples averaged together
#define INA219_CFG_SADCRES_12BIT_32S_17MS       INA219_CFGB_SADC_RES_AVG(13)    // 32 x 12-bit Shunt samples averaged together
#define INA219_CFG_SADCRES_12BIT_64S_34MS       INA219_CFGB_SADC_RES_AVG(14)    // 64 x 12-bit Shunt samples averaged together
#define INA219_CFG_SADCRES_12BIT_128S_68MS      INA219_CFGB_SADC_RES_AVG(15)    // 128 x 12-bit Shunt samples averaged together

#define INA219_CFG_MODE_MASK                    INA219_CFGB_MODE(7)             // Operating Mode Mask
#define INA219_CFG_MODE_POWERDOWN               INA219_CFGB_MODE(0)             // Power-Down
#define INA219_CFG_MODE_SVOLT_TRIGGERED         INA219_CFGB_MODE(1)             // Shunt Voltage, Triggered
#define INA219_CFG_MODE_BVOLT_TRIGGERED         INA219_CFGB_MODE(2)             // Bus Voltage, Triggered
#define INA219_CFG_MODE_SANDBVOLT_TRIGGERED     INA219_CFGB_MODE(3)             // Shunt and Bus, Triggered
#define INA219_CFG_MODE_ADCOFF                  INA219_CFGB_MODE(4)             // ADC Off (disabled)
#define INA219_CFG_MODE_SVOLT_CONTINUOUS        INA219_CFGB_MODE(5)             // Shunt Voltage, Continuous
#define INA219_CFG_MODE_BVOLT_CONTINUOUS        INA219_CFGB_MODE(6)             // Bus Voltage, Continuous
#define INA219_CFG_MODE_SANDBVOLT_CONTINUOUS    INA219_CFGB_MODE(7)             // Shunt and Bus, Continuous (default)

// Bus Voltage Register
#define INA219_BVOLT_CNVR                       0x0002       // Conversion Ready
#define INA219_BVOLT_OVF                        0x0001       // Math Overflow Flag

#define INA219_CONFIG_DEFAULT  0x399f

typedef struct 
{
  unsigned int ina219_config;
  unsigned int ina219_calbr;
  int shunt_raw;
  int voltage_raw;
  int power_raw;
  int current_raw;
  float shunt;
  long voltage;
  long power;
  long current;
}INA219_DATA;

void ina219_Write_Reg_Word(unsigned char iic_7bit_address, unsigned char reg, unsigned int data);

unsigned int ina219_Read_Reg_Word(unsigned char iic_7bit_address, unsigned char reg);

unsigned int ina219_Get_Config(unsigned char iic_7bit_address);

void ina219_SetCalibration_16V_32A(unsigned char iic_7bit_address );

void ina219_config(void);
int ina219_get_one_result_raw(unsigned char iic_7bit_address, unsigned char reg); 

void ina219_get_one_result(unsigned char iic_7bit_address, unsigned char reg, INA219_DATA *pina219data); 

void ina219_get_all_result(unsigned char iic_7bit_address, INA219_DATA *pina219data); 

void sci1_send_ina219_data_vccp_to_pc(unsigned char flag, INA219_DATA ina219_data);
void sci1_send_start_splitchar_to_pc(void);
void sci1_send_end_splitchar_to_pc(void);
void sci1_send_ina219_is_work(unsigned char kk);
void sci1_send_ina219_is_not_work(unsigned char kk);
#endif
