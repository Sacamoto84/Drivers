#include "stm32f3xx_hal.h"
#include <stdlib.h>
#include <stdio.h> /* for printf */

#include "stm32f3xx.h"
#include "stm32f3xx_it.h"

/* USER CODE BEGIN 0 */
#include "usart.h"

static unsigned char cycleBuf[256];
static unsigned char tail = 0;   //Указатель хвоста буффера
static unsigned char head = 0;   //Указатель начала буффера
static unsigned char count = 0;  //Счетчик символов

//Очистка буффера
static void FlushBuf(void)
{
  tail = 0;
  head = 0;
  count = 0;
}

//Положить символ в буффер
static void PutChar(unsigned char sym)
{
  //if (count < 256){                   //Если в буффере есть место
      cycleBuf[tail] = sym;             //помещаем в него символ
      count++;                          //инкремент счетчика символов
      tail++;                           //и индекс хвоста буффера
      //if (txBufTail == SIZE_BUF) txBufTail = 0;
  //  }
}

//Взять символ из буффера
static unsigned char GetChar(void)
{
   unsigned char sym = 0;
   if (count > 0){                       //Если буфер не пустой
      sym = cycleBuf[head];              //считываем символ из буффера
      count--;                           //уменьшаем счетчик символов
      head++;                            //инкремент индекса головы буфера
   }
   return sym;
}



void USART1_IRQHandler(void)
{
	//SEGGER_SYSVIEW_RecordEnterISR();
	uint8_t USART_data;
	
 //При приеме помещаем в буффер данные
  if (USART1->ISR & USART_ISR_RXNE) {

		USART_data = USART1->RDR; 
    PutChar(USART_data);//принятый байт
		
		//array[0] = USART_data;
		//SEGGER_SYSVIEW_Print(array);
		//if(USART_data == 'C') FlushBuf();
		//if(USART_data == 0x24) //$
		//	UART_Decode();//Если получили конец отсылаем сигнал потоку декодирования
  }	
	HAL_UART_IRQHandler(&huart1);
	//SEGGER_SYSVIEW_RecordExitISR();
}

//Возвращает количество байт в буффере
uint8_t BTserial_available(void)
{
	return count;
}

//Прочитать один байт с буффера
uint8_t BTserial_read(void)
{
	return GetChar();
}
