#include "myili9341.h"
#include "myili9341_bmp_ico.h"
#include "stm32f4xx_hal.h"
#include "fonts.h"
#include "lcd_global.h"

uint8_t  LCD_Buffer_4B[38400]; //4bit Buffer Palletra 38400
uint16_t LCD_Palette_4B[16];  //Palitra na 16 chvetov

uint16_t LCD_Palette;

void TFT9341_SetColorToPallete(uint8_t index, uint16_t color)
{
	if (index>15) return;
	LCD_Palette_4B[index] = color;
}

void TFT9341_SetPixel_4B(uint16_t x, uint16_t y,  uint8_t index)
{
	if((x>=240)||(y>=320)) return;
	
	if (x%2) 
	{
		LCD_Buffer_4B[x/2+y*120]= (LCD_Buffer_4B[x/2+y*120] & 0x0F) | (index<<4);
  }
  else
  {
		LCD_Buffer_4B[x/2+y*120]= (LCD_Buffer_4B[x/2+y*120] & 0xF0) | index;	
  }
}

void TFT9341_Update_Pallete(void)
{
  uint32_t i,ii,r,a, b;
	TFT_SetAddrWindow(0,0,239,319);
	TFT9341_SendCommand(0x2C);
	
	for(i=0;i<160*240;i++)
	{ 
	    a = LCD_Buffer_4B[i];
		  r = LCD_Palette_4B[ a &0x0F];
      TFT9341_SendData(r >> 8);
			TFT9341_SendData(r & 0xFF);
	    r = LCD_Palette_4B[ a >> 4 ];
      TFT9341_SendData(r >> 8);
			TFT9341_SendData(r & 0xFF);
  }
	
}

void TFT9341_FillRectangle_4B(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t index)
{
	for(uint16_t i=x1;i<x2;i++) 
	{
		for(uint16_t ii=y1;ii<y2;ii++)
		  TFT9341_SetPixel_4B(i, ii, index);	
	}

}

void TFT9341_FillScreen_4B(uint8_t index)
{
  //uint8_t i;
	//i = index<<4 | index;
	memset (LCD_Buffer_4B, (index<<4 | index) ,38400);
}

