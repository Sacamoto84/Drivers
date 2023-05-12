#ifndef TFT_H_
#define TFT_H_

#include "TFT_define.h"
#include "uTFT_Buffer.h"

#include "TFT_config.h"

//#include "uTFT_weak.h"

#ifdef TFT_USE_SPI
  #include "spi.h"
  #include "TFT_SPI.h"
#endif


#include "Driver/TFT_Driver.h"

//#include "./Driver/ST7789.h"
//#include "uTFT_Color.h"

#define FontId0 (uint8_t *)(tft.getResAdressFontID(0))
#define FontId1 (uint8_t *)(tft.getResAdressFontID(1))
#define FontId2 (uint8_t *)(tft.getResAdressFontID(2))
#define FontId3 (uint8_t *)(tft.getResAdressFontID(3))

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

	TFT_Driver driver;

	int16_t _xPivot;   // TFT x pivot point coordinate for rotated Sprites
	int16_t _yPivot;   // TFT x pivot point coordinate for rotated Sprites

	////////////////////////////////////////////////////////////////////////////////////////////
	void init(uTFT_LCD_t *_LCD) {
		LCD = _LCD;
		driver.init(_LCD);
	}

//┌────────────────────────────────┬────────────────────┐
	void Fill(uint16_t color); //│                    │
	void Fill16(uint16_t color); //│ Только Для 16 бит  │
//└────────────────────────────────┴────────────────────┘

	void SetPixel(int32_t x, int32_t y, uint16_t color);
	void SetPixel16(int32_t x, int32_t y, uint16_t color);
	uint16_t GetPixel(int32_t x, int32_t y);
	void SetColorToPallete(uint8_t index, uint16_t color);


	void Invert(void) //Не работает
			{
        #if defined(uSSD1306)
		  //SSD1306_Invert();
		#endif
	}

	//Подсветка 0..255
	void BackLight(uint8_t i) {
        #ifdef LCD_USE_BL
		  //TIM9->CCR2 = i;
		#endif
	}

	//Установить цвет в ячейку палитры
	void setColorToPalete(int index, uint16_t c) {
		if (index > 255)
			return;
		LCD->palete[index] = c;
	}

	void Struct_to_Pallete_4b(uTFT_LCD_t *LCD, uTFT_LCD_Pallete *in) {
		LCD->palete[0] = in->C0; LCD->palete[1] = in->C1; LCD->palete[2] = in->C2; LCD->palete[3] = in->C3;
		LCD->palete[4] = in->C4; LCD->palete[5] = in->C5; LCD->palete[6] = in->C6; LCD->palete[7] = in->C7;
		LCD->palete[8] = in->C8; LCD->palete[9] = in->C9; LCD->palete[10] = in->C10; LCD->palete[11] = in->C11;
		LCD->palete[12] = in->C12; LCD->palete[13] = in->C13; LCD->palete[14] = in->C14; LCD->palete[15] = in->C15;
	}

	// ----- Line.cpp ----
	void Line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t c);
	void LineV(int32_t X, int32_t Y1, int32_t Y2, uint16_t color);
	void LineH(int32_t Y, int32_t X1, int32_t X2, uint16_t color);
	void LineH16(int32_t Y, int32_t X1, int32_t X2, uint16_t color);
	void LineHW(int32_t x, int32_t y, int32_t w, uint16_t color);
	void LineHW16(int32_t x, int32_t y, int32_t w, uint16_t color);
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
	void Circle(int16_t x0, int16_t y0, int16_t r, uint16_t c);
	void CircleFilled(int16_t x0, int16_t y0, int16_t r, uint16_t c);

	void Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
			uint16_t x3, uint16_t y3, uint16_t color);
	void TriangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
			int16_t x3, int16_t y3, uint16_t color);

	void Gradient_Vertical(uint32_t x0, uint32_t y0, uint32_t w, uint32_t h,
			uint8_t otR, uint8_t otG, uint8_t otB, uint8_t doR, uint8_t doG,
			uint8_t doB);

	//Работает на все фонты//////////////////////////////////////////////////////////////
	void SetFontColor(uint16_t Color, uint16_t BColor) {
		uTFT.Color = Color;
		uTFT.BColor = BColor;
	}
	void SetColor(uint16_t Color) {
		uTFT.Color = Color;
	}
	void SetBColor(uint16_t BColor) {
		uTFT.BColor = BColor;
	}
	void GotoXY(int16_t x, int16_t y) {
		uTFT.CurrentX = x;
		uTFT.CurrentY = y;
	}

//	//Альфа и транспарент
//	//Впихнуть изображение с другого спрайта
//	void pushSpriteIn(TFT *_tft, int16_t x, int16_t y) {
//		int32_t x1, x2, y1, y2;
//		x1 = constrain(x, 0, LCD->TFT_WIDTH);
//		x2 = constrain(x + _tft->LCD->TFT_WIDTH, 0, LCD->TFT_WIDTH);
//		y1 = constrain(y, 0, LCD->TFT_HEIGHT);
//		y2 = constrain(y + _tft->LCD->TFT_HEIGHT, 0, LCD->TFT_HEIGHT);
//
//		for (int32_t i = x1; i < x2; i++) {
//			for (int16_t ii = y1; ii < y2; ii++) {
//				SetPixel(i, ii, _tft->GetPixel(i - x, ii - y));
//			}
//		}
//	}
//
//	void pushSpriteInTr(TFT *_tft, int16_t x, int16_t y, uint16_t tr_color) {
//		int16_t x1, x2, y1, y2;
//		uint16_t color;
//		x1 = constrain(x, 0, LCD->TFT_WIDTH);
//		x2 = constrain(x + _tft->LCD->TFT_WIDTH, 0, LCD->TFT_WIDTH);
//		y1 = constrain(y, 0, LCD->TFT_HEIGHT);
//		y2 = constrain(y + _tft->LCD->TFT_HEIGHT, 0, LCD->TFT_HEIGHT);
//
//		for (int16_t i = x1; i < x2; i++) {
//			for (int16_t ii = y1; ii < y2; ii++) {
//				color = _tft->GetPixel(i - x, ii - y);
//				if ( color != tr_color)
//				  SetPixel(i, ii, color);
//			}
//		}
//	}

	uTFT_t uTFT; //Для установка положения и цвета для текста

	uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);

private:

};

#endif /* TFT_H_ */
