#ifndef TFT_WIGET_ANIMATED_ICON_H_
#define TFT_WIGET_ANIMATED_ICON_H_

#include "main.h"
#include "TFT.h"
#include "EasingProvider.h"
#include "SEGGER.h"

class TFT_Wiget_Animated_Icon {








	void init_tft(TFT * _TFT)             { tft = _TFT;    }
















	    TFT *   tft;
	    TFT_gif gif;




		uint16_t W = 1;    //Ширина кнопки
		uint16_t H = 1;    //Высота кнопки
		uint16_t bc_color; //Цвет фона
		uint16_t x = 0;
		uint16_t y = 0;

		EasingProvider Easing;
		Ease easeP = BOUNCE_OUT; //Выбор типа анимации
		Ease easeN = QUAD_IN_OUT;

		uint8_t _select = 0;
		uint8_t _select_last = 0;

	    uint8_t _needUpdate; //Флаг того что нужно обновить обьект

		int8_t P = 0; //Флаг того что положительный фронт
		int8_t N = 0; //

		uint32_t animation_start_time_P; //Время старта анимации
		uint32_t animation_start_time_N; //Время старта анимации
	    float    animation_max_time_P =  500.0F; //Длинна анимации в ms
	    float    animation_max_time_N =  500.0F; //Длинна анимации в ms
	    float    Gloabal_percent;



};

#endif /* TFT_WIGET_ANIMATED_ICON_H_ */
