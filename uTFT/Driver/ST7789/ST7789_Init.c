#include "uTFT_define.h"
#include "ST7789.h"

#define DELAY 0x80

void ST7789_Init(uTFT_LCD_t * LCD)
{
	
	if (LCD->GPIO_CS != NULL)
	{
		LCD_Select(LCD);
	}

	if (LCD->GPIO_RESET != NULL)
	{
		LCD_Reset(LCD);
	}
	
	LCD_SPI_SendCmd(LCD, 0x01);   //SWRESET
	HAL_Delay(150);   
	
	LCD_SPI_SendCmd(LCD, 0x11);   //SLPOUT
	HAL_Delay(150);
	
	LCD_SPI_SendCmd (LCD, 0x3A);   //COLMOD RGB444(12bit) 0x03, RGB565(16bit) 0x05, 
	LCD_SPI_SendData(LCD, 0x05);  //RGB666(18bit) 0x06        
	LCD_SPI_SendCmd (LCD, 0x36);   //MADCTL
	LCD_SPI_SendData(LCD, 0x08);  //0x08 B-G-R, 0x14 R-G-B  14            	

	LCD_SPI_SendCmd (LCD, ST77XX_GAMSET);               // ST77XX_GAMSET         0x26
	LCD_SPI_SendData(LCD, 0x02); // Gamma curve 2 (G1.8)  ST77XX_DGMEN          0xBA
	LCD_SPI_SendCmd (LCD, ST77XX_DGMEN);
	LCD_SPI_SendData(LCD, 0x04); // Enable gamma
	 
	LCD_SPI_SendCmd (LCD, 0xe0);   // Positive Voltage Gamma Control
	LCD_SPI_SendData(LCD, 0xd0);
	LCD_SPI_SendData(LCD, 0x00);
	LCD_SPI_SendData(LCD, 0x03);
	LCD_SPI_SendData(LCD, 0x08);
	LCD_SPI_SendData(LCD, 0x0a);
	LCD_SPI_SendData(LCD, 0x17);
	LCD_SPI_SendData(LCD, 0x2e);
	LCD_SPI_SendData(LCD, 0x44);
	LCD_SPI_SendData(LCD, 0x3f);
	LCD_SPI_SendData(LCD, 0x29);
	LCD_SPI_SendData(LCD, 0x10);
	LCD_SPI_SendData(LCD, 0x0e);
	LCD_SPI_SendData(LCD, 0x14);
	LCD_SPI_SendData(LCD, 0x18);

	LCD_SPI_SendCmd(LCD, 0xe1);   // Negative Voltage Gamma Control
	//See datasheet for more information
	LCD_SPI_SendData(LCD, 0xd0);
	LCD_SPI_SendData(LCD, 0x00);
	LCD_SPI_SendData(LCD, 0x03);
	LCD_SPI_SendData(LCD, 0x08);
	LCD_SPI_SendData(LCD, 0x0a);
	LCD_SPI_SendData(LCD, 0x17);
	LCD_SPI_SendData(LCD, 0x2e);
	LCD_SPI_SendData(LCD, 0x44);
	LCD_SPI_SendData(LCD, 0x3f);
	LCD_SPI_SendData(LCD, 0x29);
	LCD_SPI_SendData(LCD, 0x10);
	LCD_SPI_SendData(LCD, 0x0e);
	LCD_SPI_SendData(LCD, 0x14);
	LCD_SPI_SendData(LCD, 0x18);


	LCD_SPI_SendCmd(LCD, 0x11); // Exit Sleep Mode
	//HAL_Delay(120);
	HAL_Delay(350);

	LCD_SPI_SendCmd(LCD, 0x29); // Display on
	HAL_Delay(350);//HAL_Delay(120);

	LCD_SPI_SendCmd(LCD, 0x21);   //INVON
	LCD_SPI_SendCmd(LCD, 0x13);   //NORON   
	LCD_SPI_SendCmd(LCD, 0x29);   //DISPON
 
 	if (LCD->GPIO_CS != NULL)
	{
		LCD_Unselect(LCD);
	}
 
}


void ST7789_AddrSet(uTFT_LCD_t* LCD, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	LCD_SPI_SendCmd (LCD, 0x2A);        
  LCD_SPI_SendData(LCD, 0x00);         
  LCD_SPI_SendData(LCD, x0 + LCD->dx );  	
  LCD_SPI_SendData(LCD, 0x00);          
  LCD_SPI_SendData(LCD, x1 + LCD->dx);  	
  LCD_SPI_SendCmd (LCD, 0x2B);         
  LCD_SPI_SendData(LCD, 0x00);          	
  LCD_SPI_SendData(LCD, y0 + LCD->dy); 	
  LCD_SPI_SendData(LCD, 0x00);         	
  LCD_SPI_SendData(LCD, y1 + LCD->dy);  	      
  LCD_SPI_SendCmd(LCD, 0x2C);
}


