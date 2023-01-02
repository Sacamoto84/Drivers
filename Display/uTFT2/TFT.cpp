/*
 * TFT.cpp
 *
 *  Created on: 11 мая 2021 г.
 *      Author: Ivan
 */

#include "TFT.h"

#include <math.h>
#include <stdio.h>

void TFT::SetPixel(int32_t x, int32_t y, uint16_t color) {

	if ((x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return;

	if ((LCD->Bit) == 1) {
		if (color == 1)
			LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] |= 1 << (y % 8);
		else
			LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] &= ~(1 << (y % 8));
		return;
	}

	// 16 цветов
	if ((LCD->Bit) == 4) {
		if (x % 2 == 0) {
			LCD->buffer8[x / 2 + y * ((LCD->TFT_WIDTH) / 2)] = (LCD->buffer8[x
					/ 2 + y * ((LCD->TFT_WIDTH) / 2)] & (0x0F)) | (color << 4);
		} else {
			LCD->buffer8[x / 2 + y * ((LCD->TFT_WIDTH) / 2)] = (LCD->buffer8[x
					/ 2 + y * ((LCD->TFT_WIDTH) / 2)] & (0xF0)) | color;
		}

		return;
	}

	//OK
	if ((LCD->Bit) == 8) {
		LCD->buffer8[x + y * LCD->TFT_WIDTH] = color;
		return;
	}

	//OK
 	if ((LCD->Bit) == 16) {
		LCD->buffer16[x + y * LCD->TFT_WIDTH] = color;
		return;
	}
}

void TFT::SetPixel16(int32_t x, int32_t y, uint16_t color) {

	if ((x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return;

	LCD->buffer16[x + y * LCD->TFT_WIDTH] = color;
}


uint16_t TFT::GetPixel(int32_t x, int32_t y) {

	if (LCD->Bit == 16) {
		if ((x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH)	|| (y >= LCD->TFT_HEIGHT))	return 0;
		return LCD->buffer16[x + y * LCD->TFT_WIDTH];
	}

	return 0; //Не реализовано
}

//Установка цвета в палитре
void TFT::SetColorToPallete(uint8_t index, uint16_t color) {
	//if (index>255) return;
	//LCD->LCD_Buffer_Palette[index] = color;
}



void TFT::Fill16(uint16_t color) {
	    //uint32_t t = (uint32_t)color;
		uint32_t Color = color * 65536  + color;
		uint32_t *p;
		p = (uint32_t *)&LCD->buffer16[0];

		uint32_t max = LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 2  / 4 / 2 / 2;

		while(max--)
		{
			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color;
			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color;

			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color;
			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color;
		}
		return;
}



void TFT::Fill(uint16_t color) {

	if (LCD->Bit == 1) {
		uint8_t c;
		if (color)
			c = 0xFF;
		else
			c = 0;

		for (int32_t i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 8); i++)
			LCD->buffer8[i] = c;
		return;
	}

	if (LCD->Bit == 4) {
		//for (int32_t i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 2) - 1; i++)
		//	LCD->buffer8[i] = color | (color << 4);
		//return;
		//memset (LCD->buffer8, color | (color << 4), LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 2);

		uint32_t *buf = (uint32_t*)LCD->buffer8;
		uint32_t count = LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 8;
		uint32_t c = color | (color << 4) | ((color | (color << 4))<<8) | ((color | (color << 4))<<16) | ((color | (color << 4))<<24);
		while(count--) *buf++ = c;


	}


	if (LCD->Bit == 8) {
		//for (int32_t i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 2) - 1; i++)
		//	LCD->buffer8[i] = color | (color << 4);
		//return;
		//memset (LCD->buffer8, color | (color << 4), LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 2);

		uint32_t *buf = (uint32_t*)LCD->buffer8;
		uint32_t count = LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 4;
		uint32_t c = color | (color<<8) | (color<<16) | (color<<24);
		while(count--) *buf++ = c;


	}

	if (LCD->Bit == 16) {
		//int ii = LCD->TFT_HEIGHT * LCD->TFT_WIDTH;

		//for (int32_t i = 0; i < ii; i++)
		//	LCD->buffer16[i] = color;

		/*
		uint32_t Color = color <<16 & color;
		uint32_t *p;
		p = (uint32_t *)&LCD->buffer16[0];

		uint32_t max = LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 2  / 4 / 2 / 2;

		while(max--)
		{
			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color; //370
			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color; //287

			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color; //
			*p++ = Color; *p++ = Color; *p++ = Color; *p++ = Color; //245
		}
		*/

		return;
	}
}





















uint16_t TFT::alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc)
{
	// For speed use fixed point maths and rounding to permit a power of 2 division
	uint16_t fgR = ((fgc >> 10) & 0x3E) + 1;
	uint16_t fgG = ((fgc >> 4) & 0x7E) + 1;
	uint16_t fgB = ((fgc << 1) & 0x3E) + 1;

	uint16_t bgR = ((bgc >> 10) & 0x3E) + 1;
	uint16_t bgG = ((bgc >> 4) & 0x7E) + 1;
	uint16_t bgB = ((bgc << 1) & 0x3E) + 1;

	// Shift right 1 to drop rounding bit and shift right 8 to divide by 256
	uint16_t r = (((fgR * alpha) + (bgR * (255 - alpha))) >> 9);
	uint16_t g = (((fgG * alpha) + (bgG * (255 - alpha))) >> 9);
	uint16_t b = (((fgB * alpha) + (bgB * (255 - alpha))) >> 9);

	// Combine RGB565 colours into 16 bits
	return (r << 11) | (g << 5) | (b << 0);
}








