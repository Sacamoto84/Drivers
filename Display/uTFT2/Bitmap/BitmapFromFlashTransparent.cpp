#include "bitmap.h"

void BitmapFromFlashTransparent(TFT * tft, uint16_t X, uint16_t Y,	Bitmap bmp, uint16_t TrColor) {

	if (bmp.bit == 1) {
		const uint8_t *p;
		p = (uint8_t *)bmp.data;

		uint8_t pX;
		uint8_t pY;
		uint8_t tmpCh;
		uint8_t bL;

		pY = Y;
		while (pY < Y + bmp.H) {
			pX = X;
			while (pX < X + bmp.W) {
				bL = 0;
				tmpCh = *p++;
				if (tmpCh) {
					while (bL < 8) {
						if (tmpCh & 0x01)
							tft->SetPixel(pX, pY + bL, 1);
						tmpCh >>= 1;
						if (tmpCh)
							bL++;
						else {
							pX++;
							break;
						}
					}
				} else {
					pX++;
				}
			}
			pY += 8;
		}

	}

	if (bmp.bit == 16) {
		const uint16_t *p16;
		p16 = bmp.steam16;

		uint16_t temp;
		for (uint16_t pY = Y; pY < bmp.H; pY++) {
			for (uint16_t pX = X; pX < bmp.W; pX++) {
				temp = *p16++;
				if (TrColor != temp)
				{
						tft->LCD->buffer16[pX + pY * tft->LCD->TFT_WIDTH] = temp;
				}

			}

		}
	}
}
