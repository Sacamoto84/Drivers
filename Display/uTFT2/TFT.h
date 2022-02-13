/*
 * TFT.h
 *
 *  Created on: 11 мая 2021 г.
 *      Author: Ivan
 */

#ifndef TFT_H_
#define TFT_H_

#include "./inc/uTFT_Buffer.h"
//#include "./inc/uTFT_weak.h"
#include "spi.h"

#include "TFT_SPI.h"

#include "./Driver/ST7789.h"
#include "./inc/uTFT_Color.h"

#include "./Font/Font.h"



#define ABS(x)   ((x) > 0 ? (x) : -(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


//////////////////////////////////////////////
//Макросы
//////////////////////////////////////////////

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define CREATE_SPRITE16(name, x , y) uint16_t name##_buffer16[x * y + 4];\
uTFT_LCD_t name={x , y, SPRITE_RAM, 16, NULL , NULL, &name##_buffer16[0],};

//Выделение памяти
//CREATE_SPRITE16(S1, 32 , 32);
//Sprite.init(&S1);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//////////////////////////////////////////////




class TFT {
public:

	uTFT_LCD_t *LCD;

	TFT_SPI SPI;

    uint32_t DMA_TX_Complete; //����� ������ ��������� DMA �������� ������ ��� �����

    uint32_t needUpdate;

    uint32_t blockUpdate;

	void init(uTFT_LCD_t *_LCD) {
		LCD = _LCD;

		_iwidth  = LCD->TFT_WIDTH;
		_iheight = LCD->TFT_HEIGHT;

		  _sx = 0;
		  _sy = 0;
		  _sw = _iwidth;
		  _sh = _iheight;

		  _scolor = BLACK;

		switch (LCD->LCD_Driver) {
		case SSD1306:
			SSD1306_Init();
			break;
		case ST7789:
			SPI.init(_LCD);
			ST7789_Init();
			break;   // 1 - ST7789
		case ILI9225:
			SPI.init(_LCD);
			ILI9225_init();
			break;	 // 2 - ILI9225
		case ST7735:
			ST7735_init();
			break;
		case ST7735S:
			ST7735S_init();
			break;
		default:
			break;
		}
	}

	void Update(void) {
		switch (LCD->LCD_Driver) {
		case SSD1306:
			SSD1306_UpdateScreen();
			break;
		case ILI9225:
			ILI9225_UpdateScreen();
			break; // 2 - ILI9225
		case ST7789:
			ST7789_Update();
			break; //1-ST7789
		case ST7735:
			ST7735_Update();
			break; //1-ST7789
		case ST7735S:
			ST7735_Update();
			break; //1-ST7789
		case LCD_USB:
			uTFT_USB_Update();
			break;
		default:
			break;
		}
	}
	;

	void Fill(uint16_t color);

	void SetPixel(int32_t x, int32_t y, uint16_t color);
	uint16_t GetPixel(int32_t x, int32_t y);
	void SetColorToPallete(uint8_t index, uint16_t color);

	__weak void ST7789_Init(void);
	__weak void ST7789_Update(void);
	void ST7789_Update_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void ST7789_AddrSet(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	// Поворот экрана
	void ST77XX_Rotate(uint16_t depth); // 0 90 180 270
	//Обновление регистра MADCTL
	void ST77XX_Update_MADCTL(void);

	void ST7789_UpdateDMA4bit(void);
	void ST7789_UpdateDMA4bitV2(void);
	void ST7789_UpdateDMA8bitV2(void);
	void ST7789_UpdateDMA16bitV2(void);
	void ST7789_UpdateDMA16bitV3(void);
	void ST7789_Transmit_Array(char dc, uint8_t *data, int nbytes);

	void ILI9225_init_table16(uint16_t *table, int16_t size);
	void ILI9225_setWindow   ( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);




	__weak void SSD1306_Init(void);
	void SSD1306_WRITECOMMAND(uint8_t data);
	void SSD1306_WRITEDATA(uint8_t data);
	__weak void SSD1306_UpdateScreen(void);
	void SSD1306_Contrast(uint8_t c);


	uint16_t RGB565(uint8_t R, uint8_t G, uint8_t B) {
		return ((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3);
	}

	uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
		return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
	}

	void Invert(void) //Не работает
			{
#if defined(uSSD1306)
		  //SSD1306_Invert();
		#endif
	}

	//Подсветка 0..255
	void BackLight(uint8_t i) {
        #if defined(LCD_USE_BL)
		  //TIM9->CCR2 = i;
		#endif
	}

	void Struct_to_Pallete_4b(uTFT_LCD_t *LCD, uTFT_LCD_Pallete *in) {
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

	// ----- Line.cpp ----
	void Line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t c);
	void LineV(int32_t X, int32_t Y1, int32_t Y2, uint16_t color);
	void LineH(int32_t Y, int32_t X1, int32_t X2, uint16_t color);
	void LineHW(int32_t x, int32_t y, int32_t w, uint16_t color);
	void LineMoveTo(int32_t x, int32_t y, uint16_t c);

    int16_t LineMoveX;
	int16_t LineMoveY;

	void LineMoveXY(int32_t x, int32_t y);

	// ----- Rectagle.cpp ----
	void Rectangle(int32_t x, int32_t y, uint16_t w, uint16_t h, uint16_t c);
	void RectangleFilled(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
			uint16_t c);
	void InvertRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
	//Замена цветов в данном прямоугольнике
	void ChangeColorRectangle(int32_t x, int32_t y, uint32_t w, int32_t h,
			uint16_t sColor, uint16_t dColor);

	// ----- Circle.cpp ----
	void Circle      (int16_t x0, int16_t y0, int16_t r, uint16_t c);
	void CircleFilled(int16_t x0, int16_t y0, int16_t r, uint16_t c);

	void Triangle      (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);
	void TriangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2,	int16_t x3, int16_t y3, uint16_t color);


	void Gradient_Vertical(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t otR, uint8_t otG ,uint8_t otB, uint8_t doR, uint8_t doG, uint8_t doB);


	//Работает на все фонты//////////////////////////////////////////////////////////////
	void SetFontColor(uint16_t Color, uint16_t BColor) {		uTFT.Color = Color;		uTFT.BColor = BColor;	}
	void SetColor(uint16_t Color) {		uTFT.Color = Color;	}
	void SetBColor(uint16_t BColor) {		uTFT.BColor = BColor;	}
	void GotoXY(int16_t x, int16_t y) {		uTFT.CurrentX = x;		uTFT.CurrentY = y;	}
	// ----- Font Classic ----

	char Font_Classic_Putc(char ch, FontDef_t *Font, uint8_t NoBack = 0);
	char Font_Classic_Puts(char *str, FontDef_t *Font, uint8_t NoBack = 0);

	char* Font_Classic_GetStringSize(char *str, FONTS_SIZE_t *SizeStruct, FontDef_t *Font);

	// ----- Font Micro ----
	FontDefMicroElectronika_t Font;

	void Font_Micro_SetFont(FontDefMicroElectronika_t *uFont);// Устновна фонта
	void Font_Micro_Putc(uint8_t ch, uint8_t dx, uint8_t transparrent);	// Вывод символа
	void Font_Micro_Puts(char *str, uint8_t dx, uint8_t transparrent);
	//Определение длинны строки в пикселях по типу текущего шрифта
	uint16_t Font_Micro_Find_Len_Str(char *str,
			FontDefMicroElectronika_t *uFont);


	// ----- Font Smooth ----
	//Тест вывод на экран всего алфавита
	void Font_Smooth_showFont(uint32_t td);
	void Font_Smooth_drawGlyph(uint16_t code);
	void Font_Smooth_Load(const unsigned char *massiv);
	void Font_Smooth_drawStr(const char *str);
	void Font_Smooth_drawStr(int x, int y, const char *str);
	void Font_Smooth_drawStr(int x, int y, const char *str, uint16_t color);
	// ----- Font u8g ----











	//Альфа и транспарент
	//Впихнуть изображение с другого спрайта
	void pushSpriteIn(TFT *_tft, int16_t x, int16_t y) {
		int32_t x1, x2, y1, y2;
		x1 = constrain(x, 0, LCD->TFT_WIDTH);
		x2 = constrain(x + _tft->LCD->TFT_WIDTH, 0, LCD->TFT_WIDTH);
		y1 = constrain(y, 0, LCD->TFT_HEIGHT);
		y2 = constrain(y + _tft->LCD->TFT_HEIGHT, 0, LCD->TFT_HEIGHT);

		for (int32_t i = x1; i < x2; i++) {
			for (int16_t ii = y1; ii < y2; ii++) {
				SetPixel(i, ii, _tft->GetPixel(i - x, ii - y));
			}
		}
	}

	void pushSpriteInTr(TFT *_tft, int16_t x, int16_t y, uint16_t tr_color) {
		int16_t x1, x2, y1, y2;
		uint16_t color;
		x1 = constrain(x, 0, LCD->TFT_WIDTH);
		x2 = constrain(x + _tft->LCD->TFT_WIDTH, 0, LCD->TFT_WIDTH);
		y1 = constrain(y, 0, LCD->TFT_HEIGHT);
		y2 = constrain(y + _tft->LCD->TFT_HEIGHT, 0, LCD->TFT_HEIGHT);

		for (int16_t i = x1; i < x2; i++) {
			for (int16_t ii = y1; ii < y2; ii++) {
				color = _tft->GetPixel(i - x, ii - y);
				if ( color != tr_color)
				  SetPixel(i, ii, color);
			}
		}
	}

	void UpdateWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
		switch (LCD->LCD_Driver) {

		case ST7789:
			ST7789_Update_Window(x1, y1, x2, y2);
			break; //1-ST7789
		default:
			break;
		}
	}
	;


	//Картинки
    #define FP_SCALE 10
	int32_t  _sinra;
	int32_t  _cosra;

	int16_t  _xPivot;   // TFT x pivot point coordinate for rotated Sprites
	int16_t  _yPivot;   // TFT x pivot point coordinate for rotated Sprites

	int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1

	int32_t  _sx, _sy; // x,y for scroll zone
    uint32_t _sw, _sh; // w,h for scroll zone
    uint32_t _scolor;  // gap fill colour for scroll zone

	int32_t  _iwidth, _iheight; // Sprite memory image bit width and height (swapped during rotations)

	/***************************************************************************************
	** Function name:           setPivot
	** Description:             Set the pivot point on the TFT
	*************************************************************************************x*/
	void setPivot(int16_t x, int16_t y)	{  _xPivot = x;	  _yPivot = y;	}
	bool pushRotated(TFT * TFT_dest, int16_t angle, int32_t transp);
	bool getRotatedBounds(TFT * TFT_dest, int16_t angle, int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y);
	void getRotatedBounds(int16_t angle, int16_t w, int16_t h, int16_t xp, int16_t yp, int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y);
	void setScrollRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color);
	void scroll(int16_t dx, int16_t dy);







	void Bitmap_From_Flash(int16_t X, int16_t Y, Bitmap *bmp) ;
	void Bitmap_From_Flash_Tr(uint16_t X, uint16_t Y, Bitmap bmp, uint16_t TrColor);

	//32 бит BMP с альфа каналом Сохранять как инвертированая альфа и свап  , customAlpha = 1.0 полная альфа
	void Bitmap_From_Flash_Alpha(int32_t x0, int32_t y0, Bitmap *bmp, float customAlpha);

	void Bitmap_From_Flash_Background_16bit(Bitmap *bmp);



	void BMP_From_File(int32_t x0, int32_t y0, char * Name);

	void BMP_From_File_Tr(int32_t x0, int32_t y0, char * Name, uint16_t tr_color);

	//32 ��� BMP � ����� �������
	void BMP_From_File_Alpha(int32_t x0, int32_t y0, char * Name, int offset);

	/***************************************************************************************
	** __weak
	*************************************************************************************x*/
	__weak void ST7735S_init();

	__weak void ST7735_init();
	__weak void ST7735_Update();

	__weak void ILI9225_init();
	__weak void ILI9225_UpdateScreen();

	__weak void uTFT_USB_Update();




	uTFT_t uTFT; //Для установка положения и цвета для текста

private:




};








#endif /* TFT_H_ */
