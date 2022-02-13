
#include "main.h"

#include "ST7735.h"
#include "ST7735_SPI.h"

#include "main.h"
#include "uTFT_define.h"

#define SPI_Delay 1

uint8_t madctlcurrent = MADVAL(MADCTLGRAPHICS);

__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
        micros *=(SystemCoreClock / 1000000) / 5;
        while (micros--){};
}





//void ST7735_Update(uTFT_LCD_t* LCD)
//{
////  uint32_t i;
////  uint32_t FS = 160*80;
////	
////  ST7735_AddrSet(0,0,80,80);
////	
////	A0_H;	
////	CS_L; 
////	  for (i = 0; i < FS; i++) 
////	   SpiWrite16(0x5a5a); 
////	CS_H;
	
	
	
	
//	
//	
//	
//	
//  ST7789_SendCmd(0x2A); // Column addr set
//  ST7789_SendData(0x00);     //??????? ??? XSTART
//  ST7789_SendData(TFT_XSTART);     //??????? ??? XSTART
//  ST7789_SendData(0x00);     //???????  ??? XEND
//  ST7789_SendData(TFT_XSTART+TFT_WIDTH-1);    //??????? ??? XEND

//  ST7789_SendCmd(0x2B); // Row addr set
//  ST7789_SendData(0x00);
//  ST7789_SendData(TFT_YSTART);
//  ST7789_SendData(0x00);
//  ST7789_SendData(TFT_YSTART+TFT_HEIGHT-1);
//        
//  ST7789_SendCmd(0x2C); //Memory write
//	SPI1->CR1 |= SPI_CR1_DFF; //16bit mode
//	
//  DATA;
////   for(i=0;i< (TFT_HEIGHT*TFT_WIDTH);i++)
////   {
////		 
////		// HAL_SPI_Transmit_DMA(&hspi1,(uint8_t *)LCD_Buffer, 32400*2);
////		// HAL_Delay(100);
////    //ST7789_write(LCD_Buffer[i]>>8);
////		while( !(SPI1->SR & SPI_SR_TXE)); 
////		LCD_SPI->DR = LCD_Buffer[i];
////	  
////  
////    // ST7789_write(LCD_Buffer[i] & 0xFF);
////		//while( !(SPI1->SR & SPI_SR_TXE));
////		//LCD_SPI->DR = LCD_Buffer[i] & 0xFF;
////	  
////   } 

//	uTFT_DMA_completed = 0;
//	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)LCD_Buffer, TFT_HEIGHT*TFT_WIDTH);
//	
//	while (uTFT_DMA_completed == 0)
//	{
//		//i++;
//	}



//SPI1->CR1 &= ~SPI_CR1_DFF; //8bit mode
//	 
//}






////uTFT_SetPixel
//void ST7735_SetPixel(uint16_t X, uint16_t Y, uint16_t color)
//{ ST7735_AddrSet(X,Y,X,Y); A0_H;	CS_L; SpiWrite16(color); CS_H; }

////uTFT_HLine
//void ST7735_HLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color)
//{ ST7735_AddrSet(X1,Y,X2,Y); A0_H; CS_L; for(uint16_t i = 0; i <= (X2 - X1); i++) SpiWrite16(color); CS_H; }

////uTFT_VLine
//void ST7735_VLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color)
//{ ST7735_AddrSet(X,Y1,X,Y2); A0_H; CS_L; for(uint16_t i = 0; i <= (Y2 - Y1); i++) SpiWrite16(color); CS_H; }


//void ST7735_PutChar5x7(uint8_t scale, uint16_t X, uint16_t Y, uint8_t chr, uint16_t color, uint16_t bgcolor)
//{
//  uint16_t i,j;
//  uint8_t buffer[5];

//  if ((chr >= 0x20) && (chr <= 0x7F))
//  {
//    // ASCII[0x20-0x7F]
//    memcpy(buffer,&Font5x7[(chr - 32) * 5], 5);
//  }
//  else if (chr >= 0xA0)
//  {
//    // CP1251[0xA0-0xFF]
//    memcpy(buffer,&Font5x7[(chr - 64) * 5], 5);
//  }
//  else
//  {
//    // unsupported symbol
//    memcpy(buffer,&Font5x7[160], 5);
//  }

//  // scale equals 1 drawing faster
//  if (scale == 1)
//  {
//    ST7735_AddrSet(X, Y, X + 5, Y + 7);
//		Spi8to16();
//	  A0_H;
//	  CS_L;
//    for (j = 0; j < 7; j++)
//    {
//      for (i = 0; i < 5; i++)
//      {
//        if ((buffer[i] >> j) & 0x01)
//        {
//					LCD_SPI->DR = color;
//          while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//        }
//        else
//        {
//					LCD_SPI->DR = bgcolor;
//          while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//        }
//      }
//      // vertical spacing
//			LCD_SPI->DR = bgcolor;
//      while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//    }

