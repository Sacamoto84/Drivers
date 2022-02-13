#include "uTFT_define.h"
#include "fatfs.h"

#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_Conf.h"

#include <stdio.h>

extern char str1[60];

extern FIL SDFile;       /* File object for SD */

extern uint8_t buf[5100]; //Буффер данных

uint16_t * pix;
uint32_t * bytesread1;
uint32_t time;

//void load_video(FIL *fp)
//{
//	 uint32_t index_max  = TFT_WIDTH*TFT_HEIGHT; 
//	 uint32_t index_max2 = TFT_WIDTH*TFT_HEIGHT*2; 
//   uint32_t index;
//	 int32_t x, y;
//	
//	 uint32_t fsize = f_size(fp);
//		
//	
//	
////	ILI9225_setWindow(0, 0, 175, 219);
////	A0_H;
////	CS_L;
////	DMA2_Stream3->CR &= ~DMA_SxCR_EN; //Остановка DMA
////	DMA2_Stream3->NDTR = 176*220;  
////	DMA2_Stream3->PAR   = 0x4001300C;//SPI
////	DMA2_Stream3->M0AR  = (uint32_t)&ili9225_buffer[0];		
////	DMA2_Stream3->CR |=  DMA_SxCR_CIRC; //Повторять
////	DMA2_Stream3->CR |=  DMA_SxCR_EN; //Запуск DMA
////	SPI1->CR2 |=SPI_CR2_TXDMAEN;//Запуск от DMA
//	
//	ST7789_Update_DMA_Cicle_On();
//	
//	TIM7->CR1 &= ~TIM_CR1_CEN;
//	TIM7->ARR = 0xffFF;
//	TIM7->CNT = 0;
//	TIM7->CR1 |= TIM_CR1_CEN;
//	
//	time = 0;
//	
//	uint32_t res;
//	
//	for(index = 0; index < fsize; index+=2)
//	{
//		if (index % 4096 == 0) 
//		{
//			//TIM7->CNT = 0;
//			res = f_read (fp, &buf, 4096, bytesread1);
//			//time = TIM7->CNT;
//		}
//		x = (index>>1) % TFT_WIDTH;
//		y = (((index>>1)%(index_max))/ TFT_WIDTH) * TFT_WIDTH;	
//				
//		LCD_Buffer[x + y]= *(uint16_t *)&buf[index % 4096];
//		
//		if ((index % index_max2) == 0) 
//  		{ 
//			//Кадр отрисован
//			time = TIM7->CNT;
//			//sprintf (str1,"f_read=%d uS %.2f KB/s", time, 1000.0/time*4096.0);
//			sprintf (str1,"f_read=%d uS %.2f KB/s", time, 1000.0/time*115200.0);	
//			SEGGER_SYSVIEW_Print(str1);
//			TIM7->CNT = 0;
//				//HAL_Delay(100);
//	    //uTFT_Update();
// 		}
//	}
//	
//	ST7789_Update_DMA_Cicle_Off();
//	
////	DMA2_Stream3->CR &= ~DMA_SxCR_EN; 
////	DMA2_Stream3->NDTR = 0; 
////	SPI1->CR2  &= ~SPI_CR2_TXDMAEN;
//	
////	CS_H;
//}

void load_video(uTFT_LCD_t* LCD , FIL *fp)
{
	 uint32_t index_max  =  LCD->TFT_HEIGHT *  LCD->TFT_WIDTH; 
	 uint32_t index_max2 = LCD->TFT_HEIGHT *  LCD->TFT_WIDTH * 2; 
   uint32_t index;
	 int32_t x, y;
	
	 uint32_t fsize = f_size(fp);
	
	 time = 0;
	
	 uint32_t res;
	
	for(index = 0; index < fsize; index+=2)
	{
		if (index % 4096 == 0) 
		{
			res = f_read (fp, &buf, 4096, bytesread1);
		}
		x = (index>>1) %  LCD->TFT_WIDTH;
		y = (((index>>1)%(index_max))/  LCD->TFT_WIDTH) *  LCD->TFT_WIDTH;	
				
		 LCD->buffer16[x + y]= *(uint16_t *)&buf[index % 4096];
		
		if ((index % index_max2) == 0) 
  		{ 
	     uTFT_Update(LCD);
 		  }
	}
}


void uTFT_VIDEO(uTFT_LCD_t* LCD, char * Name)
{
	//SEGGER_SYSVIEW_Print("uTFT_Video Start");
	int res;
	res = f_open(&SDFile, Name, FA_READ);
  if (res == FR_OK)
	{
		load_video(LCD, &SDFile);
    f_close(&SDFile);
		//SEGGER_SYSVIEW_Print("uTFT_Video f_close");
	}
	//else
		//SEGGER_SYSVIEW_Print("uTFT_Video == FR_ERROR");

}
