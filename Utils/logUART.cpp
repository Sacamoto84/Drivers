/*
 * logUART.cpp
 *
 *  Created on: 12 окт. 2022 г.
 *      Author: Ivan
 */
#include "logUART.h"

void classLog::setBold(void)
{
	char s[16];
	sprintf(s, "\33[01m");
	print(s);
}
void classLog::setItalic(void)
{
	char s[16];
	sprintf(s, "\33[03m");
	print(s);
}
void classLog::setUnderline(void)
{
	char s[16];
	sprintf(s, "\33[04m");
	print(s);
}
void classLog::setRevers(void)
{
	char s[16];
	sprintf(s, "\33[07m");
	print(s);
}
void classLog::setFlash(void)
{
	char s[16];
	sprintf(s, "\33[08m");
	print(s);
}
void classLog::color(int color)
{
	char s[16];
	sprintf(s, "\33[38;05;%dm",color);
	print(s);
}
void classLog::bgcolor(int color)
{
	char s[16];
	sprintf(s, "\33[48;05;%dm",color);
	print(s);
}

void classLog::colorString(int c, char const *const format)
{
	color(c);
	print(format);
	print("\33[0m");
}
void classLog::colorString(int color, int bgcolor, char const *const format)
{
	char s[32];
	sprintf(s, "\33[38;05;%d;48;05;%dm",color, bgcolor);
	print(s);
	print(format);
	print("\33[0m");
}
void classLog::colorStringln(int c, char const *const format)
{
	color(c);
	print(format);
	print("\33[0m\n");
}
void classLog::colorStringln(int color, int bgcolor, char const *const format)
{
	char s[32];
	sprintf(s, "\33[38;05;%d;48;05;%dm",color, bgcolor);
	print(s);
	print(format);
	print("\33[0m\n");
}

void classLog::warning(char const *const format) {colorStringln(11, format);}
void classLog::error(char const *const format) {colorStringln(9, format);}
void classLog::info(char const *const format) {colorStringln(45, format);}
void classLog::successful(char const *const format) {colorStringln(10, format);}

void classLog::print(char const *format) {

	if (useDMA == false)
		HAL_UART_Transmit(huart, (const uint8_t*) format, strlen(format),
				1000);
	else {
		waitComplete();
		int size = strlen(format);
		if (size)
			HAL_UART_Transmit_DMA(huart, (const uint8_t*) format, size);
		else
			*dma_completed = 1;
	}

}







