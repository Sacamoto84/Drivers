#include "TFT.h"

// ----- Line ----
void TFT::Line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t c) {

	int16_t dx, dy, sx, sy, err, e2, i, tmp;

	/* Check for overflow */
	if (x0 >= LCD->TFT_WIDTH) {
		x0 = LCD->TFT_WIDTH - 1;
	}
	if (x1 >= LCD->TFT_WIDTH) {
		x1 = LCD->TFT_WIDTH - 1;
	}
	if (y0 >= LCD->TFT_HEIGHT) {
		y0 = LCD->TFT_HEIGHT - 1;
	}
	if (y1 >= LCD->TFT_HEIGHT) {
		y1 = LCD->TFT_HEIGHT - 1;
	}

	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			SetPixel(x0, i, c);
		}

		/* Return from function */
		return;
	}

	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			SetPixel(i, y0, c);
		}

		/* Return from function */
		return;
	}

	while (1) {
		SetPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

void TFT::LineV(int32_t X, int32_t Y1, int32_t Y2, uint16_t color) {

	if ((LCD->Bit) == 1) {
		int32_t i;

		if (color == 1) {
			for (i = Y1; i <= Y2; i++)
				LCD->buffer8[X + (i / 8) * LCD->TFT_WIDTH] |= 1 << (i % 8);
		} else {
			for (i = Y1; i <= Y2; i++)
				LCD->buffer8[X + (i / 8) * LCD->TFT_WIDTH] &= ~(1 << (i % 8));
		}
	}

	if ((LCD->Bit) == 4) {
		int32_t i;
		for (i = Y1; i <= Y2; i++) //uTFT_SetPixel(X, i, color);
				{
			SetPixel(X, i, color);
		}
	}

	if (LCD->Bit == 16) {
		if (Y2 >= LCD->TFT_HEIGHT)
			Y2 = LCD->TFT_HEIGHT - 1;
		if (X >= LCD->TFT_WIDTH)
			return;

		int32_t i;
		for (i = Y1; i <= Y2; i++) //uTFT_SetPixel(X, i, color);
				{
			LCD->buffer16[X + i * LCD->TFT_WIDTH] = color;
		}
	}
}

void TFT::LineH(int32_t Y, int32_t X1, int32_t X2, uint16_t color) {
	if (X2 >= LCD->TFT_WIDTH)
		X2 = LCD->TFT_WIDTH - 1;
	if (Y >= LCD->TFT_HEIGHT)
		return;

	if ((LCD->Bit) == 1) {
		int32_t i;

		if (color == 1) {
			for (i = X1; i <= X2; i++)
				LCD->buffer8[i + (Y / 8) * LCD->TFT_WIDTH] |= 1 << (Y % 8);
		} else {
			for (i = X1; i <= X2; i++)
				LCD->buffer8[i + (Y / 8) * LCD->TFT_WIDTH] &= ~(1 << (Y % 8));
		}
	}

	if ((LCD->Bit) == 16) {
		int32_t i;
		for (i = X1; i <= X2; i++) //uTFT_SetPixel(X, i, color);
				{
			LCD->buffer16[i + Y * LCD->TFT_WIDTH] = color;
		}
	}

	if ((LCD->Bit) == 4) {
		int32_t i;
		for (i = X1; i <= X2; i++) //uTFT_SetPixel(X, i, color);
				{
			if (i % 2 == 0) {
				LCD->buffer8[i / 2 + Y * ((LCD->TFT_WIDTH) / 2)] =
						(LCD->buffer8[i / 2 + Y * ((LCD->TFT_WIDTH) / 2)]
								& (0x0F)) | (color << 4);
			} else {
				LCD->buffer8[i / 2 + Y * ((LCD->TFT_WIDTH) / 2)] =
						(LCD->buffer8[i / 2 + Y * ((LCD->TFT_WIDTH) / 2)]
								& (0xF0)) | color;
			}
		}
	}

}

void TFT::LineHW(int32_t x, int32_t y, int32_t w, uint16_t color) {
	LineH(y, x, x + w - 1, color);
}

void TFT::LineMoveTo(int32_t x, int32_t y, uint16_t c) {
	Line(LineMoveX, LineMoveY, x, y, c);
	LineMoveX = x;
	LineMoveY = y;
}

void TFT::LineMoveXY(int32_t x, int32_t y) {
	LineMoveX = x;
	LineMoveY = y;
}

// ----- Rectagle ----
void TFT::Rectangle(int32_t x, int32_t y, uint16_t w, uint16_t h, uint16_t c) {
	/* Check input parameters */
	if (x >= LCD->TFT_WIDTH || y >= LCD->TFT_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= LCD->TFT_WIDTH) {
		w = LCD->TFT_WIDTH - x;
	}
	if ((y + h) >= LCD->TFT_HEIGHT) {
		h = LCD->TFT_HEIGHT - y;
	}

	/* Draw 4 lines */
	LineH(y, x, x + w, c);
	LineH(y + h, x, x + w, c);

	LineV(x, y, y + h, c);
	LineV(x + w, y, y + h, c);
}

void TFT::RectangleFilled(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
		uint16_t c) {
	uint32_t i;

	/* Check input parameters */
	if (x >= LCD->TFT_WIDTH || y >= LCD->TFT_HEIGHT) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= LCD->TFT_WIDTH) {
		w = LCD->TFT_WIDTH - x;
	}
	if ((y + h) >= LCD->TFT_HEIGHT) {
		h = LCD->TFT_HEIGHT - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		LineH(y + i, x, x + w, c); //124us
	}
}

