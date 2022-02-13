

//#include "stm32f1xx_hal_spi.h"

#include "ST7735.h"

#include "main.h"

//__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
//{
//        micros *=(SystemCoreClock / 1000000) / 5;
//        while (micros--){};
//}

//__STATIC_INLINE void DelayNano(__IO uint32_t micros)
//{
//        micros *=(SystemCoreClock / 1000000) / 50;
//        while (micros--){};
//}

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

//#if STM32F103xB
//void Spi8to16(void)
//{
//	LCD_SPI->CR1 &= ~SPI_CR1_SPE;
//	LCD_SPI->CR1 |=  SPI_CR1_DFF;
//	LCD_SPI->CR1 |=  SPI_CR1_SPE;
//}

//void Spi16to8(void)
//{
//	LCD_SPI->CR1 &= ~SPI_CR1_SPE;
//	LCD_SPI->CR1 &= ~SPI_CR1_DFF;
//	LCD_SPI->CR1 |=  SPI_CR1_SPE;
//}
//#endif

//#if STM32F373xC
//void Spi8to16(void)
//{
////	LCD_SPI->CR1 &= ~SPI_CR1_SPE; //OFF SPI
////	LCD_SPI->CR2 |=  SPI_CR2_DS_3;
////	LCD_SPI->CR1 |=  SPI_CR1_SPE; //ON  SPI
//}

//void Spi16to8(void)
//{
////	LCD_SPI->CR1 &= ~SPI_CR1_SPE; //OFF SPI
////	LCD_SPI->CR2 &= ~SPI_CR2_DS_3;
////	LCD_SPI->CR1 |=  SPI_CR1_SPE; //ON  SPI
//}
//#endif

//void LcdWrite(char dc, const char *data, int nbytes)
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

//void ST7735_writeCmd(uint8_t c)
//{
//	A0_L;   // dc 1 = data, 0 = control
//	CS_L;
//	SpiWrite(c);
//	CS_H;
//}


//void ST7735_cmd(uint8_t cmd)
//{
//  LcdWrite(0, (uint8_t *)cmd, 1);
//}

//void ST7735_data(uint8_t data)
//{
//  LcdWrite(1, (uint8_t *)data, 1);
//}

//void ST7735_write(uint8_t data)
//{
//  HAL_SPI_Transmit(&LCD_SPI_Handle, (uint8_t *)data, 1, 1000);
//}