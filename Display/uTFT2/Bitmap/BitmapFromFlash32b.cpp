#include "bitmap.h"

//32 бит BMP с альфа каналом Сохранять как инвертированая альфа и свап  , customAlpha = 1.0 полная альфа
void BitmapFromFlash32b(TFT * tft, int16_t x0, int16_t y0,	Bitmap *bmp) {
	uint32_t sColor;
	uint32_t dColor;

	uint32_t sR, sG, sB;
	uint32_t dR, dG, dB;
	uint32_t R, G, B;

	int32_t alpha;
	int32_t oneminusalpha;

	int _H = bmp->H + y0;
	int _W = bmp->W + x0;

	int32_t pX;
	int32_t pY;

	uint32_t deltaX;

	uint8_t *p8;
	p8 = (uint8_t *)&bmp->data[0];

	for ( pY = y0; pY < _H; pY++) {
		for ( pX = x0; pX < _W; pX++)
		{
			deltaX = pX + pY * 240;

			alpha = *p8++;
			sR    = *p8++;
			sG    = *p8++;
			sB    = *p8++;

			dColor = tft->LCD->buffer16[deltaX]; //GetPixel(x, y);

			dR = (dColor & 0xF800) >> 8;
			dG = (dColor & 0x7E0)  >> 3;
			dB = (dColor & 0x1F)   << 3;

			oneminusalpha = 255 - alpha;

			R = (uint8_t)(((sR * alpha) + (oneminusalpha * dR)) >> 11 );
			G = (uint8_t)(((sG * alpha) + (oneminusalpha * dG)) >> 10 );
			B = (uint8_t)(((sB * alpha) + (oneminusalpha * dB)) >> 11 );

			sColor = (R << 11) | (G << 5) | B; //tft->RGB565(R, G, B); //(R << 11) | (G << 5) | (B << 0); //
			tft->LCD->buffer16[deltaX] = sColor;
		}
	}
};
