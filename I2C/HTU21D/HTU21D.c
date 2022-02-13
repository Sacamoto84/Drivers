#include "HTU21D.h"

float humidity;
float temperature;

//Перезагрузка датчика
void HTU21D_Reset(void)
{
  uint8_t data[] = {0xFE,0};
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, HTU21D_ADDRESS, data,1,100);
  HAL_Delay(20);
}

//Now we will read user register
void HTU21D_ReadRegister(void)
{
	uint8_t data[2];
  data[0] = 0xE7;

  HAL_I2C_Master_Transmit(&hi2c1,HTU21D_ADDRESS,data,1,100);//write command
  HAL_I2C_Master_Receive(&hi2c1,HTU21D_ADDRESS,data,1,100);//receive data
}

void HTU21D_ReadTemperature(void)
{
	static uint8_t data[3];
  data[0] = 0xF3; //Measure Temperature, No Hold Master Mode
	HAL_I2C_Master_Transmit(&hi2c1,HTU21D_ADDRESS,data,1,1000);//write command
	HAL_Delay(100);
	//HAL_I2C_Master_Receive(&hi2c1,HTU21D_ADDRESS,data,0,1000);//write command
	//HAL_Delay(100);
	HAL_I2C_Master_Receive(&hi2c1,HTU21D_ADDRESS,data,3,1000);//write command
	HAL_Delay(100);
	
	static uint16_t result;
	result = (data[0]<<8) + (data[1]>>2);
	
	temperature = -46.85 + 175.72*result/0xFFFF;
}

void HTU21D_ReadHumidity(void)
{
	static uint8_t data[3];
  data[0] = 0xF5; //Measure Temperature, No Hold Master Mode
	HAL_I2C_Master_Transmit(&hi2c1,HTU21D_ADDRESS,data,1,1000);//write command
	HAL_Delay(100);
	//HAL_I2C_Master_Receive(&hi2c1,HTU21D_ADDRESS,data,0,1000);//write command
	//HAL_Delay(100);
	HAL_I2C_Master_Receive(&hi2c1,HTU21D_ADDRESS,data,3,1000);//write command
	HAL_Delay(100);
	
	static uint16_t result;
	result = (data[0]<<8) + (data[1]>>2);
	
	humidity = -6.0 + 125.0*result/0xFFFF;
	
}