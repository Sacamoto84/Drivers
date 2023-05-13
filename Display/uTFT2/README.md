



# Инициализация 

#### TFT tft;

#### tft.init(&LCD_0);











SSD1306
========================
	
```c
//SSD1306 128 64 1 bit
uint8_t LCD_Buffer[128 * 8];
uTFT_LCD_t LCD_0 = {
      128,                  // Ширина экрана
      64,                   // Высота экрана
      SSD1306,              // Драйвер
      1,                    // bit
      NULL,                 // notUse (SPI)
      &hi2c1,	            // I2C
      0x78,                 // адресс
      NULL,                 // 16 бит буффер
      &LCD_Buffer[0],       // !16 бит буффер
};
```





