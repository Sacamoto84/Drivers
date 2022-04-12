#include "fatfs.h"
#include <stdio.h>
#include "TFT.h"

#include "SEGGER.h"
#include "SEGGER_RTT.h"

extern char str1[60];

extern FIL SDFile;       /* File object for SD */

uint16_t * pix;
UINT * bytesread1;
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

void TFT::video_load(uint8_t delay)
{
	 uint8_t buf[5100]; //Буффер данных

	 uint32_t index_max  =  LCD->TFT_HEIGHT *  LCD->TFT_WIDTH; 
	 uint32_t index_max2 =  LCD->TFT_HEIGHT *  LCD->TFT_WIDTH * 2;
     uint32_t index;
	 int32_t x, y;
	
	 uint32_t fsize = f_size(&SDFile);
	 time = 0;
	 uint32_t res;
	
	for(index = 0; index < fsize; index+=2)
	{
		if (index % 4096 == 0) 
		{
			res = f_read (&SDFile, &buf, 4096, bytesread1);
		}
		x = (index>>1) %  LCD->TFT_WIDTH;
		y = (((index>>1)%(index_max))/  LCD->TFT_WIDTH) *  LCD->TFT_WIDTH;	
				
		LCD->buffer16[x + y]= *(uint16_t *)&buf[index % 4096];
		
		if ((index % index_max2) == 0) 
  		{ 
			ST7789_UpdateDMA16bitV3();


			 if (videoCallBackFunc) {

				 uint32_t (*fcnPtr)(uint32_t) = videoCallBackFunc;
				 if(fcnPtr(0)) break;
			}

				if (delay)
					HAL_Delay(delay);
 		}
	}
}


void TFT::video_play(char * Name, uint8_t delay)
{
	SEGGER_RTT_WriteString(0, "Video Play\n");
	int res;
	res = f_open(&SDFile, Name, FA_READ);

    if (res == FR_OK)
	{
    	video_load(delay);
        f_close(&SDFile);
	}
	else
	  SEGGER_RTT_WriteString(0, "Video == FR_ERROR\n");
}
