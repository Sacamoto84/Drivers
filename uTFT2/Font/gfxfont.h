// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

#include "main.h"
#include "TFT.h"

/// Font data stored PER GLYPH
typedef struct {
	uint16_t bitmapOffset; ///< Pointer into GFXfont->bitmap
	uint8_t width;         ///< Bitmap dimensions in pixels
	uint8_t height;        ///< Bitmap dimensions in pixels
	uint8_t xAdvance;      ///< Distance to advance cursor (x axis)
	int8_t xOffset;        ///< X dist from cursor pos to UL corner
	int8_t yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
	uint8_t *bitmap;  ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;  ///< Glyph array
	uint16_t first;   ///< ASCII extents (first char)
	uint16_t last;    ///< ASCII extents (last char)
	uint8_t yAdvance; ///< Newline distance (y axis)
} GFXfont;

class GFXFONT {
public:

	TFT *_tft;
	GFXfont *gfxFont;

	void set_delta_x(int8_t x) {delta_x = x;}

	void init(TFT *tft) {
		_tft = tft;
	}

	void setFont(const GFXfont *f);
	void drawChar(unsigned char c, uint8_t size = 1);

	void Puts(char* str) {
		while (*str) {
			drawChar(*str);
			str++;

		}
	}

private:
	inline GFXglyph* pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c) {
		return gfxFont->glyph + c;
	}

	inline uint8_t* pgm_read_bitmap_ptr(const GFXfont *gfxFont) {
		return gfxFont->bitmap;
	}

	int8_t delta_x = 0; //Добавочное растояние между символами

};

/**************************************************************************/
/*!
 @brief Set the font to display when print()ing, either custom or default
 @param  f  The GFXfont object, if NULL use built in 6x8 font
 */
/**************************************************************************/
void GFXFONT::setFont(const GFXfont *f) {
	if (f) {          // Font struct pointer passed in?
		if (!gfxFont) { // And no current font struct?
			// Switching from classic to new font behavior.
			// Move cursor pos down 6 pixels so it's on baseline.
			//cursor_y += 6;
		}
	} else if (gfxFont) { // NULL passed.  Current font struct defined?
		// Switching from new to classic font behavior.
		// Move cursor pos up 6 pixels so it's at top-left of char.
		//cursor_y -= 6;
	}
	gfxFont = (GFXfont*) f;
}

// Draw a character
/**************************************************************************/
/*!
 @brief   Draw a single character
 @param    x   Bottom left corner x coordinate
 @param    y   Bottom left corner y coordinate
 @param    c   The 8-bit font-indexed character (likely ascii)
 @param    color 16-bit 5-6-5 Color to draw chraracter with
 @param    bg 16-bit 5-6-5 Color to fill background with (if same as color,
 no background)
 @param    size  Font magnification level in X-axis, 1 is 'original' size
 */
/**************************************************************************/
void GFXFONT::drawChar(unsigned char c, uint8_t size) {

	int16_t x      = _tft->uTFT.CurrentX;
	int16_t y      = _tft->uTFT.CurrentY;
	uint16_t color = _tft->uTFT.Color;

	c -= gfxFont->first;

	GFXglyph *glyph = pgm_read_glyph_ptr(gfxFont, c);
	uint8_t *bitmap = pgm_read_bitmap_ptr(gfxFont);

	uint16_t bo = glyph->bitmapOffset;

	uint8_t w = glyph->width, h = glyph->height;
	int8_t xo = glyph->xOffset, yo = glyph->yOffset;

	uint8_t xx, yy, bits = 0, bit = 0;
	int16_t xo16 = 0, yo16 = 0;

	if (size > 1) {	xo16 = xo;	yo16 = yo;	}

	for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++) {
			if (!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if (bits & 0x80) {
				if (size == 1) {
					_tft->SetPixel(x + xo + xx, y + yo + yy, color);
				} else {

		            _tft->RectangleFilled(x + (xo16 + xx) * size,	y + (yo16 + yy) * size, size, size, color);
				}
			}
			bits <<= 1;
		}
	}

	//_tft->uTFT.CurrentX += glyph->width + delta_x;
	_tft->uTFT.CurrentX += glyph->xAdvance + delta_x;// +glyph->xOffset + delta_x;

}





#endif // _GFXFONT_H_
