#include "bitmap.h"

// 756us 216MHz
void BitmapFromFlashBackground16bit(TFT * tft, Bitmap *bmp) {

	const uint32_t *p16;
	p16 = (uint32_t *)bmp->steam16;

    uint32_t *p;
	p = (uint32_t *)&tft->LCD->buffer16[0];

	uint32_t count = tft->LCD->TFT_HEIGHT * tft->LCD->TFT_WIDTH / 2 / 4 / 4 / 2;

	while(count--)
	{
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
	}

}

