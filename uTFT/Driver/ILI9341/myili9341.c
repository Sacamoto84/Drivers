#include "myili9341.h"
#include "myili9341_bmp_ico.h"
#include "stm32f4xx_hal.h"
#include "fonts.h"
#include "lcd_global.h"

extern RNG_HandleTypeDef hrng;


uint16_t X_SIZE = 0;
uint16_t Y_SIZE = 0;
uint32_t dtt=0;

uint32_t  TextColor;
uint32_t  BackColor;
uint8_t *pchar, *str2;
 
LCD_DrawPropTypeDef lcdprop;

const uint8_t *ch;

extern unsigned short bmp1[];

void TFT_FontsIni(void)
{
  lcdprop.BackColor=LCD_COLOR_BLACK;
  lcdprop.TextColor=LCD_COLOR_GREEN;
  lcdprop.pFont=&Font16;
}
void TFT_SetFont(sFONT *fonts){  lcdprop.pFont=fonts;}
void TFT_SetTextColor(uint16_t color){  lcdprop.TextColor=color;}
void TFT_SetBackColor(uint16_t color){  lcdprop.BackColor=color;}

void TFT9341_Delay(uint32_t dly)
{        
    uint32_t i;
    for(i = 0; i < dly; i++);
}

__STATIC_INLINE void DelayMicro(__IO uint32_t micros)
{
        micros *=(SystemCoreClock / 1000000) / 5;
        while (micros--){};
}


void TFT9341_SendCommand(unsigned char cmd)
{
        ADDR_CMD = cmd;
}

void TFT9341_SendData(unsigned char dt)
{       
        ADDR_DATA = dt;  
        TFT9341_Delay(2);
}

unsigned int TFT9341_ReadReg(unsigned char r)
{
        uint32_t id;
        uint8_t x;
        TFT9341_SendCommand(r);
        DelayMicro(50);//50
        x=ADDR_DATA;
        id=x;
        id<<=8;
        DelayMicro(1);//1
        x=ADDR_DATA;
        id|=x;
        id<<=8;
        DelayMicro(1);//1
        x=ADDR_DATA;
        id|=x;
        id<<=8;
        DelayMicro(1);
        x=ADDR_DATA;
        id|=x;
        if(r==0xEF)
        {
                id<<=8;
                DelayMicro(51);//5
                x=ADDR_DATA;
                id|=x;
        }
        DelayMicro(150);//stabilization time
        return id;
}

void TFT9341_SetRotation(unsigned char r)
{
        TFT9341_SendCommand(0x36);
        switch(r)
        {
                case 0:
                TFT9341_SendData(0x48);
                X_SIZE = 240;
                Y_SIZE = 320;
                break;
                case 1:
                TFT9341_SendData(0x28);
                X_SIZE = 320;
                Y_SIZE = 240;
                break;
                case 2:
                TFT9341_SendData(0x88);
                X_SIZE = 240;
                Y_SIZE = 320;
                break;
                case 3:
                TFT9341_SendData(0xE8);
                X_SIZE = 320;
                Y_SIZE = 240;
                break;
        }
}

void TFT9341_reset(void)
{
        RESET_ACTIVE;
        //osDelay(2);
	      HAL_Delay(2);
        RESET_IDLE;
        TFT9341_SendCommand(0x01); //Software Reset
        for (unsigned char i=0;i<3;i++) 
        	TFT9341_SendData(0xFF);
}

