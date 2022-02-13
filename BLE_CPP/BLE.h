#ifndef BLE_H
#define BLE_H

#include <stdlib.h>
#include <stdio.h> /* for printf */
#include <string.h> /* for printf */

#include "main.h"

#include "SEGGER.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
#include "SEGGER_SYSVIEW.h"

#include "usart.h"

#define SIZE_BUF 8192

class BLE {
public:
	BLE();

	~BLE();

	void init(UART_HandleTypeDef * rx, UART_HandleTypeDef * tx);
	void USART_RX_IRQHandler(void); //Ставим в IRQ RX

	UART_HandleTypeDef * _huart_rx;
	UART_HandleTypeDef * _huart_tx;

	uint16_t available(void); //Возвращает количество байт в буффере
	uint8_t  read(void);      //Прочитать один байт с буффера
	void     Task(void);
//-------------------------------------- protected ----------------------------------//
protected:

 void BLE_UART_Decode_Level_1(int PosS, int PosCRC);
 void BLE_UART_Decode(void);
 void FlushBuf(void);              //Очистка буффера
 void PutChar(unsigned char sym);  //Положить символ в буффер
 unsigned char GetChar(void);      //Взять символ из буффера
 
 uint8_t CRC8(char *pcBlock, unsigned int len);

 uint16_t tail = 0;                //Указатель хвоста буффера
 uint16_t head = 0;                //Указатель начала буффера
 uint16_t count = 0;               //Счетчик символов

 uint16_t countCommand = 0; 

 //Кольцевой входной буффер
 unsigned char cycleBuf[SIZE_BUF];// __attribute__((section(".my_ccmram"))); //Кольцевой приемный буффер 8K
 char big_buffer[1024*2];
 uint8_t local_crc;  //
 uint8_t str_crc[3]; //
 char temp;
 char buffer1[30]; //Значение
 char mini_buffer[256];

};


#endif