void ST7789_Update(uTFT_LCD_t* LCD)
{
	if (LCD->GPIO_CS != NULL)
	{
	  LCD_Select(LCD);
	}
	
  uint32_t i;

	uint8_t HI;
	uint8_t LO;
	
	HI = (LCD->dx + LCD->TFT_WIDTH - 1) >> 8;
	LO = (LCD->dx + LCD->TFT_WIDTH - 1) & 0xFF;

  LCD_SPI_SendCmd (LCD, 0x2A); // Column addr set
  LCD_SPI_SendData(LCD, 0x00);     //??????? ??? XSTART
  LCD_SPI_SendData(LCD, LCD->dx);     //??????? ??? XSTART
  LCD_SPI_SendData(LCD, HI );     //???????  ??? XEND
  LCD_SPI_SendData(LCD, LO );    //??????? ??? XEND
	

	
  LCD_SPI_SendCmd (LCD, 0x2B); // Row addr set
  LCD_SPI_SendData(LCD, 0x00);
  LCD_SPI_SendData(LCD, LCD->dy); //TFT_YSTART
  LCD_SPI_SendData(LCD, 0x00);
  LCD_SPI_SendData(LCD, LCD->dy+LCD->TFT_HEIGHT - 1); //TFT_YSTART
        
	//ST7789_AddrSet(LCD, 0, 0, LCD->TFT_WIDTH - 1, LCD->TFT_HEIGHT - 1);
	
 LCD_SPI_SendCmd(LCD, 0x2C); //Memory write
	
	//LCD->hspi->Instance->CR1 |= SPI_CR1_DFF; 
	
  HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_SET); //DATA
	
	Spi8to16(LCD); //16bit mode
	
	if (LCD->Bit == 4)
	{	
		for(uint16_t i = 0; i<(LCD->TFT_HEIGHT * LCD->TFT_WIDTH)-1;i++)
		{
			if (i % 2)
			{
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i/2]) & 0x0F]; //4 bit
			}
			else
			{
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i/2]) >> 4]; //4 bit
			}
		}
	}
	
	if (LCD->Bit == 16)
	{
   for(i = 0;i< (LCD->TFT_HEIGHT * LCD->TFT_WIDTH); i++)
   { 
		 // HAL_SPI_Transmit_DMA(&hspi1,(uint8_t *)LCD_Buffer, 32400*2);
		 while( !(LCD->hspi->Instance->SR & SPI_SR_TXE)); 
		 LCD->hspi->Instance->DR = LCD->buffer16[i];  
   } 
  } 
	 
	 
	 
	 
	 
	 

////	uTFT_DMA_completed = 0;
////	
////	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)LCD->buffer16[0], LCD->TFT_HEIGHT*LCD->TFT_WIDTH);
////	
////	while (uTFT_DMA_completed == 0)
////	{
////	}
	 while( !(LCD->hspi->Instance->SR & SPI_SR_TXE));
	Spi16to8(LCD); //8bit mode
 
	if (LCD->GPIO_CS != NULL)
	{
	  LCD_Unselect(LCD);
	}
	 
}

//void ST7789_Update_DMA_Cicle_On(void)
//{
//	
//	DMA2_Stream3->CR &= ~DMA_SxCR_EN;     //Отключаем DMA
//  while( !(SPI1->SR & SPI_SR_TXE));	    //Ждем окончания передачи по SPI
//	DMA2_Stream3->NDTR = 0;               //Сброс счетчика DMA
//	SPI1->CR1 &= ~SPI_CR1_SPE;            //Спокойно отключаем SPI
//	SPI1->CR2  &= ~SPI_CR2_TXDMAEN;       //Отвязываем от DMA
//	SPI1->CR1 &= ~SPI_CR1_DFF;            //8bit mode
//	SPI1->CR1 |= SPI_CR1_SPE;             //Включаем для работы в обычном режиме
//	
//	
//	ST7789_SendCmd(0x2A); // Column addr set
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
//	
//	while( !(SPI1->SR & SPI_SR_TXE));       //Ждем окончания передачи по SPI
//	SPI1->CR1 &= ~SPI_CR1_SPE;              //Спокойно отключаем SPI
//	SPI1->CR1 |= SPI_CR1_DFF;               //16bit mode
//	
//	DATA;
//	
//	DMA2_Stream3->CR &= ~DMA_SxCR_EN; // DMA 
//	DMA2_Stream3->NDTR = TFT_HEIGHT*TFT_WIDTH;  
//	DMA2_Stream3->PAR   = 0x4001300C;//SPI
//	DMA2_Stream3->M0AR  = (uint32_t)&LCD_Buffer[0];		
//	DMA2_Stream3->CR |=  DMA_SxCR_CIRC;      //Кольцевой режим
//	DMA2_Stream3->CR |=  DMA_SxCR_EN;        //Включаем DMA
//	SPI1->CR2 |= SPI_CR2_TXDMAEN;            //SPI на DMA 
//	SPI1->CR1 |= SPI_CR1_SPE;                //Включаем SPI для работы в DMA
//}

//void ST7789_Update_DMA_Cicle_Off(void)
//{
//	DMA2_Stream3->CR &= ~DMA_SxCR_EN;     //Отключаем DMA
//  while( !(SPI1->SR & SPI_SR_TXE));	    //Ждем окончания передачи по SPI
//	DMA2_Stream3->NDTR = 0;               //Сброс счетчика DMA
//	DMA2_Stream3->CR &= ~DMA_SxCR_CIRC;   //Выкл Кольцевой режим
//	SPI1->CR1 &= ~SPI_CR1_SPE;            //Спокойно отключаем SPI
//	SPI1->CR2 &= ~SPI_CR2_TXDMAEN;        //Отвязываем от DMA
//	SPI1->CR1 &= ~SPI_CR1_DFF;            //8bit mode
//	SPI1->CR1 |= SPI_CR1_SPE;             //Включаем для работы в обычном режиме
//}
	