void TFT9341_ini(void)
{
        TFT9341_reset();
        //osDelay(100);
	      HAL_Delay(100);
        dtt = TFT9341_ReadReg(0xD3); 
	
	      TFT9341_SendCommand(0x01);//Software Reset
        DelayMicro(1);
        TFT9341_SendCommand(0xCB);//Power Control A
        TFT9341_SendData(0x39);
        TFT9341_SendData(0x2C);
        TFT9341_SendData(0x00);
        TFT9341_SendData(0x34);
        TFT9341_SendData(0x02);
        DelayMicro(1);
        TFT9341_SendCommand(0xCF);//Power Control B
        TFT9341_SendData(0x00);
        TFT9341_SendData(0xC1);
        TFT9341_SendData(0x30);
        DelayMicro(1);
        TFT9341_SendCommand(0xE8);//Driver timing control A
        TFT9341_SendData(0x85);
        TFT9341_SendData(0x00);
        TFT9341_SendData(0x78);
        DelayMicro(1);
        TFT9341_SendCommand(0xEA);//Driver timing control B
        TFT9341_SendData(0x00);
        TFT9341_SendData(0x00);
        DelayMicro(1);
        TFT9341_SendCommand(0xED);//Power on Sequence control
        TFT9341_SendData(0x64);
        TFT9341_SendData(0x03);
        TFT9341_SendData(0x12);
        TFT9341_SendData(0x81);
        DelayMicro(1);
        TFT9341_SendCommand(0xF7);//Pump ratio control
        TFT9341_SendData(0x20);
        DelayMicro(1);
        TFT9341_SendCommand(0xC0);//Power Control 1
        TFT9341_SendData(0x10);
        DelayMicro(1);
        TFT9341_SendCommand(0xC1);//Power Control 2
        TFT9341_SendData(0x10);
        DelayMicro(1);
        TFT9341_SendCommand(0xC5);//VCOM Control 1
        TFT9341_SendData(0x3E);
        TFT9341_SendData(0x28);
        DelayMicro(1);
        TFT9341_SendCommand(0xC7);//VCOM Control 2
        TFT9341_SendData(0x86);
        DelayMicro(1);
        TFT9341_SetRotation(0);
        DelayMicro(1);
        TFT9341_SendCommand(0x3A);//Pixel Format Set
        TFT9341_SendData(0x55);//16bit
        DelayMicro(1);
        TFT9341_SendCommand(0xB1);
        TFT9341_SendData(0x00); //
        TFT9341_SendData(0x10);// ???? ??? 79 ??
        DelayMicro(1);
        TFT9341_SendCommand(0xB6);//Display Function Control
        TFT9341_SendData(0x08);
        TFT9341_SendData(0x82);
        TFT9341_SendData(0x27);//320 ???
        DelayMicro(1);
        TFT9341_SendCommand(0xF2);//Enable 3G (?? ? ?? ???? ? ???
        TFT9341_SendData(0x00);//? ????
        DelayMicro(1);
        TFT9341_SendCommand(0x26);//Gamma set
        TFT9341_SendData(0x02);//Gamma Curve (G2.2) (???? ???? ???
				
        DelayMicro(1);
        TFT9341_SendCommand(0xE0);//Positive Gamma  Correction
        TFT9341_SendData(0x0F);
        TFT9341_SendData(0x31);
        TFT9341_SendData(0x2B);
        TFT9341_SendData(0x0C);
        TFT9341_SendData(0x0E);
        TFT9341_SendData(0x08);
        TFT9341_SendData(0x4E);
        TFT9341_SendData(0xF1);
        TFT9341_SendData(0x37);
        TFT9341_SendData(0x07);
        TFT9341_SendData(0x10);
        TFT9341_SendData(0x03);
        TFT9341_SendData(0x0E);
        TFT9341_SendData(0x09);
        TFT9341_SendData(0x00);
        DelayMicro(1);
        TFT9341_SendCommand(0xE1);//Negative Gamma  Correction
        TFT9341_SendData(0x00);
        TFT9341_SendData(0x0E);
        TFT9341_SendData(0x14);
        TFT9341_SendData(0x03);
        TFT9341_SendData(0x11);
        TFT9341_SendData(0x07);
        TFT9341_SendData(0x31);
        TFT9341_SendData(0xC1);
        TFT9341_SendData(0x48);
        TFT9341_SendData(0x08);
        TFT9341_SendData(0x0F);
        TFT9341_SendData(0x0C);
        TFT9341_SendData(0x31);
        TFT9341_SendData(0x36);
        TFT9341_SendData(0x0F);
        DelayMicro(1);
        TFT9341_SendCommand(0x11);//????? ???? ???
        //osDelay(150);
				HAL_Delay(150);
        TFT9341_SendCommand(0x29);//??????????
        TFT9341_SendData(0x2C);
        //osDelay(150);
				HAL_Delay(150);
        TFT_FontsIni();
}


