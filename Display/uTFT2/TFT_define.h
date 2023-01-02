/*
 * TFT_define.h
 *
 *  Created on: 1 янв. 2023 г.
 *      Author: Ivan
 */

#ifndef TFT_DEFINE_H_
#define TFT_DEFINE_H_

#include "main.h"

#define ABS(x)   ((x) > 0 ? (x) : -(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

typedef struct {
	int16_t  CurrentX;
	int16_t  CurrentY;
	uint16_t Color;
	uint16_t BColor;
	uint8_t  Inverted;
	uint8_t  GetColor; //1-Нужно читать цвет фона, 0-Использовать цвет BColor
} uTFT_t;

typedef struct {
	int16_t x0;
	int16_t y0;
	int16_t x1;
	int16_t y1;
	int16_t W;
	int16_t H;
} List_Update_Particle;



#endif /* TFT_DEFINE_H_ */
