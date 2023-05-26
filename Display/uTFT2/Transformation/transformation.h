/*
 * transformation.h
 *
 *  Created on: 1 янв. 2023 г.
 *      Author: Ivan
 */

#ifndef TRANSFORMATION_TRANSFORMATION_H_
#define TRANSFORMATION_TRANSFORMATION_H_

#include "TFT.h"

class transformation {
public:

	transformation(TFT * _tft) {
		tft =_tft;

		_iwidth  = tft->LCD->TFT_WIDTH;
		_iheight = tft->LCD->TFT_HEIGHT;

		_sx = 0;
		_sy = 0;
		_sw = _iwidth;
		_sh = _iheight;

		_scolor = 0;
	}

	bool pushRotated(TFT * tft ,TFT * tft_src, int16_t angle, int32_t transp);
	void setScrollRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color);
	void scroll(int16_t dx, int16_t dy);

	bool getRotatedBounds(TFT * TFT_src, int16_t angle, int16_t *min_x,
			int16_t *min_y, int16_t *max_x, int16_t *max_y);

	void getRotatedBounds(int16_t angle, int16_t w, int16_t h, int16_t xp,
			int16_t yp, int16_t *min_x, int16_t *min_y, int16_t *max_x,
			int16_t *max_y);
private:
	TFT * tft;

	#define FP_SCALE 10
    int32_t  _sinra;
    int32_t  _cosra;

	int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1

	int32_t  _sx, _sy; // x,y for scroll zone
	uint32_t _sw, _sh; // w,h for scroll zone
	uint32_t _scolor;  // gap fill colour for scroll zone

	int32_t  _iwidth, _iheight; // Sprite memory image bit width and height (swapped during rotations)
};





#endif /* TRANSFORMATION_TRANSFORMATION_H_ */
