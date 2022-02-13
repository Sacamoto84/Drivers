#ifndef __LCD_GLOBAL_H
#define __LCD_GLOBAL_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f4xx_hal.h"
#include "myili9341.h"	 
#include "fonts.h"	
#include "lcd_color.h"		 
	 
typedef struct
{    
  short X;
  short Y;
	short Press;
	short Releases;
  short Down;
} _TS;	
	 
	 
//extern volatile uint16_t PixeArray[15360] __attribute__((at(0x10000100)));	 
	 
extern const uint8_t *ch;	 
extern LCD_DrawPropTypeDef lcdprop;	 
	 

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Из myili9341.c	 
void TFT9341_SendCommand(unsigned char cmd);
	
//Функция для передачи данных в память контроллера дисплея
void TFT9341_SendData(unsigned char dt);

//Функция для чтения регистра из котроллера дисплея 
unsigned int TFT9341_ReadReg(unsigned char r);

//Функция ориентации дисплея
void TFT9341_SetRotation(unsigned char r);
//функция перезагрузки дисплея
void TFT9341_reset(void);
void TFT9341_ini(void);

void TFT_Flood(uint16_t color, uint32_t len);
void TFT_SetAddrWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

void TFT9341_FillScreen(uint16_t color);
void TFT9341_FillRectangle(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);

void     TFT9341_SetPixel(uint16_t x, uint16_t y, uint16_t color);
uint16_t TFT9341_GetPixel(int x, int y);

uint16_t TFT9341_RandColor(void);//функция для установки случайного цвета для дисплея
void     TFT_DrawLine(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2);	 
void     TFT_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2); //Функция отрисовки прямоугольника, но уже не закрашенного	 
void     TFT_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color); //Функция рисования на дисплее окружности с определенным радиусом
void     TFT9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

//Строки
void     TFT_SetFont(sFONT *fonts);
void     TFT_SetTextColor(uint16_t color);
void     TFT_SetBackColor(uint16_t color);
void     TFT_DrawChar(uint16_t x, uint16_t y, uint8_t c);
void     TFT_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *Text);
////////////////////////////////////////////////////////////////////////////////////////////////////////
long map(long x, long in_min, long in_max, long out_min, long out_max);

#ifdef __cplusplus
}
#endif
  
#endif 
