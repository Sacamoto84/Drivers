#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdlib.h>
#include <stdio.h> /* for printf */
#include <string.h> /* for printf */
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "usart.h"
#include "SEGGER_RTT.h"
#include "SEGGER_SYSVIEW.h"

//#include "BLE.h"

#include "tim.h"

#include "SEGGER_SYSVIEW.h" 

#include "usart.h"

#include "BLE_Commands.h"

extern DMA_HandleTypeDef hdma_usart3_tx;

char str2[32];

void StartTaskLCD(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Кольцевой буффер
////////////////////
//https://chipenable.ru/index.php/programming-avr/44-uchebnyy-kurs-organizatsiya-obmena-po-usart-u-s-ispolzovaniem-koltsevogo-bufera.html
#define SIZE_BUF 2048

volatile unsigned char cycleBuf[SIZE_BUF] CCMRAM; //Кольцевой приемный буффер 8K

volatile uint16_t tail = 0;                //Указатель хвоста буффера
volatile uint16_t head = 0;                //Указатель начала буффера
volatile uint16_t count = 0;               //Счетчик символов

volatile uint16_t countCommand = 0; 

//Очистка буффера
void FlushBuf(void){  tail  = 0;  head  = 0;  count = 0;}

//Положить символ в буффер
void PutChar(unsigned char sym)
{
  if (count < SIZE_BUF){                //если в буфере еще есть место
      cycleBuf[tail] = sym;             //помещаем в него символ
      count++;                          //инкрементируем счетчик символов
      tail++;                           //и индекс хвоста буфера
      if (tail == SIZE_BUF) tail = 0;
    }
}

//Взять символ из буффера
unsigned char GetChar(void)
{
   unsigned char sym = 0;
   if (count > 0){                            //если буфер не пустой
      sym = cycleBuf[head];              //считываем символ из буфера
      count--;                                   //уменьшаем счетчик символов
      head++;                                  //инкрементируем индекс головы буфера
      if (head == SIZE_BUF) head = 0;
   }
   return sym;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BLE_UART_Decode_Level_1(int PosS, int PosCRC);

//uint32_t BLE_UART_Timer;              //50 ms inc timer
//uint32_t BLE_UART_Timer_Last;         //50 ms inc timer

void BLE_UART_Decode(void);

volatile float ii19 = 0.1F;
char buffer20[30];
char buffer30[30];

volatile uint8_t i19 = 0;
volatile uint32_t crc = 0xaa;

void USART3_IRQHandler(void)
{
  SEGGER_SYSVIEW_RecordEnterISR();
  static uint8_t USART_data;

	
  if (USART3->SR & USART_SR_ORE){}
	
  //При приеме помещаем в буффер данные
  if ((USART3->SR & USART_SR_RXNE) && ((USART3->SR & USART_SR_ORE) == 0)){
    
		USART_data = USART3->DR;
        PutChar(USART_data);//принятый байт
		//SEGGER_RTT_printf(0,"RX:PutChar\n");
		
		if(USART_data == 0x24) //$
		{  
			 SEGGER_SYSVIEW_Print("RX:End Packet");
			 //SEGGER_RTT_printf(0,"RX:End Packet$\n");
			 //SEGGER_RTT_WriteString(0, "U3 RX End Packet '$'\n");
			 countCommand++;
			
		}
  }	
	
	HAL_UART_IRQHandler(&huart3);
	SEGGER_SYSVIEW_RecordExitISR();
}

//Возвращает количество байт в буффере
uint16_t BTserial_available(void){	return count; }

//Прочитать один байт с буффера
uint8_t BTserial_read(void){ return GetChar(); }

 char temp;

 char big_buffer[SIZE_big_buffer] CCMRAM __attribute__((aligned (4))); //

 uint8_t local_crc;  //
 uint8_t str_crc[3]; //

void BLE_UART_Decode(void)
{		
	//SEGGER_RTT_printf(0, "Decode\n");

	SEGGER_SYSVIEW_Print("BLE_UART_Decode");

    volatile int i;
    volatile int Max_Big_Buf;
	volatile int PosS, PosE, PosCRC; 
	volatile uint8_t temp;  
	
	local_crc  = 0;    
	str_crc[0] = 0x30; 
	str_crc[1] = 0x30; 
	str_crc[2] = 0x30; 
	
	PosS = -1;         //Стартовая позиция Начала пакета !
	PosE = -1;         //Стартовая позиция Конца пакета  $
	PosCRC = -1;       //Стартовая позиция Начала CRC    ;
	
	//Заполняем большой 8K буффер из кольцевого до символа $ вклюительно
	i = 0;
	do
    {
		temp = GetChar();
	  big_buffer[i++] = temp; 
	}
	while (temp != '$');
	
	PosE = i - 1; //Позиция конца пакета
	
	//Level 0
	Max_Big_Buf = i; //Максимальный индекс в буффере
	big_buffer[Max_Big_Buf] = 0;
	if (PosE < 128)
	{	
		SEGGER_RTT_printf(0, "\n%s%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_MAGENTA ,big_buffer,RTT_CTRL_RESET);	
	}
    //Ищем индекс начала пакета
	for (i = 0; i < Max_Big_Buf; i++)
	{
	  if (big_buffer[i] == '!') 
		{PosS = i; break;}
	}
	
	//Если нет начала пакета
	if ( PosS == -1 )
	{
    SEGGER_RTT_WriteString(0, "L0 > Error > PosS == -1\n");
    return;
	}		
	//Есть начало и конец

	//Ищем символ начала CRC
	for(i = PosS; i < PosE; i++)
	{	  if (big_buffer[i] == ';') PosCRC = i; }
	
	//Если нет начала CRC
	if (PosCRC == -1)
	{
		SEGGER_RTT_WriteString(0, "L0 > Error > PosCRC == -1\n");
    return;
	}
	
    //Нашли начало CRC
	if ((PosE - PosCRC) > 4)
	{
		SEGGER_RTT_WriteString(0, "L0 > Error > (PosE - PosCRC) > 4\n");
    return;
	}
		
	//Читаем сиволы CRC
	temp = PosE - PosCRC;
	switch (temp)
    {
    	case 2: str_crc[2] = big_buffer[PosCRC+1]; break;
    	case 3: str_crc[1] = big_buffer[PosCRC+1]; str_crc[2] = big_buffer[PosCRC+2]; break;
	    case 4: str_crc[0] = big_buffer[PosCRC+1]; str_crc[1] = big_buffer[PosCRC+2]; str_crc[2] = big_buffer[PosCRC+3]; break;
    }		
		
		//Вычеляем значение CRC
		local_crc = (str_crc[0] - 0x30)*100 + (str_crc[1] - 0x30)*10 + (str_crc[2] - 0x30);
		
		//Расчитываем CRC
		temp = CRC8(&big_buffer[PosS+1], PosCRC - PosS - 1);
		
		if  (local_crc != temp)
		{
		  SEGGER_RTT_printf(0, "L0 > Error > Error calculate CRC In:%d != Calc:%d\n", local_crc , temp);
			return;
		}
		
		//Level 0 ок
		//Передаем управление Level1
		//Передаем позицию начала и CRC
		BLE_UART_Decode_Level_1(PosS, PosCRC);	 
}

char buffer1[30]; //Значение
char mini_buffer[256];

void BLE_UART_Decode_Level_1(int PosS, int PosCRC)
{
	int i;
	volatile int PosSharp; //
	char code_index;
	
	//Работаем с big_buffer 
  //Разбераем уже сам пакет

	PosSharp = -1;
	
	//Ищем символ #
	for(i = PosS+1; i < PosS+4; i++)
	{	  if (big_buffer[i] == '#') PosSharp = i; }
	
	//Если нет начала CRC
	if (PosSharp == -1) 
	{
		SEGGER_RTT_WriteString(0, "L1 > Error > PosSharp == -1\n");
    return;
	}

	//SEGGER_RTT_printf(0, "L1 > Info > PosSharp == %d\n", PosSharp);
	
	//PosSharp найден но он должен находится на PosS + 3
	if (PosSharp != PosS + 3)
	{
		SEGGER_RTT_WriteString(0, "L1 > Error > PosSharp != PosS + 3\n");
	  return;
	}
	
	//Выделяем индекс
	code_index = (big_buffer[PosS+1] - 0x30)*10 + (big_buffer[PosS+2] - 0x30);
    SEGGER_RTT_printf(0, "%s%sL1>PORT>%d%s\n", RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_BRIGHT_BLUE,code_index,RTT_CTRL_RESET);
	
	switch (code_index)
  {
		case 98: BLE_Port_98(PosS + 4, PosCRC - PosS - 4);	break; //CLI
  	    case 99: BLE_Port_99(PosS + 4, PosCRC - PosS - 4);	break;

    default: SEGGER_RTT_printf(0, "L1 >Error> Unsupported PORT : %d\n", code_index);
  		break;
  }
	

}	

void BLE_Task(void)
{
  while (countCommand)
  {
    countCommand--;		
    BLE_UART_Decode();
  }
}


/*
  Name  : CRC-8
  Poly  : 0x31    x^8 + x^5 + x^4 + 1
  Init  : 0xFF
  Revert: false
  XorOut: 0x00
  Check : 0xF7 ("123456789")
  MaxLen: 15 байт(127 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
uint8_t CRC8(char *pcBlock, unsigned int len)
{
    unsigned char _crc = 0xFF;
    unsigned int i;

    while (len--)
    {
        _crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
            _crc = _crc & 0x80 ? (_crc << 1) ^ 0x31 : _crc << 1;
    }
    return _crc;
}