//void uTFT_DrawRectangle(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t color)
//{
//  ST7735_HLine(X1,X2,Y1,color);
//  ST7735_HLine(X1,X2,Y2,color);
//  ST7735_VLine(X1,Y1,Y2,color);
//  ST7735_VLine(X2,Y1,Y2,color);
//}

//Инверсия цветов в данном прямоугольнике
void TFT::InvertRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	for (uint8_t i = x; i <= x + w; i++)
		for (uint16_t ii = y; ii <= y + h; ii++)
			SetPixel(i, ii, !GetPixel(i, ii));
}

//Замена цветов в данном прямоугольнике
void TFT::ChangeColorRectangle(int32_t x, int32_t y, uint32_t w, int32_t h,
		uint16_t sColor, uint16_t dColor) {
	for (uint16_t i = x; i <= x + w; i++)
		for (uint16_t ii = y; ii <= y + h; ii++) {
			if (GetPixel(i, ii) == sColor)
				SetPixel(i, ii, dColor);
		}
}

// ----- Круги ----
void TFT::Circle(int16_t x0, int16_t y0, int16_t r, uint16_t c) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SetPixel(x0, y0 + r, c);
	SetPixel(x0, y0 - r, c);
	SetPixel(x0 + r, y0, c);
	SetPixel(x0 - r, y0, c);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		SetPixel(x0 + x, y0 + y, c);
		SetPixel(x0 - x, y0 + y, c);
		SetPixel(x0 + x, y0 - y, c);
		SetPixel(x0 - x, y0 - y, c);

		SetPixel(x0 + y, y0 + x, c);
		SetPixel(x0 - y, y0 + x, c);
		SetPixel(x0 + y, y0 - x, c);
		SetPixel(x0 - y, y0 - x, c);
	}
}

void TFT::CircleFilled(int16_t x0, int16_t y0, int16_t r, uint16_t c) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SetPixel(x0, y0 + r, c);
	SetPixel(x0, y0 - r, c);
	SetPixel(x0 + r, y0, c);
	SetPixel(x0 - r, y0, c);
	Line(x0 - r, y0, x0 + r, y0, c);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		Line(x0 - x, y0 + y, x0 + x, y0 + y, c);
		Line(x0 + x, y0 - y, x0 - x, y0 - y, c);

		Line(x0 + y, y0 + x, x0 - y, y0 + x, c);
		Line(x0 + y, y0 - x, x0 - y, y0 - x, c);
	}
}

// ----- Треугольники ----

void TFT::Triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
		uint16_t x3, uint16_t y3, uint16_t color) {
	/* Draw lines */
	Line(x1, y1, x2, y2, color);
	Line(x2, y2, x3, y3, color);
	Line(x3, y3, x1, y1, color);
}

void TFT::TriangleFilled(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
		int16_t x3, int16_t y3, uint16_t color) {

	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, yinc1 =
			0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
			curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) {
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		Line(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}