//    // horizontal spacing
//    for (i = 0; i < 6; i++)
//    {
//			LCD_SPI->DR = bgcolor;
//      while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//    }
//  }
//  else
//  {
//    for (j = 0; j < 7; j++)
//    {
//      for (i = 0; i < 5; i++)
//      {
//        // pixel group
//        ST7735_FillRect(X + (i * scale), Y + (j * scale), X + (i * scale) + scale - 1, Y + (j * scale) + scale - 1, ((buffer[i] >> j) & 0x01) ? color : bgcolor);
//      }
//      // vertical spacing
////      ST7735_FillRect(X + (i * scale), Y + (j * scale), X + (i * scale) + scale - 1, Y + (j * scale) + scale - 1, V_SEP);
//      ST7735_FillRect(X + (i * scale), Y + (j * scale), X + (i * scale) + scale - 1, Y + (j * scale) + scale - 1, bgcolor);
//    }
//    // horizontal spacing
////    ST7735_FillRect(X, Y + (j * scale), X + (i * scale) + scale - 1, Y + (j * scale) + scale - 1, H_SEP);
//    ST7735_FillRect(X, Y + (j * scale), X + (i * scale) + scale - 1, Y + (j * scale) + scale - 1, bgcolor);
//  }
//  CS_H;
//	Spi16to8();
//}

//void ST7735_PutStr5x7(uint8_t scale, uint8_t X, uint8_t Y, char *str, uint16_t color, uint16_t bgcolor)
//{
//  // scale equals 1 drawing faster
//  if (scale == 1)
//  {
//    while (*str)
//    {
//      ST7735_PutChar5x7(scale, X,Y,*str++,color,bgcolor);
//      if (X < 128 - 6) { X += 6; } else if (Y < 128 - 8) { X = 0; Y += 8; } else { X = 0; Y = 0; }
//    };
//  }
//  else
//  {
//    while (*str)
//    {
//      ST7735_PutChar5x7(scale, X,Y,*str++,color,bgcolor);
//      if (X < 128 - (scale*5) + scale) { X += (scale * 5) + scale; } else if (Y < 128 - (scale * 7) + scale) { X = 0; Y += (scale * 7) + scale; } else { X = 0; Y = 0; }
//    };
//  }
//}


//void ST7735_PutChar7x11(uint16_t X, uint16_t Y, uint8_t chr, uint16_t color, uint16_t bgcolor)
//{
//  uint16_t i,j;
//  uint8_t buffer[11];
//  if ((chr >= 0x20) && (chr <= 0x7F))
//  {
//    // ASCII[0x20-0x7F]
//    memcpy(buffer,&Font7x11[(chr - 32) * 11], 11);
//  }
//  else if (chr >= 0xA0)
//  {
//    // CP1251[0xA0-0xFF]
//    memcpy(buffer,&Font7x11[(chr - 64) * 11], 11);
//  }
//  else
//  {
//    // unsupported symbol
//    memcpy(buffer,&Font7x11[160], 11);
//  }

//  ST7735_AddrSet(X, Y, X + 7, Y + 11);
//	Spi8to16();
//	A0_H;
//	CS_L;
//  for (i = 0; i < 11; i++)
//  {
//    for (j = 0; j < 7; j++)
//    {
//      if ((buffer[i] >> j) & 0x01)
//      {
//        LCD_SPI->DR = color;
//        while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//      }
//      else
//      {
//        LCD_SPI->DR = bgcolor;
//        while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//      }
//    }
//    // vertical spacing
//    LCD_SPI->DR = bgcolor;
//    while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//  }

//  // horizontal spacing
//  for (i = 0; i < 8; i++)
//  {
//    LCD_SPI->DR = bgcolor;
//    while(!(LCD_SPI->SR & SPI_FLAG_TXE));
//  }

//  CS_H;
//	Spi16to8();
//}

//void ST7735_PutStr7x11(uint8_t X, uint8_t Y, char *str, uint16_t color, uint16_t bgcolor)
//{
//  while (*str)
//  {
//    ST7735_PutChar7x11(X,Y,*str++,color,bgcolor);
//    if (X < 128 - 8) { X += 8; } else if (Y < 128 - 12) { X = 0; Y += 12; } else { X = 0; Y = 0; }
//  };
//}







