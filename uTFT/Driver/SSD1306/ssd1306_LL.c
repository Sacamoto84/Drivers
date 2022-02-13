//#include "global.h"
#include "ssd1306.h"
#include "uTFT_define.h"

#define SSD1306_I2C_ADDR         0x78 //0x78
#define SSD1306_I2C_ADDR2        0x7A //0x7A

void SSD1306_WRITECOMMAND(uTFT_LCD_t* LCD, uint8_t data)
{
	if (LCD->hi2c)  //I2C
	{
		uint8_t dt[2];
		dt[0] = 0;
		dt[1] = data;
		HAL_I2C_Master_Transmit(LCD->hi2c, LCD->I2C_Adress, dt, 2, 1000);
	}
	else                          //SPI
	{
    LCD_SPI_SendCmd(LCD, data);	
	}
	
}

void SSD1306_WRITEDATA(uTFT_LCD_t* LCD, uint8_t data)
{
	if (LCD->hi2c)  //I2C
	{
		uint8_t dt[2];
		dt[0] = 0x40;
		dt[1] = data;
		HAL_I2C_Master_Transmit(LCD->hi2c, LCD->I2C_Adress, dt, 2, 1000);
	}
	else                          //SPI
	{
	  LCD_SPI_SendData(LCD, data);
	}
	
}

//Инициализация первого экрана
void SSD1306_Init(uTFT_LCD_t* LCD) {
	
	//Только для SPI
	if (LCD->GPIO_RESET != NULL)	
	  LCD_Reset(LCD);
	
	if (LCD->GPIO_CS != NULL)	
	  LCD_Select(LCD);
	
	/* Init LCD */
	SSD1306_WRITECOMMAND(LCD, 0xAE); //display off
	SSD1306_WRITECOMMAND(LCD, 0x20); //Set Memory Addressing Mode   
	SSD1306_WRITECOMMAND(LCD, 0x00); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	SSD1306_WRITECOMMAND(LCD, 0xB0); //Set Page Start Address for Page Addressing Mode,0-7
	SSD1306_WRITECOMMAND(LCD, 0xC8); //Set COM Output Scan Direction
	SSD1306_WRITECOMMAND(LCD, 0x00); //---set low column address
	SSD1306_WRITECOMMAND(LCD, 0x10); //---set high column address
	SSD1306_WRITECOMMAND(LCD, 0x40); //--set start line address
	SSD1306_WRITECOMMAND(LCD, 0x81); //--set contrast control register
	SSD1306_WRITECOMMAND(LCD, 0xFF);
	SSD1306_WRITECOMMAND(LCD, 0xA1); //--set segment re-map 0 to 127
	SSD1306_WRITECOMMAND(LCD, 0xA6); //--set normal display
	SSD1306_WRITECOMMAND(LCD, 0xA8); //--set multiplex ratio(1 to 64)
	
	if (LCD->TFT_HEIGHT == 64)
	  SSD1306_WRITECOMMAND(LCD, 0x3F); //64
	else
	  SSD1306_WRITECOMMAND(LCD, 0x1F);   //32
	
	SSD1306_WRITECOMMAND(LCD, 0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	SSD1306_WRITECOMMAND(LCD, 0xD3); //-set display offset
	SSD1306_WRITECOMMAND(LCD, 0x00); //-not offset
	SSD1306_WRITECOMMAND(LCD, 0xD5); //--set display clock divide ratio/oscillator frequency
	SSD1306_WRITECOMMAND(LCD, 0xF0); //--set divide ratio
	SSD1306_WRITECOMMAND(LCD, 0xD9); //--set pre-charge period
	SSD1306_WRITECOMMAND(LCD, 0x22); //
	
	SSD1306_WRITECOMMAND(LCD, 0xDA); //--set com pins hardware configuration
	if (LCD->TFT_HEIGHT == 64)
	  SSD1306_WRITECOMMAND(LCD, 0x12); //64
	else
	  SSD1306_WRITECOMMAND(LCD, 0x02); //32
	
	SSD1306_WRITECOMMAND(LCD, 0xDB); //--set vcomh
	SSD1306_WRITECOMMAND(LCD, 0x20); //0x20,0.77xVcc
	SSD1306_WRITECOMMAND(LCD, 0x8D); //--set DC-DC enable
	SSD1306_WRITECOMMAND(LCD, 0x14); //
	SSD1306_WRITECOMMAND(LCD, 0xAF); //--turn on SSD1306 panel
		
	if (LCD->GPIO_CS != NULL)	
	  LCD_Unselect(LCD);	
		
	/* Set default values */
	//uTFT.CurrentX = 0;
	//uTFT.CurrentY = 0;
}

//490uS-72MHz
void SSD1306_UpdateScreen(uTFT_LCD_t* LCD) {
volatile uint8_t * p;	 
	
	if (LCD->hi2c)  //I2C
	{
		  SSD1306_WRITECOMMAND(LCD, 0xB0);
		  SSD1306_WRITECOMMAND(LCD, 0x00);
		  SSD1306_WRITECOMMAND(LCD, 0x10);
		
		#if defined(LCD_osDelay)
			osDelay(2);
		#else
		  HAL_Delay(2);
		#endif
		
			p = &LCD->buffer8[0];
			p--;
			HAL_I2C_Master_Transmit(LCD->hi2c, LCD->I2C_Adress, p, (LCD->TFT_HEIGHT * LCD->TFT_WIDTH) / 8 + 1, 1000);	
		  return;
		
	}
	else 
	{ //SPI
    if (LCD->GPIO_CS != NULL)	
	  LCD_Select(LCD);
		
		SSD1306_WRITECOMMAND(LCD, 0xB0);
		SSD1306_WRITECOMMAND(LCD, 0x00);
		SSD1306_WRITECOMMAND(LCD, 0x10);
		
		HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_SET);
		uint8_t * p;
		p = &LCD->buffer8[0];
		p--;			
		for(uint16_t i = 0; i < 1025; i++)
		{			
		  LCD->hspi->Instance->DR = *p++;		
      while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );					
		}	
		
		if (LCD->GPIO_CS != NULL)	
	  LCD_Unselect(LCD);
	}				
	
}

void SSD1306_Contrast(uTFT_LCD_t* LCD, uint8_t c)
{
		SSD1306_WRITECOMMAND(LCD, 0x81);
		SSD1306_WRITECOMMAND(LCD, c);
}


