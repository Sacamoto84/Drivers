#include "bitmap.h"

void BitmapFromFlash16b(TFT * tft, int16_t X, int16_t Y, Bitmap *bmp) {
		const uint16_t *p16;
		p16 = (uint16_t *)bmp->data;
		int32_t pX;
		int32_t pY;
	    int _H = bmp->H + Y;
	    int _W = bmp->W + X;

		for ( pY = Y; pY < _H; pY++) {
			for ( pX = X; pX < _W; pX++)
			    tft->LCD->buffer16[pX + pY * tft->LCD->TFT_WIDTH] = *p16++;
		}
}
