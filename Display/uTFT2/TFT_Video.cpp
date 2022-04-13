#include "fatfs.h"
#include <stdio.h>
#include "TFT.h"

#include "SEGGER.h"
#include "SEGGER_RTT.h"

#include "HiSpeedDWT.h"

extern HiSpeedDWT TimerDWT;
extern HiSpeedDWT TimerT5;

extern char str1[60];

extern FIL SDFile;       /* File object for SD */

uint16_t * pix;

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
    uint32_t index;
	uint32_t fsize = f_size(&SDFile);
	ST7789_Update_DMA_Cicle_On();
	UINT bytesread;
	uint32_t start_time = uwTick;
	int32_t delta;
	video_stop = 0;
    //11ms delay=0;
	for(index = 0; index < fsize; index+=115200)
	{
		f_read (&SDFile, (uint8_t*)(LCD->buffer16), 115200, &bytesread);
		if (videoCallBackFunc) {
	     	uint32_t (*fcnPtr)(uint32_t) = videoCallBackFunc;
			if(fcnPtr(0)) break;
		}

		delta =  delay - (uwTick - start_time) - 1; //Время кадра

		if ((delay) && (delta > 0))
		{
			//HAL_Delay(delta);
			  uint32_t tickstart = HAL_GetTick();
			  uint32_t wait = delta;

			  /* Add a freq to guarantee minimum wait */
			  if (wait < HAL_MAX_DELAY)
			  {
			    wait += (uint32_t)(uwTickFreq);
			  }

			  while((HAL_GetTick() - tickstart) < wait)
			  {
				if (videoCallBackFunc)
				{
				    uint32_t (*fcnPtr)(uint32_t) = videoCallBackFunc;
					if(fcnPtr(0))
					{
						ST7789_Update_DMA_Cicle_Off();
						return;
					}
				}
			  }
		}
		start_time = uwTick;
	}


	ST7789_Update_DMA_Cicle_Off();
}


void TFT::video_play(char * Name, uint8_t delay)
{
	SEGGER_RTT_WriteString(0, "video play\n");
	int res;
	res = f_open(&SDFile, Name, FA_READ);

    if (res == FR_OK)
	{
    	//TimerDWT.Start();
    	video_load(delay);
        f_close(&SDFile);
        //TimerDWT.Loger((char*)"video_load");
	}
	else
	  SEGGER_RTT_WriteString(0, "Video == FR_ERROR\n");
}
