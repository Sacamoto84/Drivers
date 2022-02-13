#include <stdlib.h>
#include <stdio.h> /* for printf */

#if defined(STM32F103xB)  
  #include "stm32f1xx_hal.h" 
	#include "stm32f1xx.h"
  #include "stm32f1xx_it.h"
#endif	

#if defined(STM32F373xC)
  #include "stm32f3xx_hal.h" 
	#include "stm32f3xx.h"
  #include "stm32f3xx_it.h"
#endif	


#include "usart.h"
#include "dma.h"
#include "usart.h"

#include "main.h"

#include "VirtuinoBluetooth.h"

#include "cmsis_os.h"

#include "SEGGER_RTT.h"

extern osSemaphoreId myBinarySem_RX_CounterHandle;


char BTserial_read(void);
char BTserial_available(void);
char BTserial_readStringUntil(char ch, char * str, int max_item, int timeout);

static unsigned char cycleBuf[256];
static unsigned char tail = 0;   //Указатель хвоста буффера
static unsigned char head = 0;   //Указатель начала буффера
volatile uint8_t count;               //Счетчик символов

extern DMA_HandleTypeDef hdma_usart1_rx;

#define DMA_RX_MAX 64

static unsigned char DMA_RX_Buf[DMA_RX_MAX];
static unsigned char DMA_TX_Buf[32];

//Очистка буффера
static void FlushBuf(void)
{
  tail = 0;
  head = 0;
  count = 0;
}

//Положить символ в буффер
void PutChar(unsigned char sym)
{
  //if (count < 256){                   //Если в буффере есть место
      cycleBuf[tail] = sym;             //помещаем в него символ
      count++;                          //инкремент счетчика символов
      tail++;                           //и индекс хвоста буффера
      //if (txBufTail == SIZE_BUF) txBufTail = 0;
  //  }
}

//Взять символ из буффера
unsigned char GetChar(void)
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
	
	#if defined(STM32F373xC) 
		if (USART1->ISR & USART_ISR_ORE)
		{
			USART1->ICR |= USART_ICR_ORECF;
		}
	#endif
	
	#if defined(STM32F103xB) 
		if (USART1->SR & USART_SR_ORE)
		{
		
		}
	#endif
	
  //При приеме помещаем в буффер данные
	#if defined(STM32F373xC)
  if (USART1->ISR & USART_ISR_RXNE) {
		USART_data = USART1->RDR;
	#endif	

	#if defined(STM32F103xB) 
  if (USART1->SR & USART_SR_RXNE) {
		USART_data = USART1->DR;
	#endif

  PutChar(USART_data);//принятый байт

		
		//array[0] = USART_data;
		//SEGGER_SYSVIEW_Print(array);
		//if(USART_data == 'C') FlushBuf();
		//if(USART_data == 0x24) //$
		//	UART_Decode();//Если получили конец отсылаем сигнал потоку декодирования	
  }	
	//HAL_UART_IRQHandler(&huart1);
	//SEGGER_SYSVIEW_RecordExitISR();
}

//Возвращает количество байт в буффере
char BTserial_available(void)
{
	return count;
}

//Прочитать один байт с буффера
char BTserial_read(void)
{
	return GetChar();
}

