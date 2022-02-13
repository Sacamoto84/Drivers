#include "../TFT.h"

void TFT::ST7789_Init(void) {
	if (LCD->GPIO_CS != NULL) {
		CS_0;
	}

	if (LCD->GPIO_RESET != NULL) {
		SPI.LCD_Reset();
	}

	SPI.SendCmd(0x01);   //SWRESET
	HAL_Delay(150);

	SPI.SendCmd(0x11);   //SLPOUT
	HAL_Delay(150);

	SPI.SendCmd(0x3A);   //COLMOD RGB444(12bit) 0x03, RGB565(16bit) 0x05,
	SPI.SendData(0x05);  //RGB666(18bit) 0x06
	SPI.SendCmd(0x36);   //MADCTL
	SPI.SendData(0x08);  //0x08 B-G-R, 0x14 R-G-B  14

	SPI.SendCmd(ST77XX_GAMSET);               // ST77XX_GAMSET         0x26
	SPI.SendData(0x02); // Gamma curve 2 (G1.8)  ST77XX_DGMEN          0xBA
	SPI.SendCmd(ST77XX_DGMEN);
	SPI.SendData(0x04); // Enable gamma

	SPI.SendCmd(0xe0);   // Positive Voltage Gamma Control
	SPI.SendData(0xd0);
	SPI.SendData(0x00);
	SPI.SendData(0x03);
	SPI.SendData(0x08);
	SPI.SendData(0x0a);
	SPI.SendData(0x17);
	SPI.SendData(0x2e);
	SPI.SendData(0x44);
	SPI.SendData(0x3f);
	SPI.SendData(0x29);
	SPI.SendData(0x10);
	SPI.SendData(0x0e);
	SPI.SendData(0x14);
	SPI.SendData(0x18);

	SPI.SendCmd(0xe1);   // Negative Voltage Gamma Control
	//See datasheet for more information
	SPI.SendData(0xd0);
	SPI.SendData(0x00);
	SPI.SendData(0x03);
	SPI.SendData(0x08);
	SPI.SendData(0x0a);
	SPI.SendData(0x17);
	SPI.SendData(0x2e);
	SPI.SendData(0x44);
	SPI.SendData(0x3f);
	SPI.SendData(0x29);
	SPI.SendData(0x10);
	SPI.SendData(0x0e);
	SPI.SendData(0x14);
	SPI.SendData(0x18);

	SPI.SendCmd(0x11); // Exit Sleep Mode
	//HAL_Delay(120);
	HAL_Delay(350);

	SPI.SendCmd(0x29); // Display on
	HAL_Delay(350); //HAL_Delay(120);

	SPI.SendCmd(0x21);   //INVON
	SPI.SendCmd(0x13);   //NORON
	SPI.SendCmd(0x29);   //DISPON

	if (LCD->GPIO_CS != NULL) {
		CS_1;
	}
}

void TFT::ST7789_Update(void) {
	if (LCD->GPIO_CS != NULL) {
		CS_0;
	}

	int32_t i = 0;

	uint8_t HI;
	uint8_t LO;

	HI = (LCD->dx + LCD->TFT_WIDTH - 1) >> 8;
	LO = (LCD->dx + LCD->TFT_WIDTH - 1) & 0xFF;

	SPI.SendCmd(0x2A);     // Column addr set
	SPI.SendData(0x00);     //??????? ??? XSTART
	SPI.SendData(LCD->dx);     //??????? ??? XSTART
	SPI.SendData(HI);     //???????  ??? XEND
	SPI.SendData(LO);    //??????? ??? XEND

	SPI.SendCmd(0x2B); // Row addr set
	SPI.SendData(0x00);
	SPI.SendData(LCD->dy); //TFT_YSTART
	SPI.SendData(0x00);
	SPI.SendData(LCD->dy + LCD->TFT_HEIGHT - 1); //TFT_YSTART

	SPI.SendCmd(0x2C); //Memory write

	//LCD->hspi->Instance->CR1 |= SPI_CR1_DFF;
	SPI.Spi8to16();

	DATA;

	/////////////////Spi8to16(LCD); //16bit mode

	if (LCD->Bit == 4) {
		for (uint16_t i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH) - 1; i++) {
			if (i % 2) {
				while ((LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0)
					;
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i / 2])
						& 0x0F]; //4 bit
			} else {
				while ((LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0)
					;
				LCD->hspi->Instance->DR =
						LCD->palete[(LCD->buffer8[i / 2]) >> 4]; //4 bit
			}
		}
	}

	if (LCD->Bit == 16) {
		for (i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH); i++) {
			// HAL_SPI_Transmit_DMA(&hspi1,(uint8_t *)LCD_Buffer, 32400*2);
			while (!(LCD->hspi->Instance->SR & SPI_SR_TXE))
				;
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
	while (!(LCD->hspi->Instance->SR & SPI_SR_TXE))
		;

	SPI.Spi16to8(); //8bit mode

	if (LCD->GPIO_CS != NULL) {
		CS_1;
	}

}

void TFT::ST7789_AddrSet(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	SPI.SendCmd(0x2A);
	SPI.SendData(0x00);
	SPI.SendData(x0 + LCD->dx);
	SPI.SendData(0x00);
	SPI.SendData(x1 + LCD->dx);
	SPI.SendCmd(0x2B);
	SPI.SendData(0x00);
	SPI.SendData(y0 + LCD->dy);
	SPI.SendData(0x00);
	SPI.SendData(y1 + LCD->dy);
	SPI.SendCmd(0x2C);
}

