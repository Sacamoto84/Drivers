#include "stm32f1xx_hal.h"
#include "global.h"
#include <stdlib.h>
#include <stdio.h> /* for printf */

#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */
#include "global.h"
#include "usart.h"

static void UART_Decode(void); 

static char  decoder_index_dec;
static float decoder_value_dec;

static char decoder_float_value; //Признак запроса значения

static char temp;
static char buffer[2];  //Индекс
static char buffer1[8]; //Значение

static unsigned char cycleBuf[256];
static unsigned char tail = 0;   //Указатель хвоста буффера
static unsigned char head = 0;   //Указатель начала буффера
static unsigned char count = 0;  //Счетчик символов

static char osSignalSet_Decode; //Сигнал начала декодировки принятого пакета
static char array[16] = "s";

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
  //if (count < 256){                //Если в буффере есть место
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
	SEGGER_SYSVIEW_RecordEnterISR();
	uint8_t USART_data;
	
 //При приеме помещаем в буффер данные
  if (USART1->SR & USART_SR_RXNE) {
		USART_data = USART1->DR; 
    PutChar(USART_data);//принятый байт
		//array[0] = USART_data;
		//SEGGER_SYSVIEW_Print(array);
		if(USART_data == 'C') FlushBuf();
		if(USART_data == 0x24) //$
			UART_Decode();//Если получили конец отсылаем сигнал потоку декодирования
  }	
	HAL_UART_IRQHandler(&huart1);
	SEGGER_SYSVIEW_RecordExitISR();
}

//Поток обновления данных на Blynk и Virtuino
static void UART_Decode (void) {

	  static char i;
	  
	  for(i=0;i<7;i++) buffer1[i] = 0;
	
    //Читаем данный символ из буффера
     if (GetChar()=='!') //Заебись это начало пакета
		 { 
			 temp = GetChar();
			 buffer[0] = GetChar();
			 decoder_index_dec = buffer[0]-0x30;//atoi(buffer[0]); //Получили индекс целого числа 00.31
			 
			 if (GetChar() != '=') goto rel1; //Ошибка нету равно
			 // = есть теперь проверим что нужно ? или  задание значения
			   temp = GetChar();
				 if (temp == '?') {goto rel1;}// хотят получить значение по индексу decoder_index_dec
				 else  {i=0; goto rel2;}
			 
			 for(i=0;i < 8 ;i++)
			 {
				  temp = GetChar();
				  if (temp == '$') break;
				  rel2: buffer1[i] = temp;
			 }
			 buffer1[i+1] = 0; //Конец строки

       decoder_value_dec =atof(buffer1);
			 vmemory[decoder_index_dec] = decoder_value_dec; //Запись в образ
			 		 
		 }
		 else 
	   {	
		   rel1:	 
			 //temp = GetChar();
		   while(GetChar() != '$');  //Начала нет пропердим до концаи будем ждать следующий пакет
		 }
}