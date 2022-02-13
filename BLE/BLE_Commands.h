/*
 * BLE_Commands.h
 *
 *  Created on: 3 дек. 2021 г.
 *      Author: Ivan
 */

#ifndef BLE_COMMANDS_H_
#define BLE_COMMANDS_H_

 #define SIZE_big_buffer 1024*4+16

void BT_Send_RAW(char * str);
void BT_Send_toCLI(char * str);
void BT_Send_ACK(int index);
void BT_Send_String_to_Host(int index, char * outstr);
void BT_Port_00(int PosS, int len);
void BLE_Port_01(int PosS, int len);
//CLI
void BLE_Port_98(int PosS, int len);
//Установка Яркости экрана OK
void BLE_Port_99(int PosS, int len);

void BLE_Task(void);

uint8_t CRC8(char *pcBlock, unsigned int len); //Расчет CRC8

void taskCLI_setup(void);

#endif /* BLE_COMMANDS_H_ */
