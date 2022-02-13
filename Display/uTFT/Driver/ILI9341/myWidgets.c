#include "lcd_global.h"
//#include "interrupts_key.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


void GUI_Widgets_Rules(uint16_t x, uint16_t y, uint16_t min, uint16_t max , uint16_t value)
{
	
	value =  map(value, min , max, 0, 200);
	
	TFT9341_FillRectangle(BLACK, x-16, y-2,x+200+16, y+60);
	TFT_SetTextColor(GREEN);
  TFT_SetBackColor(BLACK);
	TFT9341_FillRectangle(WHITE, x, y,x+200, y+32);
	TFT9341_DrawFilledCircle( x, y+16, 16, WHITE);
	TFT9341_DrawFilledCircle( x+200, y+16, 16, WHITE);
	TFT9341_DrawFilledCircle( x+value, y+9+7, 16, RED);
	char str[16];
	sprintf(str, "%d", value);
	TFT_DisplayString(x+80, y+36, str);
}

void GUI_Widgets_Rules_RAM(uint16_t min, uint16_t max , uint16_t value)
{
	value =  map(value, min , max, 0, 200);
	
	uint16_t x,y;
	
	x=18;
	y=10;
	
	//TFT_FillRectangle_RAM(BLACK, x-16, y-2,x+200+16, y+60);
	TFT_SetTextColor(WHITE);
  TFT_SetBackColor(BLACK);
	
	TFT_FillRectangle_RAM(WHITE, x, 9, x+200, y+33);
	TFT_DrawFilledCircle_RAM( x, 10+15, 16, 0x6565);
	TFT_DrawFilledCircle_RAM( x+200-1, 10+15, 16, WHITE);
	TFT_FillRectangle_RAM(0x6565, x, 9, x+value, y+32);
	TFT_DrawFilledCircle_RAM( x+value, 10+9+6, 17, 0x324B);
	
	TFT_DrawHLine_RAM(0x324B, x, x+200, 9);
	TFT_DrawHLine_RAM(0x324B, x, x+200, y+31);
	
	TFT_DrawCircle_L_RAM( x, 10+15, 16,       0x324B);
	TFT_DrawCircle_R_RAM( x+200-1, 10+15, 16, 0x324B);
	
	
	char str[16];
	sprintf(str, "%d", value);
	TFT_DisplayString_RAM(10+80, 10+3, str);
	
}

void GUI_Widgets_Keyboard_All(void)
{
//	if (KEY_0)
//	{
//		TFT_SetTextColor(RED);
//    TFT_SetBackColor(BLACK);
//	  TFT_DrawChar(250, 100, (uint8_t)'1');
//	}
//	else
//	{
//		TFT_SetTextColor(CYAN);
//    TFT_SetBackColor(BLACK);
//	  TFT_DrawChar(250, 100, (uint8_t)'0');
//	}
}

void GUI_Widgets_Trend(void)
{
TFT9341_FillRectangle(WHITE, 0, 0,120, 64);

	
	
	
	
	
	
	
	
	
	
	
	
	
	
}



