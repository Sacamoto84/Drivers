#include "ILI9225.h"

#include "font5x7.h"
#include "font7x11.h"
#include "main.h"
#include "res.h"
#include "uTFT_define.h"



void ILI9225_setWindow(uTFT_LCD_t * LCD, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

	LCD_SPI_SendCmd16 ( LCD, ILI9225_HORIZONTAL_WINDOW_ADDR1);
  LCD_SPI_SendData16( LCD, x1);
	
	LCD_SPI_SendCmd16 ( LCD, ILI9225_HORIZONTAL_WINDOW_ADDR2);
  LCD_SPI_SendData16( LCD, x0);
	
	LCD_SPI_SendCmd16 ( LCD, ILI9225_VERTICAL_WINDOW_ADDR1);
  LCD_SPI_SendData16( LCD, y1);
	
	LCD_SPI_SendCmd16 ( LCD, ILI9225_VERTICAL_WINDOW_ADDR2);
  LCD_SPI_SendData16( LCD, y0);
	
	LCD_SPI_SendCmd16 ( LCD, ILI9225_RAM_ADDR_SET1);
  LCD_SPI_SendData16( LCD, x0);
	
	LCD_SPI_SendCmd16 ( LCD, ILI9225_RAM_ADDR_SET2);
  LCD_SPI_SendData16( LCD, y0);
	
	LCD_SPI_SendCmd16 ( LCD, 0x0022);
	
}

void ILI9225_UpdateScreen(uTFT_LCD_t * LCD)
{
	
	if (LCD->GPIO_CS != NULL)	
	  LCD_Select(LCD);
	
	ILI9225_setWindow(LCD, 0, 0, 175, 219);

	///A0_H;   // dc 1 = data, 0 = control
	///CS_L;
	
	
	
	DATA;
	
	if (LCD->Bit == 4)
	{
		for(uint16_t i = 0; i<176*220;i++)
		{
			if (i % 2)
			{
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i/2]) & 0x0F]; //4 bit
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
			}
			else
			{
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i/2]) >> 4]; //4 bit
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
			}
		}
	}
	
		if (LCD->GPIO_CS != NULL)	
	  LCD_Select(LCD);
	

}
