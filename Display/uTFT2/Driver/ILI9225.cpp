#include "ILI9225.h"

void TFT::ILI9225_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

	SPI.SendCmd16 (ILI9225_HORIZONTAL_WINDOW_ADDR1);
    SPI.SendData16( x1);
	
    SPI.SendCmd16 ( ILI9225_HORIZONTAL_WINDOW_ADDR2);
	SPI.SendData16( x0);
	
	SPI.SendCmd16 ( ILI9225_VERTICAL_WINDOW_ADDR1);
	SPI.SendData16( y1);
	
	SPI.SendCmd16 ( ILI9225_VERTICAL_WINDOW_ADDR2);
	SPI.SendData16( y0);
	
	SPI.SendCmd16 ( ILI9225_RAM_ADDR_SET1);
	SPI.SendData16( x0);
	
	SPI.SendCmd16 ( ILI9225_RAM_ADDR_SET2);
	SPI.SendData16( y0);
	
	SPI.SendCmd16 ( 0x0022);
	
}

void TFT::ILI9225_UpdateScreen(void)
{
	
	if (LCD->GPIO_CS != NULL) {
		CS_0;
	}
	
	ILI9225_setWindow(0, 0, 175, 219);

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
	
	if (LCD->Bit == 16)
	{
		for(uint16_t i = 0; i<176*220;i++)
		{
				LCD->hspi->Instance->DR = LCD->buffer16[i]; //4 bit
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
		}
	}


	if (LCD->GPIO_CS != NULL) {
		CS_1;
	}
	

}
