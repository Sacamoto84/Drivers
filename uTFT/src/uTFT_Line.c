#include "uTFT_define.h"

void uTFT_DrawLine(uTFT_LCD_t * LCD, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp; 
	
	/* Check for overflow */
	if (x0 >= LCD->TFT_WIDTH) {x0 = LCD->TFT_WIDTH  - 1;}
	if (x1 >= LCD->TFT_WIDTH) {x1 = LCD->TFT_WIDTH  - 1;}
	if (y0 >= LCD->TFT_HEIGHT){y0 = LCD->TFT_HEIGHT - 1;}
	if (y1 >= LCD->TFT_HEIGHT){y1 = LCD->TFT_HEIGHT - 1;}
	
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1); 
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1); 
	sx = (x0 < x1) ? 1 : -1; 
	sy = (y0 < y1) ? 1 : -1; 
	err = ((dx > dy) ? dx : -dy) / 2; 

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			uTFT_SetPixel(LCD, x0, i, c);
		}
		
		/* Return from function */
		return;
	}
	
	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}
		
		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}
		
		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			uTFT_SetPixel(LCD, i, y0, c);
		}
		
		/* Return from function */
		return;
	}
	
	while (1) {
		uTFT_SetPixel(LCD, x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err; 
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		} 
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		} 
	}
}

void uTFT_VLine(uTFT_LCD_t * LCD, int32_t X, int32_t Y1, int32_t Y2, uint16_t color)
{
	
	if ((LCD->Bit) == 1)
  {
		uint32_t i;
		
		if (color == 1)
		{			
		  for ( i = Y1; i <= Y2; i++)  
		    LCD->buffer8[X + (i / 8) * LCD->TFT_WIDTH]  |= 1 << (i % 8);	 
		}	
		else
		{
		  for ( i = Y1; i <= Y2; i++)  
		    LCD->buffer8[X + (i / 8) * LCD->TFT_WIDTH]  &= ~(1 << (i % 8));
		}				
	}
	
	
	
	
	
	if ((LCD->Bit) == 4)
  {
			uint32_t i;
		  for ( i = Y1; i <= Y2; i++) //uTFT_SetPixel(X, i, color); 
			{
        uTFT_SetPixel(LCD, X, i, color);
			} 
	}

	if (LCD->Bit == 16)
  {
			if (Y2 >= LCD->TFT_HEIGHT) Y2 = LCD->TFT_HEIGHT - 1;   
	    if (X >= LCD->TFT_WIDTH) return;
		
	 	 uint32_t i;
		  for ( i = Y1; i <= Y2; i++) //uTFT_SetPixel(X, i, color); 
			{
        LCD->buffer16[X + i * LCD->TFT_WIDTH]= color;	
			}
	}
}

void uTFT_HLine(uTFT_LCD_t * LCD, int32_t Y , int32_t X1, int32_t X2, uint16_t color)
{
	if (X2 >= LCD->TFT_WIDTH) X2 = LCD->TFT_WIDTH - 1;   
	if (Y >= LCD->TFT_HEIGHT) return;

	
	if ((LCD->Bit) == 1)
  {
		uint32_t i;
		
		if (color == 1)
		{			
		  for ( i = X1; i <= X2; i++)  
		    LCD->buffer8[i + (Y / 8) * LCD->TFT_WIDTH]  |= 1 << (Y % 8);	 
		}	
		else
		{
		  for ( i = X1; i <= X2; i++)  
		    LCD->buffer8[i + (Y / 8) * LCD->TFT_WIDTH]  &= ~(1 << (Y % 8));
		}				
	}
	
	if ((LCD->Bit) == 16)
  {
		uint32_t i;
		for ( i = X1; i <= X2; i++) //uTFT_SetPixel(X, i, color); 
		{
		 LCD->buffer16[i + Y * LCD->TFT_WIDTH]= color;	
		}
	}
	
	if ((LCD->Bit) == 4)
  {
		uint32_t i;
		for ( i = X1; i <= X2; i++) //uTFT_SetPixel(X, i, color); 
		{
			if (i%2 == 0)
			{
				LCD->buffer8[i/2+Y*((LCD->TFT_WIDTH)/2)] = (LCD->buffer8[i/2+Y*((LCD->TFT_WIDTH)/2)] & (0x0F)) | (color<<4);
			}
			else
			{
				LCD->buffer8[i/2+Y*((LCD->TFT_WIDTH)/2)] =  (LCD->buffer8[i/2+Y*((LCD->TFT_WIDTH)/2)] & (0xF0)) | color;
			}		
		}
	}
	
	
}

void uTFT_HLineW(uTFT_LCD_t* LCD , int32_t x , int32_t y, int32_t w, uint16_t color)
{
	uTFT_HLine(LCD, y , x,  x + w - 1, color);
}

/////////////////////////////////////////////////////////////////////
uint16_t uTFT_oldX;
uint16_t uTFT_oldY;

void uTFT_MoveToLine(uTFT_LCD_t* LCD, int32_t x, int32_t y, uint16_t c)
{
	uTFT_DrawLine(LCD, uTFT_oldX, uTFT_oldY,  x,  y,  c);
	uTFT_oldX = x;
	uTFT_oldY = y;
}

void uTFT_MoveSetXY(uint32_t x, uint32_t y)
{
	uTFT_oldX = x;
	uTFT_oldX = y;
}
/////////////////////////////////////////////////////////////////////

