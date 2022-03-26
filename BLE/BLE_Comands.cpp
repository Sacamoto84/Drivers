#ifdef USE_CLI

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

#include "BLE_Commands.h"
#include "LOG.h"

extern void SendToCli(char *str, int len);

extern volatile int U3_DMA_TX_Complete;

//00 - Прочесть версию прошивки 
//01 - Установить CH1 EN на значение 0 и >=1 Integer (без подтверждения)
//02 - Установить CH2 EN на значение 0 и >=1 Integer (без подтверждения)

//
extern char big_buffer[] __attribute__((aligned (4))); //
char dma_str[1024];

void BT_Send_RAW(char *str) {

	int max = strlen(str);

	if (max < 128) {
		//SEGGER_RTT_printf(0, "%s%sTX>%s", RTT_CTRL_TEXT_BLACK,
		//RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_RESET);
		//SEGGER_RTT_printf(0, "%s%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_WHITE,
		//RTT_CTRL_BG_GREEN, str, RTT_CTRL_RESET);
		LOG((char *)"TX",'D', str);
	} else {
		SEGGER_RTT_printf(0, "%s%sTX>%s", RTT_CTRL_TEXT_BLACK,
		RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, "%s%s>len:%d%s\n", RTT_CTRL_TEXT_BRIGHT_WHITE,
		RTT_CTRL_BG_GREEN, strlen(str), RTT_CTRL_RESET);
	}
	//HAL_UART_Transmit(&huart3, (uint8_t*)str, strlen(str), 1000);


	int index = 0;
	DMA1_Stream3->CR &= ~DMA_SxCR_EN; //Выключаем DMA
	DMA1_Stream3->CR &= ~DMA_SxCR_HTIE;
	DMA1_Stream3->CR |= DMA_SxCR_TCIE;
	DMA1_Stream3->PAR = 0x40004804;

	while (1) {

		while (1) {
			dma_str[index % 1024] = str[index];
			index++;

			if (index >= max)
				break;

			if (index % 1024 == 0)
				break;

		}

		U3_DMA_TX_Complete = 0;
		//HAL_UART_Transmit_DMA(&huart3, (uint8_t*)str, strlen(str));
		DMA1_Stream3->CR &= ~DMA_SxCR_EN; //Выключаем DMA

		DMA1_Stream3->CR &= ~DMA_SxCR_HTIE;
		DMA1_Stream3->CR |= DMA_SxCR_TCIE;

		if (index % 1024 == 0)
		  DMA1_Stream3->NDTR = 1024;
        else
          DMA1_Stream3->NDTR = index % 1024;

		DMA1_Stream3->M0AR = (uint32_t) &dma_str[0];
		DMA1_Stream3->CR |= DMA_SxCR_EN; //Включаем DMA

		while (U3_DMA_TX_Complete == 0)
			__NOP();

		if (index >= max)
			break;
	}

}

void BT_Send_toCLI(char *str) {
	char str16[64];
	char str16_1[64 + 64];
	uint8_t crc;

	sprintf(str16, "98#%s", str);

	crc = CRC8(&str16[0], strlen(str16));

	sprintf(str16_1, "!%s;%d$", str16, crc);

	SEGGER_RTT_printf(0, "%s%sTX>%s", RTT_CTRL_TEXT_BLACK,
	RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_WHITE,
	RTT_CTRL_BG_GREEN, str16_1, RTT_CTRL_RESET);

	HAL_UART_Transmit(&huart3, (uint8_t*) &str16_1[0], strlen(str16_1), 1000);

}

//Процедура формирования строки ACK по индексу
void BT_Send_ACK_to_Host(int index) {
	char str16[32];
	char str16_1[32 + 32];
	char crc;

	if (index < 10)
		sprintf(str16, "0%d#ACK", index);
	else
		sprintf(str16, "%d#ACK", index);

	crc = CRC8(&str16[0], 6);

	sprintf(str16_1, "!%s;%d$", str16, crc);

	SEGGER_RTT_printf(0, "%s%sTX>%s", RTT_CTRL_TEXT_BLACK,
	RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_WHITE,
	RTT_CTRL_BG_GREEN, str16_1, RTT_CTRL_RESET);
	HAL_UART_Transmit(&huart3, (uint8_t*) &str16_1[0], strlen(str16_1), 1000);
}

//Процедура формирования строки пересылки(короткий) 64
void BT_Send_String_to_Host(int index, char *outstr) {
	char str16[64];
	char str16_1[64 * 2];
	char crc;

	if (index < 10)
		sprintf(str16, "0%d#%s", index, outstr);
	else
		sprintf(str16, "%d#%s", index, outstr);

	crc = CRC8(&str16[0], strlen(str16));

	sprintf(str16_1, "!%s;%d$", str16, crc);
	SEGGER_RTT_printf(0, "%s%sTX>%s", RTT_CTRL_TEXT_BLACK,
	RTT_CTRL_BG_BRIGHT_YELLOW, RTT_CTRL_RESET);
	SEGGER_RTT_printf(0, "%s%s%s%s\n", RTT_CTRL_TEXT_BRIGHT_WHITE,
	RTT_CTRL_BG_GREEN, str16_1, RTT_CTRL_RESET);
	HAL_UART_Transmit(&huart3, (uint8_t*) &str16_1[0], strlen(str16_1), 1000);
}

//WRITE > CH1 EN
void BT_Port_00(int PosS, int len) {
	SEGGER_RTT_printf(0, "%s%sL2>Code 00 > WRITE > CH1 EN%s\n",
	RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_RESET);

	BT_Send_ACK(0);
	return;
}

//WRITE > CH2 EN
void BLE_Port_01(int PosS, int len) {
	SEGGER_RTT_printf(0, "%s%sL2>Code 01 > WRITE > CH2 EN%s\n",
	RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_RESET);
	BT_Send_ACK(1);
	return;
}

//CLI
void BLE_Port_98(int PosS, int len) {
	SEGGER_RTT_printf(0, "%s%sBLE_Port_98>WRITE>CLI%s\n", RTT_CTRL_TEXT_BLACK,
	RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_RESET);
	char svalue[64] = { 0 };
	memcpy(svalue, &big_buffer[PosS], len);
	SendToCli(&svalue[0], len);
	BT_Send_ACK_to_Host(98);
	return;
}

//Установка Яркости экрана OK
void BLE_Port_99(int PosS, int len) {
	SEGGER_RTT_printf(0, "%s%sL2> Code 99 > WRITE > BackLight%s\n",
	RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_GREEN, RTT_CTRL_RESET);
	char svalue[16] = { 0 };
	memcpy(svalue, &big_buffer[PosS], len);
	TIM9->CCR1 = atoi(svalue);
	BT_Send_ACK_to_Host(99);
	return;
}

#endif
