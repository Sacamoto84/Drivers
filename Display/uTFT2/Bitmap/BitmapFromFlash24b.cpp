#include "bitmap.h"

void BitmapFromFlash24b(TFT * tft, int16_t X, int16_t Y, Bitmap *bmp) {

	    //rtt.println("Bitmap_From_Flash_24b");

		const uint8_t *p8;
		p8 = (uint8_t *)bmp->data;
		int32_t pX;
		int32_t pY;
	    int _H = bmp->H + Y;
	    int _W = bmp->W + X;

	    uint8_t A, HI;
		uint16_t Color;
		uint16_t dColor;
		uint32_t delta;

		for ( pY = Y; pY < _H; pY++) {
			for ( pX = X; pX < _W; pX++)
			{
				A = *p8++;
			    HI = *p8++;
			    Color = HI | ( *p8++ << 8 );
			    delta = pX + pY * 240;
			    dColor = tft->LCD->buffer16[delta];
			    tft->LCD->buffer16[delta] = tft->alphaBlend(A, Color, dColor);
			}
		}

}
