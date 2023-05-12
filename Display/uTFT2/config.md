Создаем в ini

TFT_config.h

	#ifdef TFT_USE_SPI
	  SPI_HandleTypeDef *hspi;
    #endif

>>
#ifndef INC_TFT_CONFIG_H_
#define INC_TFT_CONFIG_H_

//#define TFT_USE_SPI
#define TFT_USE_I2С

//#define LCD_USE_BL



//
#define TFT_Driver_SSD1306
//#define TFT_Driver_ILI9225
//#define TFT_Driver_ST7789
//#define TFT_Driver_ST7735
//#define TFT_Driver_ST7735S





#endif /* INC_TFT_CONFIG_H_ */


TFT tft;

Инициализация
tft.init(&LCD_0);










/////////////////////////////////////////////
//SSD1306 128 64 1 bit
uint8_t LCD_Buffer[128 * 8 + 16];

uTFT_LCD_t LCD_0 = {
		128,                  // Ширина экрана
		64,                   // Высота экрана
		SSD1306,              // Драйвер
		1,                    // bit
		NULL,                 // notUse
		&hi2c1,	              // I2C
		0x78,                 // адресс
		NULL,                 // 16 бит буффер
		&LCD_Buffer[0],       // !16 бит буффер
};

////////////////////////////////////////////////////////////
//ST7789 135 240 16 bit
uint16_t LCD_Buffer16_0[240 * 136 + 16];
//uint16_t LCD_Buffer16_0[1];

uTFT_LCD_t LCD_0 = { 135,                  // Ширина экрана
		240,                  // Высота экрана
		ST7789,               // Драйвер
		16,                   // bit
		&hspi5,               // Spi
		NULL,	              // I2C
		&LCD_Buffer16_0[6],   // 16 бит буффер
		NULL,                 // !16 бит буффер
		NULL,                 // Палитра
		52,                   // Смещение по X
		40,                   // Смещение по Y
		CS_GPIO_Port,         // CS PORT
		CS_Pin,               // CS PIN
		DC_GPIO_Port,         // DC PORT
		DC_Pin,               // DC PIN
		RESET_GPIO_Port,      // RESET PORT
		RESET_Pin,	          // RESET PIN
		0, {                     //MADCTL
		0,   //MY
				0,   //MX
				0,   //MV
				0,   //ML
				0,   //RGB
				0,   //MH
		}, {                     //Смещение но X Y
		135, 240, 52,  //DX_0
				40,  //DY_0
				240, 135, 40,   //DX_90
				53,   //DY_90
				135, 240, 53,	 //DX_180
				40,   //DY_180
				240, 135, 40,   //DX_270
				52,   //DY_270
		} };

