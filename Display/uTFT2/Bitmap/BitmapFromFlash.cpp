#include "bitmap.h"

void BitmapFromFlash(TFT * tft, int16_t X, int16_t Y, Bitmap *bmp)
{
	if (bmp->bit == 1) {

			uint8_t pX;
			uint8_t pY;
			uint8_t tmpCh;
			uint8_t bL;
			const uint8_t *p;

			p = (uint8_t *)bmp->data;

			pY = Y;
			while (pY < Y + bmp->H) {
				pX = X;
				while (pX < X + bmp->W) {
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

		if (bmp->bit == 16) {
			const uint16_t *p16;
			p16 = bmp->steam16;

			for (int16_t pY = Y; pY < bmp->H + Y; pY++) {
				for (int16_t pX = X; pX < bmp->W + X; pX++)
					tft->SetPixel(pX, pY, *p16++);
			}
		}
}

