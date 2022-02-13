#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "adc.h"
#include "tim.h"

#include "global_define.h"
#include <math.h>


#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_Int.h"
#include "SEGGER_RTT.h"

extern DMA_HandleTypeDef hdma_adc1;
//extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;


uint8_t ADC1_Data[ADCSAMPLES];
uint16_t ADC3_Data;

int8_t ADC1_Data_Render[500];
int8_t ADC2_Data_Render[500];

uint16_t ADC_Find_Trigger(uint16_t * data);

int8_t ADC1_Filter[500];
int8_t ADC2_Filter[500];


uint16_t trigger;


//Поиск максимального значения
int8_t find_max(int8_t * data)
{
	int8_t *max = data;
  
	if (*data++ > *max) max = data;
	if (*data++ > *max) max = data;
	if (*data++ > *max) max = data;
	
	return *max;
}


//ADC1
void DMA2_Stream0_IRQHandler(void)
{
	#if defined(SEGGER)
	  SEGGER_SYSVIEW_RecordEnterISR();
	#endif

  HAL_DMA_IRQHandler(&hdma_adc1);
	
  TIM8->CR1 &= ~TIM_CR1_CEN; //Остановка таймера
	TIM8->CNT = 0;             //Сброс счетчика
	
	uint16_t offset;
	
	if (DMA2_Stream0->CR & DMA_SxCR_CT) //Использоемого буффера
	{
		offset = 0;
	}
	else
	{
		offset = ADCSAMPLES/2;
	}
	
	//trigger =	ADC_Find_Trigger(&ADC1_Data[0]);
	trigger = 0;
	
	for(uint16_t i=0; i<ADCSAMPLES/2;i+=2)
	{
	  ADC1_Data_Render[i/2] = ADC1_Data[i+offset]-127;
	  ADC2_Data_Render[i/2] = ADC1_Data[i+1+offset]-127;
	}
	
	//int8_t * p;
	//p = &ADC2_Data_Render[0];
	
	for(uint16_t i=0; i<200;i++)
	  {
       ADC2_Filter[i] = abs(ADC2_Data_Render[i]);//find_max(&ADC2_Data_Render[i]);
		}
		
	
	#if defined(SEGGER)
	  SEGGER_SYSVIEW_RecordExitISR();
	#endif
}

uint16_t ADC_Find_Trigger(uint16_t * data)
{
  for(uint16_t i=0;i<176*2;i++)
	{
		if ((data[i]<2047) && ( data[i+1]>2047)) return  i;	
	}
  return 0;
}


//Функция остановки ADC
void stop_adc(void)
{
	TIM8->CR1 &= ~TIM_CR1_CEN;
}

//Функция запуск ADC
void start_adc(void)
{
	TIM8->CR1 |= TIM_CR1_CEN;
}

//Функция запуск ADC
void invert_adc(void)
{
	if(TIM8->CR1 & TIM_CR1_CEN) TIM8->CR1 &= ~TIM_CR1_CEN;
	else TIM8->CR1 |= TIM_CR1_CEN;
}












