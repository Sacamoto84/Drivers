#ifndef HTU21D_H
#define HTU21D_H

#if defined(STM32F103xB)
  #include "stm32f1xx_hal.h"	
#elif defined(STM32F373xC)
  #include "stm32f3xx_hal.h"
#endif

extern I2C_HandleTypeDef hi2c1;

typedef enum Si7021_commands
{
  Humi_HM        = 0xE5, // Measure Relative Humidity, Hold Master Mode
  Humi_NHM       = 0xF5, // Measure Relative Humidity, No Hold Master Mode
  Temp_HM        = 0xE3, // Measure Temperature, Hold Master Mode
  Temp_NHM       = 0xF3, // Measure Temperature, No Hold Master Mode
  Temp_AH        = 0xE0, // Read Temperature Value from Previous RH Measurement
  Si7021_Reset   = 0xFE, // Reset
  W_RHT_U_reg    = 0xE6, // Write RH/T User Register 1
  R_RHT_U_reg    = 0xE7, // Read RH/T User Register 1
  W_Heater_C_reg = 0x51, // Write Heater Control Register
  R_Heater_C_reg = 0x11, // Read Heater Control Register
  R_ID_Byte11    = 0xFA, // Read Electronic ID 1st Byte, first part
  R_ID_Byte12    = 0x0F, // Read Electronic ID 1st Byte, second part
  R_ID_Byte21    = 0xFC, // Read Electronic ID 2nd Byte, first part
  R_ID_Byte22    = 0xC9, // Read Electronic ID 2nd Byte, second part
  R_Firm_rev1    = 0x84, // Read Firmware Revision, first part
  R_Firm_rev2    = 0xB8  // Read Firmware Revision, second part
}Si7021_commands_t;

//#define HTU21D_ADDRESS         ( (uint8_t ) 0x80 )
#define HTU21D_ADDRESS              0x80
//I2C_Master_BufferWriteRead ( ADDR_SI7021, Humi_HM, result, 2 );


#endif