//Читаем пока не встретим символ
char BTserial_readStringUntil(char ch, char * str, int max_item, int timeout)
{
	char data;
	char *p;
	p = str; //Начальный указатель
	
	char * p_stop; 
	p_stop	= p + max_item;
	
  do	
	 	if (count) //Есть данные
		{
			data = GetChar();
      
			if (ch == data) 
			{
        SEGGER_RTT_printf(0, RTT_CTRL_RESET">"RTT_CTRL_TEXT_BRIGHT_YELLOW"readStringUntil return 0; //OK\r\n");
				return 0; //Ok
			}				
			*p++ = data;
			
			if (p >= p_stop) 
			{
				
				SEGGER_RTT_printf(0, RTT_CTRL_RESET">"RTT_CTRL_TEXT_BRIGHT_YELLOW"readStringUntil return 2; //Выход т.к. не пришел данный символ\r\n");
				return 2; //Выход т.к. не пришел данный символ
			}
			
    }
		else
		{
			  //Если нет данных ждем семафор
				if( xSemaphoreTake( myBinarySem_RX_CounterHandle,  portMAX_DELAY ) == pdTRUE )
         {         
           SEGGER_RTT_printf(0, RTT_CTRL_RESET">"RTT_CTRL_TEXT_BRIGHT_YELLOW"readStringUntil Semafore Data read\r\n");
				 } 
		}
		
  while (1);
		
	return 1; //Выход по таймауту
  		
		
}	

//Инициализация UART DMA 
void BTserial_init(void)
{
	HAL_UART_Receive_DMA(&huart1,DMA_RX_Buf, DMA_RX_MAX); //Запуск DMA 
	
}








//Прерывание по половине буффера или целого буффера DMA RX 103 373
//UART1 DMA RX
void DMA1_Channel5_IRQHandler(void)
{ 
	#if defined(SEGGER)
	  SEGGER_SYSVIEW_RecordEnterISR();
	#endif  
	
  uint16_t i;
	char str[90];
	
	//Определяем какая часть буффера готова
	if (DMA1->ISR & DMA_ISR_HTIF5) //Первая часть
	{
		for( i = 0; i< DMA_RX_MAX/2 ; i++)
		  PutChar(DMA_RX_Buf[i]);
		
		DMA1->IFCR |= DMA_IFCR_CHTIF5;

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(myBinarySem_RX_CounterHandle, &xHigherPriorityTaskWoken);
		//SEGGER_RTT_printf(0, RTT_CTRL_RESET">"RTT_CTRL_TEXT_BRIGHT_RED"--DMA Give H > count=%d tail=%d head=%d\r\n",count, tail, head);
		//SEGGER_RTT_printf(0, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n");
		//SEGGER_RTT_Write(0, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n", 82);
		sprintf(str, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n");
		SEGGER_RTT_Write(0, str,82);
		
		
		
		#if defined(SEGGER) 
			SEGGER_SYSVIEW_RecordExitISR(); 
		#endif	
		
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);		
		
		return;
	}
	
	if (DMA1->ISR & DMA_ISR_TCIF5) //Вторая часть
	{
		
		for( i = DMA_RX_MAX/2; i< DMA_RX_MAX ; i++)
		  PutChar(DMA_RX_Buf[i]);
		
		DMA1->IFCR |= DMA_IFCR_CTCIF5;
		

		
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(myBinarySem_RX_CounterHandle, &xHigherPriorityTaskWoken);
	//SEGGER_RTT_SetTerminal(0);	
		//SEGGER_RTT_printf(0, RTT_CTRL_RESET">"RTT_CTRL_TEXT_BRIGHT_RED"--DMA Give T > count= tail= head=\r\n");
	//	SEGGER_RTT_Write(0, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n", 82);
  //SEGGER_RTT_printf(0, RTT_CTRL_RESET">"RTT_CTRL_TEXT_BRIGHT_RED"--DMA Give T > count=%d tail=%d head=%d\r\n",count, tail, head);
		//SEGGER_RTT_printf(0, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n");
		sprintf(str, "01234567890123456789012345678901234567890123456789012345678901234567890123456789\r\n");
		SEGGER_RTT_Write(0, str,82);

		
	#if defined(SEGGER) 
	  SEGGER_SYSVIEW_RecordExitISR(); 
	#endif
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);		
		
	return;
	}
	
  DMA1->IFCR |= DMA_IFCR_CGIF5; 
	
	#if defined(SEGGER) 
	  SEGGER_SYSVIEW_RecordExitISR(); 
	#endif
	
  //HAL_DMA_IRQHandler(&hdma_usart1_rx);
}