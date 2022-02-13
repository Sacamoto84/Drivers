#include "myili9341.h"
#include "myili9341_bmp_ico.h"
#include "stm32f4xx_hal.h"
#include "fonts.h"
#include "lcd_global.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

volatile uint16_t PixeArray[15360/2] __attribute__((at(0x10001000))); //Расположим в CCM RAM для хранения макс 240x64x2 

void     TFT_DrawPixel_RAM(uint16_t x, uint16_t y, uint16_t color);
uint16_t TFT9341_GetPixel_RAM(int x, int y);


void TFT_DrawPixel_RAM(uint16_t x, uint16_t y, uint16_t color) //Работает
{
    if((x<0)||(y<0)||(x>=240)||(y>=64)) return;
    PixeArray[x+y*240]= color;
}

uint16_t TFT9341_GetPixel_RAM(int x, int y)
{
  	//uint16_t dt;
    if((x<0)||(y<0)||(x>=240)||(y>=64)) return -1;
	  return PixeArray[x+y*240];
}

void TFT_DrawLine_RAM(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
        static int steep;
					steep = abs(y2-y1)>abs(x2-x1);
        if (steep)
        {
                swap(x1,y1);
                swap(x2,y2);
        }
        if(x1>x2)
        {
                swap(x1,x2);
                swap(y1,y2);
        }
				
       static int dx,dy;
        dx=x2-x1;
        dy=abs(y2-y1);
				if (dx>240) return;
				
				
       static int err;
			  err=dx/2;
       static int ystep;
        
				if(y1<y2)           ystep = 1;
        else                ystep = -1;
				
        for (;x1<=x2;x1++)
        {
                if (steep)        
									TFT_DrawPixel_RAM(y1,x1,color);
                else 
									TFT_DrawPixel_RAM(x1,y1,color);
                err-=dy;
                if (err<0)
                {
                        y1 += ystep;
                        err+=dx;
                }
        }
}

void TFT_DrawHLine_RAM(uint16_t color, uint16_t x1, uint16_t x2, uint16_t y)
{      
        int dx,dy;
        dx=x2-x1;
	      int i;      
	      for(i=x1;i<x2;i++) TFT_DrawPixel_RAM( i, y, color);       
}

//Функция отрисовки прямоугольника, но уже не закрашенного
void TFT_DrawRect_RAM(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
        TFT_DrawLine_RAM(color,x1,y1,x2,y1);
        TFT_DrawLine_RAM(color,x2,y1,x2,y2);
        TFT_DrawLine_RAM(color,x1,y1,x1,y2);
        TFT_DrawLine_RAM(color,x1,y2,x2,y2);
}

void TFT_DrawFilledCircle_RAM(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	
	static int16_t f; 
	static int16_t ddF_x;
	static int16_t ddF_y;
	static int16_t x = 0;
	static int16_t y;
	
	f = 1 - r;
	ddF_x = 1;
	ddF_y	= -2 * r;
	x = 0;
	y = r;

    TFT_DrawPixel_RAM(x0, y0 + r, color);
    TFT_DrawPixel_RAM(x0, y0 - r, color);
    TFT_DrawPixel_RAM(x0 + r, y0, color);
    TFT_DrawPixel_RAM(x0 - r, y0, color);
    TFT_DrawLine_RAM(color, x0 - r, y0, x0 + r, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TFT_DrawLine_RAM(color, x0 - x, y0 + y, x0 + x, y0 + y);
        TFT_DrawLine_RAM(color, x0 + x, y0 - y, x0 - x, y0 - y);
        TFT_DrawLine_RAM(color, x0 + y, y0 + x, x0 - y, y0 + x);
        TFT_DrawLine_RAM(color, x0 + y, y0 - x, x0 - y, y0 - x);
    }
}

//Работает
void TFT_FillScreen_RAM(uint16_t color)
{
  uint16_t i;
	for(i=0;i<15360;i++) PixeArray[i] = color;
}
//—————————————————————
void TFT_FillRectangle_RAM(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
        //TFT_SetAddrWindow(x1, y1, x2, y2);
        //TFT_Flood(color, (uint16_t)(x2-x1+1) * (uint16_t)(y2-y1+1));
	int y;   
	for(y=y1;y<y2;y++)
		       TFT_DrawHLine_RAM(color, x1, x2, y);
	
}


void TFT9341_Render_RAM(uint16_t x, uint16_t y,uint16_t dx, uint16_t dy)
{
  uint16_t i,ii,r;
	TFT_SetAddrWindow(x,y,x+dx-1,y+dy-1);
	TFT9341_SendCommand(0x2C);
	for(i=0;i<dy;i++)
	{
	  for(ii=0;ii<dx;ii++)
	  {
		  r = PixeArray[ii+i*240];
      TFT9341_SendData(r >> 8);TFT9341_SendData(r & 0xFF);
	   }
  }
}

void TFT_DrawChar_RAM(uint16_t x, uint16_t y, uint8_t c)
{
	uint32_t i=0, j=0;
	uint16_t height, width;
	uint8_t offset;
	uint8_t *pchar;
	uint32_t line;
	ch = &lcdprop.pFont->table[(c-' ') * lcdprop.pFont->Height *((lcdprop.pFont->Width +7) / 8)];
	height = lcdprop.pFont->Height;
	width = lcdprop.pFont->Width;
	offset = 8 * ((width + 7)/8) - width;
	for(i = 0;i < height; i++)
	{
		pchar = ((uint8_t *)ch + (width + 7)/8 * i);
		switch((width + 7)/8)
		{
		case 1:
			line = pchar[0];
			break;
		case 2:
			line = (pchar[0]<<8) | pchar[1];
			break;
		case 3:
		default:
			line = (pchar[0]<<16) | (pchar[1]<<8) | pchar[2];
			break;
		}
		for(j = 0;j < width; j++)
		{
			if(line & (1 << (width - j + offset -1)))
			{
				TFT_DrawPixel_RAM((x+j), y, lcdprop.TextColor);
				
			}
			else
			{
				TFT_DrawPixel_RAM((x+j), y, lcdprop.BackColor);
			}
		}
		y++;
	}
}