void TFT_Flood(uint16_t color, uint32_t len)
{
        uint16_t blocks;
        uint8_t i, hi = color>>8, lo=color;
	      
        TFT9341_SendCommand(0x2C);
        TFT9341_SendData(hi);
        DelayMicro(1);
        TFT9341_SendData(lo);
        len--;
        blocks=(uint16_t)(len/64);//64 pixels/block
        while(blocks--)
        {
                i=16;
                do
                {
                        TFT9341_SendData(hi);			
                        TFT9341_SendData(lo);
                        TFT9341_SendData(hi);												
                        TFT9341_SendData(lo);
                        TFT9341_SendData(hi);
                        TFT9341_SendData(lo);
                        TFT9341_SendData(hi);
                        TFT9341_SendData(lo);
									
                } while (--i);
        }
        //Fill any remaining pixels(1 to 64)
				
        for(i=(uint8_t)len & 63; i--;)
        {
                TFT9341_SendData(hi);
                TFT9341_SendData(lo);
        }
}

void TFT_SetAddrWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
  TFT9341_SendCommand(0x2A);//Column Addres Set
  TFT9341_SendData(x1 >> 8);
  TFT9341_SendData(x1 & 0xFF);
  TFT9341_SendData(x2 >> 8);
  TFT9341_SendData(x2 & 0xFF);
  DelayMicro(1);
  TFT9341_SendCommand(0x2B);//Page Addres Set
  TFT9341_SendData(y1 >> 8);
  TFT9341_SendData(y1 & 0xFF);
  TFT9341_SendData(y2 >> 8);
  TFT9341_SendData(y2 & 0xFF);
  DelayMicro(1);
}

void TFT9341_FillScreen(uint16_t color)
{
        //TFT_SetAddrWindow(0,0,320-1,240-1);
	      TFT_SetAddrWindow(0,0,240-1,320-1);
        TFT_Flood(color,(long)320*(long)240);
}

void TFT9341_FillRectangle(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
        TFT_SetAddrWindow(x1, y1, x2, y2);
        TFT_Flood(color, (uint16_t)(x2-x1+1) * (uint16_t)(y2-y1+1));
}


void TFT9341_SetPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if((x<0)||(y<0)||(x>=240)||(y>=320)) return;
    TFT_SetAddrWindow(x, y, x, y);
    TFT9341_SendCommand(0x2C);
    TFT9341_SendData(color >> 8);
    TFT9341_SendData(color & 0xFF);
}

uint16_t TFT9341_GetPixel(int x, int y)
{
  	uint16_t dt;
    if((x<0)||(y<0)||(x>=240)||(y>=320)) return -1;
    TFT_SetAddrWindow(x, y, x, y);
    TFT9341_SendCommand(0x2E);
	  dt = ADDR_DATA;
	  dt = dt << 8;
	  TFT9341_Delay(1);
	  dt |= ADDR_DATA;
	  return dt;
}

void TFT_DrawChar(uint16_t x, uint16_t y, uint8_t c)
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
				uTFT_SetPixel((x+j), y, lcdprop.TextColor);
				
			}
			else
			{
				uTFT_SetPixel((x+j), y, lcdprop.BackColor);
			}
		}
		y++;
	}
}