// Поворот экрана
void TFT::ST77XX_Rotate(uint16_t depth) {
	if (depth == 0) {
		LCD->MADCTL.MV = 0;
		LCD->MADCTL.MX = 0;
		LCD->MADCTL.MY = 0;
		LCD->dx = LCD->ROTATE_DELTA.DX_0;
		LCD->dy = LCD->ROTATE_DELTA.DY_0;
		LCD->TFT_WIDTH = LCD->ROTATE_DELTA.W_0;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_0;
	}
	if (depth == 90) {
		LCD->MADCTL.MV = 1;
		LCD->MADCTL.MX = 1;
		LCD->MADCTL.MY = 0;
		LCD->dx = LCD->ROTATE_DELTA.DX_90;
		LCD->dy = LCD->ROTATE_DELTA.DY_90;
		LCD->TFT_WIDTH = LCD->ROTATE_DELTA.W_90;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_90;
	}
	if (depth == 180) {
		LCD->MADCTL.MV = 0;
		LCD->MADCTL.MX = 1;
		LCD->MADCTL.MY = 1;
		LCD->dx = LCD->ROTATE_DELTA.DX_180;
		LCD->dy = LCD->ROTATE_DELTA.DY_180;
		LCD->TFT_WIDTH = LCD->ROTATE_DELTA.W_180;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_180;
	}
	if (depth == 270) {
		LCD->MADCTL.MV = 1;
		LCD->MADCTL.MX = 0;
		LCD->MADCTL.MY = 1;
		LCD->dx = LCD->ROTATE_DELTA.DX_270;
		LCD->dy = LCD->ROTATE_DELTA.DY_270;
		LCD->TFT_WIDTH = LCD->ROTATE_DELTA.W_270;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_270;
	}
	ST77XX_Update_MADCTL();
}

//Обновление регистра MADCTL
void TFT::ST77XX_Update_MADCTL(void) {
	uint8_t data;

	if (LCD->GPIO_CS != NULL)
		CS_0;

	data = 0;
	data = (LCD->MADCTL.MY << 7) | (LCD->MADCTL.MX << 6) | (LCD->MADCTL.MV << 5)
			| (LCD->MADCTL.ML << 4) | (LCD->MADCTL.RGB << 3)
			| (LCD->MADCTL.MH << 2);
	SPI.SendCmd(0x36);
	SPI.SendData(data);

	if (LCD->GPIO_CS != NULL)
		CS_1;
}

void TFT::ST7789_Update_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	if (LCD->GPIO_CS != NULL) {
		CS_0;
	}

	uint8_t x1c = constrain(x1, 0, LCD->TFT_WIDTH - 1);
	uint8_t x2c = constrain(x2, 0, LCD->TFT_WIDTH - 1);

	uint8_t y1c = constrain(y1, 0, LCD->TFT_HEIGHT - 1);
	uint8_t y2c = constrain(y2, 0, LCD->TFT_HEIGHT - 1);

	int32_t x = 0;
	int32_t y, i;
	uint8_t HI;
	uint8_t LO;

	HI = (LCD->dx + LCD->TFT_WIDTH - 1) >> 8;
	LO = (LCD->dx + LCD->TFT_WIDTH - 1) & 0xFF;

	SPI.SendCmd(0x2A);     // Column addr set
	SPI.SendData(0);     //??????? ??? XSTART
	SPI.SendData(x1c + LCD->dx);     //??????? ??? XSTART
	SPI.SendData(0);     //???????  ??? XEND
	SPI.SendData(x2c + LCD->dx);    //??????? ??? XEND

	SPI.SendCmd(0x2B); // Row addr set
	SPI.SendData(0x00);
	SPI.SendData(y1c + LCD->dy); //TFT_YSTART
	SPI.SendData(0x00);
	SPI.SendData(y2c + LCD->dy); //TFT_YSTART

	SPI.SendCmd(0x2C); //Memory write

	//LCD->hspi->Instance->CR1 |= SPI_CR1_DFF;
	SPI.Spi8to16();

	DATA;

	uint32_t count = (x2c - x1c) * (y2c - y1c);

	/////////////////Spi8to16(LCD); //16bit mode

	if (LCD->Bit == 4) {
		for (i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH) - 1; i++) {
			if (i % 2) {
				while ((LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0)
					;
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i / 2])
						& 0x0F]; //4 bit
			} else {
				while ((LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0)
					;
				LCD->hspi->Instance->DR =
						LCD->palete[(LCD->buffer8[i / 2]) >> 4]; //4 bit
			}
		}
	}

	if (LCD->Bit == 16) {
		uint16_t *p;

		for (y = y1c; y < y2c; y++) {
			p = &LCD->buffer16[0] + y * LCD->TFT_WIDTH + x1c;
			for (x = 0; x < (x2c - x1c) +1 ; x++) {

				while (!(LCD->hspi->Instance->SR & SPI_SR_TXE))
					;
				LCD->hspi->Instance->DR = *p++;

			}
		}

	}

////	uTFT_DMA_completed = 0;
////
////	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)LCD->buffer16[0], LCD->TFT_HEIGHT*LCD->TFT_WIDTH);
////
////	while (uTFT_DMA_completed == 0)
////	{
////	}
	while (!(LCD->hspi->Instance->SR & SPI_SR_TXE))
		;

	SPI.Spi16to8(); //8bit mode

	if (LCD->GPIO_CS != NULL) {
		CS_1;
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