void TFT_DisplayString_RAM(uint16_t Xpos, uint16_t Ypos, uint8_t *Text)
{
	uint16_t ref_column = 1, i = 0;
	uint32_t size = 0, xsize = 0;
	uint8_t *ptr = Text;
	while(*ptr++) size++;
  ref_column = Xpos;
	if((ref_column < 1) || (ref_column >= 0x8000))
	{
		ref_column = 1;
	}
	while ((*Text != 0) & (((240 - (i*lcdprop.pFont->Width)) & 0xFFFF) >= lcdprop.pFont->Width))
	{
		TFT_DrawChar_RAM(ref_column, Ypos, *Text);
		ref_column += lcdprop.pFont->Width;
		Text++;
		i++;
	}
}


//Функция рисования на дисплее окружности с определенным радиусом
void TFT_DrawCircle_RAM(uint16_t x0, uint16_t y0, int r, uint16_t color)
{
        int f = 1-r;
        int ddF_x=1;
        int ddF_y=-2*r;
        int x = 0;
        int y = r;
        TFT_DrawPixel_RAM(x0,y0+r,color);
        TFT_DrawPixel_RAM(x0,y0-r,color);
        TFT_DrawPixel_RAM(x0+r,y0,color);
        TFT_DrawPixel_RAM(x0-r,y0,color);
        while (x<y)
        {
                if (f>=0)
                {
                        y--;
                        ddF_y+=2;
                        f+=ddF_y;
                }
                x++;
                ddF_x+=2;
                f+=ddF_x;
                TFT_DrawPixel_RAM(x0+x,y0+y,color);
                TFT_DrawPixel_RAM(x0-x,y0+y,color);
                TFT_DrawPixel_RAM(x0+x,y0-y,color);
                TFT_DrawPixel_RAM(x0-x,y0-y,color);
                TFT_DrawPixel_RAM(x0+y,y0+x,color);
                TFT_DrawPixel_RAM(x0-y,y0+x,color);
                TFT_DrawPixel_RAM(x0+y,y0-x,color);
                TFT_DrawPixel_RAM(x0-y,y0-x,color);
        }
}

//Функция рисования на дисплее окружности с определенным радиусом
void TFT_DrawCircle_R_RAM(uint16_t x0, uint16_t y0, int r, uint16_t color)
{
        int f = 1-r;
        int ddF_x=1;
        int ddF_y=-2*r;
        int x = 0;
        int y = r;
        TFT_DrawPixel_RAM(x0,y0+r,color);
        TFT_DrawPixel_RAM(x0,y0-r,color);
        TFT_DrawPixel_RAM(x0+r,y0,color);
        //TFT_DrawPixel_RAM(x0-r,y0,color);
        while (x<y)
        {
                if (f>=0)
                {
                        y--;
                        ddF_y+=2;
                        f+=ddF_y;
                }
                x++;
                ddF_x+=2;
                f+=ddF_x;
                TFT_DrawPixel_RAM(x0+x,y0+y,color);
                //TFT_DrawPixel_RAM(x0-x,y0+y,color);
                TFT_DrawPixel_RAM(x0+x,y0-y,color);
                //TFT_DrawPixel_RAM(x0-x,y0-y,color);
                TFT_DrawPixel_RAM(x0+y,y0+x,color);
                //TFT_DrawPixel_RAM(x0-y,y0+x,color);
                TFT_DrawPixel_RAM(x0+y,y0-x,color);
                //TFT_DrawPixel_RAM(x0-y,y0-x,color);
        }
}
//Функция рисования на дисплее окружности с определенным радиусом
void TFT_DrawCircle_L_RAM(uint16_t x0, uint16_t y0, int r, uint16_t color)
{
        int f = 1-r;
        int ddF_x=1;
        int ddF_y=-2*r;
        int x = 0;
        int y = r;
        TFT_DrawPixel_RAM(x0,y0+r,color);
        //TFT_DrawPixel_RAM(x0,y0-r,color);
        //TFT_DrawPixel_RAM(x0+r,y0,color);
        //TFT_DrawPixel_RAM(x0-r,y0,color);
        while (x<y)
        {
                if (f>=0)
                {
                        y--;
                        ddF_y+=2;
                        f+=ddF_y;
                }
                x++;
                ddF_x+=2;
                f+=ddF_x;
                //TFT_DrawPixel_RAM(x0+x,y0+y,color);
                TFT_DrawPixel_RAM(x0-x,y0+y,color);
                //TFT_DrawPixel_RAM(x0+x,y0-y,color);
                TFT_DrawPixel_RAM(x0-x,y0-y,color);
                //TFT_DrawPixel_RAM(x0+y,y0+x,color);
                TFT_DrawPixel_RAM(x0-y,y0+x,color);
                //TFT_DrawPixel_RAM(x0+y,y0-x,color);
                TFT_DrawPixel_RAM(x0-y,y0-x,color);
        }
}
