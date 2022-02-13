#include "uTFT_define.h"

void uTFT_DrawRectangle(uTFT_LCD_t * LCD, int32_t x, int32_t y, uint16_t w, uint16_t h, uint16_t c) {
	/* Check input parameters */
	if (
		x >= LCD->TFT_WIDTH ||
		y >= LCD->TFT_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= LCD->TFT_WIDTH) {
		w = LCD->TFT_WIDTH - x;
	}
	if ((y + h) >= LCD->TFT_HEIGHT) {
		h = LCD->TFT_HEIGHT - y;
	}
	
	/* Draw 4 lines */
	uTFT_HLine(LCD, y , x,  x + w,  c);
	uTFT_HLine(LCD, y + h , x, x + w,  c);
	
	uTFT_VLine(LCD, x, y, y + h , c);
	uTFT_VLine(LCD, x + w, y, y + h , c);
}

void uTFT_DrawFilledRectangle(uTFT_LCD_t * LCD, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c) {
	uint32_t i;
	
	/* Check input parameters */
	if (
		x >= LCD->TFT_WIDTH ||
		y >= LCD->TFT_HEIGHT
	) {
		/* Return error */
		return;
	}
	
	/* Check width and height */
	if ((x + w) >= LCD->TFT_WIDTH) {
		w = LCD->TFT_WIDTH - x;
	}
	if ((y + h) >= LCD->TFT_HEIGHT) {
		h = LCD->TFT_HEIGHT - y;
	}
	
	/* Draw lines */
	for (i = 0; i <= h; i++) {
		uTFT_HLine(LCD, y+i ,  x,  x + w, c); //124us
	}
}

//void uTFT_DrawRectangle(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
//{
//  ST7735_HLine(X1,X2,Y1,color);
//  ST7735_HLine(X1,X2,Y2,color);
//  ST7735_VLine(X1,Y1,Y2,color);
//  ST7735_VLine(X2,Y1,Y2,color);
//}

//Инверсия цветов в данном прямоугольнике
void uTFT_InvertRectangle(uTFT_LCD_t * LCD, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
	for(uint8_t i=x; i<= x+w;i++)
	 for(uint16_t ii = y; ii<=y+h;ii++)	
	    uTFT_SetPixel(LCD,i,ii,!uTFT_GetPixel(LCD,i,ii));	 
}

//Замена цветов в данном прямоугольнике
void uTFT_ChangeColorRectangle(uTFT_LCD_t * LCD, int32_t x, int32_t y, uint32_t w, int32_t h, uint16_t sColor, uint16_t dColor)
{
	for(uint16_t i = x; i <= x+w; i++)
	 for(uint16_t ii = y; ii <= y+h; ii++)	
	  {
      if (uTFT_GetPixel(LCD, i,ii) == sColor)
				uTFT_SetPixel(LCD, i, ii, dColor);
		}
}