void TFT_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *Text)
{
	uint16_t ref_column = 1, i = 0;
	uint32_t size = 0, xsize = 0;
	uint8_t *ptr = Text;
	while(*ptr++) size++ ;
	xsize = (X_SIZE/lcdprop.pFont->Width); 
  ref_column = Xpos;
	if((ref_column < 1) || (ref_column >= 0x8000))
	{
		ref_column = 1;
	}
	while ((*Text != 0) & (((X_SIZE - (i*lcdprop.pFont->Width)) & 0xFFFF) >= lcdprop.pFont->Width))
	{
		TFT_DrawChar(ref_column, Ypos, *Text);
		ref_column += lcdprop.pFont->Width;
		Text++;
		i++;
	}
}

uint16_t TFT9341_RandColor(void)
{
        return HAL_RNG_GetRandomNumber(&hrng)&0x0000FFFF;
}

void TFT_DrawLine(uint16_t color, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
        int steep = abs(y2-y1)>abs(x2-x1);
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
        int dx,dy;
        dx=x2-x1;
        dy=abs(y2-y1);
        int err=dx/2;
        int ystep;
        if(y1<y2)        ystep = 1;
        else                ystep = -1;
        for (;x1<=x2;x1++)
        {
                if (steep)        TFT9341_SetPixel(y1,x1,color);
                else TFT9341_SetPixel(x1,y1,color);
                err-=dy;
                if (err<0)
                {
                        y1 += ystep;
                        err+=dx;
                }
        }
}

void TFT_DrawRect(uint16_t color, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
        TFT_DrawLine(color,x1,y1,x2,y1);
        TFT_DrawLine(color,x2,y1,x2,y2);
        TFT_DrawLine(color,x1,y1,x1,y2);
        TFT_DrawLine(color,x1,y2,x2,y2);
}

void TFT_DrawCircle(uint16_t x0, uint16_t y0, int r, uint16_t color)
{
        int f = 1-r;
        int ddF_x=1;
        int ddF_y=-2*r;
        int x = 0;
        int y = r;
        TFT9341_SetPixel(x0,y0+r,color);
        TFT9341_SetPixel(x0,y0-r,color);
        TFT9341_SetPixel(x0+r,y0,color);
        TFT9341_SetPixel(x0-r,y0,color);
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
                TFT9341_SetPixel(x0+x,y0+y,color);
                TFT9341_SetPixel(x0-x,y0+y,color);
                TFT9341_SetPixel(x0+x,y0-y,color);
                TFT9341_SetPixel(x0-x,y0-y,color);
                TFT9341_SetPixel(x0+y,y0+x,color);
                TFT9341_SetPixel(x0-y,y0+x,color);
                TFT9341_SetPixel(x0+y,y0-x,color);
                TFT9341_SetPixel(x0-y,y0-x,color);
        }
}

void TFT9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    TFT9341_SetPixel(x0, y0 + r, color);
    TFT9341_SetPixel(x0, y0 - r, color);
    TFT9341_SetPixel(x0 + r, y0, color);
    TFT9341_SetPixel(x0 - r, y0, color);
    TFT_DrawLine(color, x0 - r, y0, x0 + r, y0);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        TFT_DrawLine(color, x0 - x, y0 + y, x0 + x, y0 + y);
        TFT_DrawLine(color, x0 + x, y0 - y, x0 - x, y0 - y);

        TFT_DrawLine(color, x0 + y, y0 + x, x0 - y, y0 + x);
        TFT_DrawLine(color, x0 + y, y0 - x, x0 - y, y0 - x);
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t RGB( uint8_t r8, uint8_t g8, uint8_t b8)
{
uint16_t  r5 = ( (float) r8 * 31.0f / 255.0f + 0.5f );
uint16_t  g6 = ( (float) g8 * 63.0f / 255.0f + 0.5f );
uint16_t  b5 = ( (float) b8 * 31.0f / 255.0f + 0.5f );
return (uint16_t) (r5 <<11 | g6 << 5 | b5); 	
}



