#include "uTFT_define.h"

void uTFT_DrawCircle(uTFT_LCD_t* LCD, int16_t x0, int16_t y0, int16_t r, uint16_t c) {
	
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
 
    uTFT_SetPixel(LCD, x0, y0 + r, c);
    uTFT_SetPixel(LCD, x0, y0 - r, c);
    uTFT_SetPixel(LCD, x0 + r, y0, c);
    uTFT_SetPixel(LCD, x0 - r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        uTFT_SetPixel(LCD, x0 + x, y0 + y, c);
        uTFT_SetPixel(LCD, x0 - x, y0 + y, c);
        uTFT_SetPixel(LCD, x0 + x, y0 - y, c);
        uTFT_SetPixel(LCD, x0 - x, y0 - y, c);

        uTFT_SetPixel(LCD, x0 + y, y0 + x, c);
        uTFT_SetPixel(LCD, x0 - y, y0 + x, c);
        uTFT_SetPixel(LCD, x0 + y, y0 - x, c);
        uTFT_SetPixel(LCD, x0 - y, y0 - x, c);
    }
}

void uTFT_DrawFilledCircle(uTFT_LCD_t* LCD, int16_t x0, int16_t y0, int16_t r, uint16_t c) {
	
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

    uTFT_SetPixel(LCD, x0, y0 + r, c );
    uTFT_SetPixel(LCD, x0, y0 - r, c );
    uTFT_SetPixel(LCD, x0 + r, y0, c);
    uTFT_SetPixel(LCD, x0 - r, y0, c);
    uTFT_DrawLine(LCD, x0 - r, y0, x0 + r, y0, c);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        uTFT_DrawLine(LCD, x0 - x, y0 + y, x0 + x, y0 + y, c);
        uTFT_DrawLine(LCD, x0 + x, y0 - y, x0 - x, y0 - y, c);

        uTFT_DrawLine(LCD, x0 + y, y0 + x, x0 - y, y0 + x, c);
        uTFT_DrawLine(LCD, x0 + y, y0 - x, x0 - y, y0 - x, c);
    }
}
