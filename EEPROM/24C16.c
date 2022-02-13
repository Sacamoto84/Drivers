#include "stm32f3xx_hal.h"
#include "main.h"
#include <math.h>

extern I2C_HandleTypeDef hi2c2;

void EEPROM_24C16_I2C_Write(uint8_t reg, uint8_t data) {
	HAL_I2C_Mem_Write(&hi2c2, (uint16_t)0xA0, reg, 1, &data, 1, 1000);
	HAL_Delay(12);
}

uint16_t  EEPROM_24C16_I2C_Read(uint8_t reg) {
  uint8_t dt;
  HAL_I2C_Mem_Read(&hi2c2, (uint16_t)0xA0, reg, 1, &dt, 1, 1000);
  return dt;	
}

void EEPROM_24C32_I2C_Write(uint16_t reg, uint8_t data) {
	
	HAL_I2C_Mem_Write(&hi2c2, (uint16_t)0xA0, reg, 2, &data, 1, 1000);
	HAL_Delay(12);
}

uint16_t  EEPROM_24C32_I2C_Read(uint16_t reg) {
  uint8_t dt;
  HAL_I2C_Mem_Read(&hi2c2, (uint16_t)0xA0, reg, 2, &dt, 1, 1000);
  return dt;	
}