#include "uTFT.h"
/*

void Lcd_writeCmd8(uint8_t c);
void Lcd_writeCmd16(uint16_t c);

extern SPI_HandleTypeDef hspi1;

#define LCD_SPI SPI1
#define LCD_SPI_Handle hspi1

__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
        micros *=(SystemCoreClock / 1000000) / 5;
        while (micros--);
}

__STATIC_INLINE void DelayNano(__IO uint32_t micros)
{
        micros *=(SystemCoreClock / 1000000) / 50;
        while (micros--);
}

// Только для порта настроенного на 16 бит
void LCD_SPI_SendData16(uTFT_LCD_t * LCD, uint16_t data)
{
  /////DATA;
	/////__nop;
	//LCD->hspi->Instance->DR = data;
	//while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
}

// Только для порта настроенного на 16 бит
void LCD_SPI_SendCmd16(uTFT_LCD_t * LCD, uint16_t cmd)
{
   ///// CMD;
	/////__nop;
	//LCD->hspi->Instance->DR = cmd;
	//while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0);
}

*/

//void SpiWrite(uint8_t data)
//{ 
//	LCD_SPI->DR = data;
//	while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//}

//void SpiWrite16(uint16_t data)
//{  
//	LCD_SPI->DR = (uint8_t)(data>>8);
//  while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//  LCD_SPI->DR = (uint8_t)data;
//  while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//}

//void SpiWrite(uint16_t data)
//{ 
//	//while(   LCD_SPI->SR & SPI_FLAG_BSY ); // wait until SPI is not busy anymore
//  //while( !(LCD_SPI->SR & SPI_FLAG_TXE) ); // wait until transmit complete
//	LCD_SPI->DR = data;
//	//while(   LCD_SPI->SR & SPI_FLAG_BSY ); // wait until SPI is not busy anymore	
//	while( (LCD_SPI->SR & SPI_FLAG_TXE) == 0 ); // wait until transmit complete
//	DelayMicro(1);
//}

//void SpiWrite16(uint16_t data)
//{  
//	//while(   LCD_SPI->SR & SPI_FLAG_BSY ); // wait until SPI is not busy anymore
//  //while( !(LCD_SPI->SR & SPI_FLAG_TXE) ); // wait until transmit complete
//	LCD_SPI->DR = (uint8_t)(data>>8);
//	//while(   LCD_SPI->SR & SPI_FLAG_BSY ); // wait until SPI is not busy anymore
//	while( !(LCD_SPI->SR & SPI_FLAG_TXE) ); // wait until transmit complete
//	DelayMicro(40);
//	
//  LCD_SPI->DR = (uint8_t)data;
//	//while(   LCD_SPI->SR & SPI_FLAG_BSY ); // wait until SPI is not busy anymore
//	while( !(LCD_SPI->SR & SPI_FLAG_TXE) ); // wait until transmit complete
//	DelayMicro(40);
//	
//}

//void LcdWrite(char dc, uint8_t *data, int nbytes)
//{  
//	DelayMicro(10);
//	A0_DC;   // dc 1 = data, 0 = control
//	DelayMicro(10);
//	CS_L;
//	DelayMicro(10);
//	  HAL_SPI_Transmit(&LCD_SPI_Handle, (uint8_t *)data, nbytes, 1000);
//	DelayMicro(10);
//	CS_H;
//	DelayMicro(10);
//}

//void LcdWrite16_1(char dc, uint16_t data)
//{
//  //A0_DC;  // dc 1 = data, 0 = control
//	A0_H;
//	CS_L;
//	SpiWrite(data>>8);
//	SpiWrite((uint8_t)data);
//  CS_H;
//}

//void Lcd_writeCmd8(uint8_t c)
//{
//	A0_L;   // dc 1 = data, 0 = control
//	CS_L;
//	SpiWrite(c);
//	CS_H;
//}

//void Lcd_writeData8(uint8_t c)
//{
//	A0_H;   // dc 1 = data, 0 = control
//	CS_L;
//	SpiWrite(c);
//	CS_H;
//}

//void Lcd_writeCmd16(uint16_t c)
//{
//	A0_L;   // dc 1 = data, 0 = control
//	CS_L;
//	SpiWrite(c);
//	CS_H;
//}

//void Lcd_writeData16(uint16_t c)
//{
//	A0_H;   // dc 1 = data, 0 = control
//	CS_L;
//	SpiWrite(c);
//	CS_H;
//}

//void Lcd_writeCmdData(uint16_t cmd, uint16_t dat)
//{   
//    Lcd_writeCmd16(cmd);
//    Lcd_writeData16(dat); 
//}

//void ST7735_cmd(uint8_t cmd)
//{
//  LcdWrite(0, &cmd, 1);
//}

//void ST7735_data(uint8_t data)
//{
//  //LcdWrite(1, (uint8_t *)data, 1);
//	LcdWrite(1, &data, 1);
//}

//void ST7735_write(uint8_t data)
//{
//  HAL_SPI_Transmit(&LCD_SPI_Handle, &data, 1, 1000);
//}

#if STM32F103xB1
void Spi8to16(void)
{
	LCD_SPI->CR1 &= ~SPI_CR1_SPE;
	LCD_SPI->CR1 |=  SPI_CR1_DFF;
	LCD_SPI->CR1 |=  SPI_CR1_SPE;
}

void Spi16to8(void)
{
	LCD_SPI->CR1 &= ~SPI_CR1_SPE;
	LCD_SPI->CR1 &= ~SPI_CR1_DFF;
	LCD_SPI->CR1 |=  SPI_CR1_SPE;
}
#endif

#if STM32F373xC
void Spi8to16(void)
{
//	LCD_SPI->CR1 &= ~SPI_CR1_SPE; //OFF SPI
//	LCD_SPI->CR1 |=  SPI_CR2_DFF;
//	LCD_SPI->CR1 |=  SPI_CR1_SPE; //ON  SPI
}

void Spi16to8(void)
{
//	LCD_SPI->CR1 &= ~SPI_CR1_SPE; //OFF SPI
//	LCD_SPI->CR2 &= ~SPI_CR2_DFF;
//	LCD_SPI->CR1 |=  SPI_CR1_SPE; //ON  SPI
}
#endif





