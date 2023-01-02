/*
 * TFTWiget.h
 *
 *  Created on: 24 мар. 2022 г.
 *      Author: Ivan
 */

#ifndef TFT_WIGET_ANIMATED_RECTAGLE_H_
#define TFT_WIGET_ANIMATED_RECTAGLE_H_

#include "main.h"

#include "../Animated/EasingProvider.h"
#include "../TFT.h"

extern uint16_t palitra[];

class TFT_Animated_Rectagle {
public:

	void init_tft(TFT * _TFT)             { tft = _TFT;    }
    void setHW(uint16_t _H, uint16_t _W ) { H = _H; W =_W; }
    void setH (uint16_t _H)               { H = _H;        }
	void setW (uint16_t _W )              { W =_W;         }
	void setXY(uint16_t X, uint16_t Y )   { x = X; y = Y;  }
	void setX (uint16_t X)                { x = X;         }
	void setY (uint16_t Y )               { y = constrain(Y, 0, 240);}
	List_Update_Particle info(void)       { List_Update_Particle U;  U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W - 1; U.y1 = y + H - 1; return U; }
    void setEaseP(Ease e)                 { easeP = e; } //Выбор типа движения в анимации
    void setEaseN(Ease e)                 { easeN = e; } //Выбор типа движения в анимации

    uint8_t needUpdate(void)              {
    	if (_needUpdate)
    	{
    		_needUpdate = 0;
    		return 1;
    	}
    	return 0;};

    void run()
    {

       float temp;
       //char str[32];

       float elapsedTimeRate; //Состояние онимации от 0.0 до 1.0

    	if (P)
    	{
    	  temp = (float)( (float)( uwTick - animation_start_time_P ) / animation_max_time_P ); //Получаем время 0..1 анимации
    	  if (temp >= 1.0F) {P = 0; temp = 1.0F; } //Анимацию больше не использовать
    	  Gloabal_percent = temp;

    	  elapsedTimeRate = Easing.get(easeP, temp ); //Получаем коеффициент смещения 0..1 от времени c учетом типа анимации

    	  if (typeAnimation == 0){  tft->RectangleFilled(x, y, W*elapsedTimeRate, H, palitra[18]);  }

    	  if (typeAnimation == 1){
              for(int i = y; i< y+H; i++){
    		    tft->LineH(i, W/2-(elapsedTimeRate*W)/2,  W/2,  palitra[18]);}
    		  tft->RectangleFilled(W/2, y, (W*elapsedTimeRate)/2, H, palitra[18]);  }

    	  if (typeAnimation == 2){
              uint16_t col = tft->GetPixel(x, y);
              if (elapsedTimeRate >= 1.0F) elapsedTimeRate = 1.0F;
              if (elapsedTimeRate <= 0.0F) elapsedTimeRate = 0.0F;
              uint16_t newcolor = tft->alphaBlend( (uint8_t)(255.0F*elapsedTimeRate), palitra[18], col);
    		  tft->RectangleFilled(x, y, W, H, newcolor);  }

    	  _needUpdate = 1;
    	}


    	if (N)
    	{
    	  temp = (1.0F - Gloabal_percent) + (float)( (float)( uwTick - animation_start_time_N ) / animation_max_time_N ); //Получаем время 0..1 анимации
    	  //sprintf(str, "id:%d (-) run temp:%f Now:%f, animation_start_time_N:%f \n", id, temp, (float)Now, animation_start_time_N);
    	  //SEGGER_RTT_WriteString(0, str);

    	  if (temp >= 1.0F) {N = 0; temp = 1.0F; } //Анимацию больше не использовать
    	  temp = constrain(temp, 0.0F, 1.0F);
    	  elapsedTimeRate = Easing.get(easeN, (1.0F - temp) ); //Получаем коеффициент смещения 0..1 от времени c учетом типа анимации


    	  if (typeAnimation == 0)
    	  {
    		  tft->RectangleFilled(x, y, W*elapsedTimeRate, H, palitra[18]);
    	  }

    	  if (typeAnimation == 1)
    	  {
              for(int i = y; i< y+H; i++)
    		    tft->LineH(i, W/2-(elapsedTimeRate*W)/2,  W/2-1,  palitra[18]);

    		  tft->RectangleFilled(W/2, y, (W*elapsedTimeRate)/2, H, palitra[18]);
    	  }

    	  if (typeAnimation == 2)
    	  {
              uint16_t col = tft->GetPixel(x, y);
              if (elapsedTimeRate >= 1.0F) elapsedTimeRate = 1.0F;
              if (elapsedTimeRate <= 0.0F) elapsedTimeRate = 0.0F;

              uint16_t newcolor = tft->alphaBlend( (uint8_t)(255.0F*elapsedTimeRate), palitra[18], col);

    		  tft->RectangleFilled(x, y, W, H, newcolor);
    	  }

    	  _needUpdate = 1;
    	}


       if ((P == 0) && (_select == 1))
       {
	     tft->RectangleFilled(x, y, W, H, palitra[18]);

	     //if (_needUpdate)
	    	 _needUpdate = 1;
	     //else
	     //   _needUpdate = 0;
       }

      // if ((N == 0) && (_select == 0))
      //{_needUpdate = 1;}
	   //  tft->RectangleFilled(x, y, 0, H, BLUE);

    }





	void select(uint8_t s)
	{

		_select = s;

		//SEGGER_RTT_printf(0, "select= %d\n", s);

		if ((_select == 1) && (_select_last ==  0))
		{
		  P = 1;
		  N = 0;
		  animation_start_time_P = uwTick;
		  //SEGGER_RTT_printf(0, "id:%d onChangeP P1 N0 Now:%f \n", animation_start_time_P);
		  _select_last = s;
		  return;
		}

		if ((_select == 0) && (_select_last ==  1))
		{
           P = 0;
		   N = 1;
		   animation_start_time_N =  uwTick;
		   //SEGGER_RTT_printf(0, "id:%d onChangeN P0 N1 Now:%f \n", animation_start_time_N);
		}

		_select_last = s;

	}

  //https://tympanus.net/Development/CreativeButtons/
  uint8_t typeAnimation; //Тип анимации 0-слева линейное 1-из середины в 2 стороны 2-закраска квадрата

private:
	TFT * tft;

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

#endif /* TFT_WIGET_ANIMATED_RECTAGLE_H_ */
