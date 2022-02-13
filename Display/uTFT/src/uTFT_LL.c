#include "uTFT_define.h"
	
volatile uint8_t uTFT_DMA_completed; 


//////////////////////////////////////////////////////////////////////////////////////////

uint32_t dma_color;

uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B)
{
  return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
}


uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); }
	
void uTFT_Fill(uTFT_LCD_t * LCD, uint16_t color) {
 	
if (LCD->Bit == 1)
{
	uint8_t c;
	if (color) c =0xFF;
	else c = 0;
	
	for(uint32_t i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 8) ; i++)		
		LCD->buffer8[i] = c;
	return;
}
		
	
	
if (LCD->Bit == 4)
{
	for(uint32_t i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH/2) - 1; i++)		
		LCD->buffer8[i] = color | (color << 4);
	return;
}
	
	
if (LCD->Bit == 16)
{
	for(uint32_t i = 0; i < (LCD->TFT_HEIGHT * LCD->TFT_WIDTH) - 1; i++)		
		LCD->buffer16[i] = color;
	return;
}
}


//Установка цвета в палитре
void uTFT_SetColorToPallete(uTFT_LCD_t * LCD, uint8_t index, uint16_t color)
{
	if (index>255) return;
	#if ((LCD_Buffer_Color_Bit != 16) && (LCD_Buffer_Color_Bit != 1)) 
	 // LCD_Buffer_Palette[index] = color;
  #endif
}

void uTFT_SetPixel(uTFT_LCD_t * LCD, int32_t x, int32_t y, uint16_t color) {
	
  if ((x<0)||(y<0)||(x >= LCD->TFT_WIDTH )||(y >= LCD->TFT_HEIGHT)) return;
			
	if ((LCD->Bit) == 1)
	{		
		if (color == 1) 
		LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH]  |= 1 << (y % 8);
	  else 
		LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH]  &= ~(1 << (y % 8));	
	}
	
	// 16 цветов
	if ((LCD->Bit) == 4)
  {		
			if (x%2 == 0)
			{	LCD->buffer8[x/2+y*((LCD->TFT_WIDTH)/2)] = (LCD->buffer8[x/2+y*((LCD->TFT_WIDTH)/2)] & (0x0F)) | (color<<4); }
			else
			{	LCD->buffer8[x/2+y*((LCD->TFT_WIDTH)/2)] =  (LCD->buffer8[x/2+y*((LCD->TFT_WIDTH)/2)] & (0xF0)) | color;  	 }	
		
		return;
	}	
	
	//OK
	if ((LCD->Bit) == 16)
  {		
	    LCD->buffer16[x + y*LCD->TFT_WIDTH]= color;
		  return;
	}
}

uint16_t uTFT_GetPixel(uTFT_LCD_t * LCD, int32_t x, int32_t y) {	
	
	
	
	if (LCD->Bit == 16)
  {	
	  if ((x<0)||(y<0)||(x >= LCD->TFT_WIDTH )||	(y >= LCD->TFT_HEIGHT)) 	return 0; 
	  return LCD->buffer16[x + y*LCD->TFT_WIDTH];
	}	
	
	
}

void uTFT_Invert(void) 
{
	#if defined(uSSD1306)
	  SSD1306_Invert();
	#endif
}

//Инициализация дисплея
void uTFT_Init(uTFT_LCD_t* LCD) 
{		
	switch (LCD->LCD_Driver)
	{
		case SSD1306 : SSD1306_Init(LCD);  break;	
		case ST7789  : ST7789_Init(LCD);   break;   // 1 - ST7789	
		case ILI9225 : ILI9225_init(LCD);  break;	 // 2 - ILI9225	
		case ST7735  : ST7735_init(LCD);   break;
		case ST7735S : ST7735S_init(LCD);  break;	
		default:
			break;
	}
	
}


void uTFT_Update(uTFT_LCD_t* LCD) 
{	
switch (LCD->LCD_Driver)
{
	//
	case SSD1306 : SSD1306_UpdateScreen(LCD); break;
	case ILI9225 : ILI9225_UpdateScreen(LCD); break; // 2 - ILI9225
	case ST7789  : ST7789_Update(LCD);        break; //1-ST7789
	case ST7735  : ST7735_Update(LCD);        break; //1-ST7789	
	case ST7735S : ST7735_Update(LCD);        break; //1-ST7789	
	case LCD_USB     : uTFT_USB_Update(LCD);      break;
	default:
		break;
}

	
	
}

//void uTFT_DrawFilledRectangle(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t c)
//{

//  #if defined(TFT9341)
//	if(LCD_Palette)
//	  TFT9341_FillRectangle_4B(x, y, x+w, y+h, c);
//	else
//    TFT9341_FillRectangle(c, x, y, x+w, y+h);
//	#else
//	  _uTFT_DrawFilledRectangle(x, y, w, h, c);
//	#endif
//	
//	#if defined(uST7789)
//	 _uTFT_DrawFilledRectangle(x, y, w, h, c);
//	#endif
//	
//}

//Подсветка 0..255
void uTFT_BackLight(uint8_t i)
{  
	#if defined(LCD_USE_BL)
	  //TIM9->CCR2 = i;
	#endif
	
}

void uTFT_Struct_to_Pallete_4b(uTFT_LCD_t * LCD, uTFT_LCD_Pallete * in)
{
    LCD->palete[0] = in->C0;
	  LCD->palete[1] = in->C1;
	  LCD->palete[2] = in->C2;
	  LCD->palete[3] = in->C3;
	  LCD->palete[4] = in->C4;
	  LCD->palete[5] = in->C5;
	  LCD->palete[6] = in->C6;
	  LCD->palete[7] = in->C7;
	  LCD->palete[8] = in->C8;
	  LCD->palete[9] = in->C9;
	  LCD->palete[10] = in->C10;
	  LCD->palete[11] = in->C11;
	  LCD->palete[12] = in->C12;
	  LCD->palete[13] = in->C13;	
	  LCD->palete[14] = in->C14;
	  LCD->palete[15] = in->C15;	
}

