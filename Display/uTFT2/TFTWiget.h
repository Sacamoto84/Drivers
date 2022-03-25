/*
 * TFTWiget.h
 *
 *  Created on: 24 мар. 2022 г.
 *      Author: Ivan
 */

#ifndef TFTWIGET_H_
#define TFTWIGET_H_

#include "main.h"
#include "TFT.h"
#include "EasingProvider.h"
#include "LiveData.h"
#include "SEGGER.h"

class TFT_Wiget_Animated_Button {
public:

	void init_tft(TFT * _TFT)             { tft = _TFT;    }
    void setHW(uint16_t _H, uint16_t _W ) { H = _H; W =_W;  U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W -1 ; U.y1 = y + H - 1;}
    void setH (uint16_t _H)               { H = _H;         U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W -1 ; U.y1 = y + H - 1;}
	void setW (uint16_t _W )              { W =_W;          U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W -1 ; U.y1 = y + H - 1;}
	void setXY(uint16_t X, uint16_t Y )   { x = X; y = Y;   U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W -1 ; U.y1 = y + H - 1;}
	void setX (uint16_t X)                { x = X;          U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W -1 ; U.y1 = y + H - 1;}
	void setY (uint16_t Y )               { y = constrain(Y, 0, 240);           U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W -1 ; U.y1 = y + H - 1;}
	List_Update_Particle info(void)       { U.H = H; U.W = W; U.x0 = x; U.y0 = y; U.x1 = x + W; U.y1 = y + H; return U; }
    void setEaseP(Ease e)                  { easeP = e; } //Выбор типа движения в анимации
    void setEaseN(Ease e)                  { easeN = e; } //Выбор типа движения в анимации
    uint8_t needUpdate(void)              {
    	if (_needUpdate)
    	{
    		_needUpdate =0;
    		return 1;
    	}
    	return 0;};

    void run()
    {

       float temp;
       char str[32];


    	if (P)
    	{
    	  temp = (float)( (float)( Now - animation_start_time_P ) / animation_max_time_N ); //Получаем время 0..1 анимации
    	  if (temp >= 1.0F) {P = 0; temp = 1.0F; } //Анимацию больше не использовать
    	  elapsedTimeRate = Easing.get(easeP, temp ); //Получаем коеффициент смещения 0..1 от времени c учетом типа анимации
    	  tft->RectangleFilled(x, y, W*elapsedTimeRate, H, palitra[18]);
         //sprintf(str, "run P:%d N:%d eT:%d t:%d\n", P, N, (int)(elapsedTimeRate*1000), (int)(temp*1000));
         //SEGGER_RTT_WriteString(0, str);
    	  _needUpdate = 1;
    	}


    	if (N)
    	{
    	  temp = (float)( (float)( Now - animation_start_time_N ) / animation_max_time_P ); //Получаем время 0..1 анимации
    	  if (temp <= 0.0F) {N = 0; temp = 0.0F; } //Анимацию больше не использовать
    	  temp = constrain(temp, 0.0F, 1.0F);
    	  elapsedTimeRate = Easing.get(easeN, (1.0F - temp) ); //Получаем коеффициент смещения 0..1 от времени c учетом типа анимации
    	  tft->RectangleFilled(x, y, W*elapsedTimeRate, H, palitra[18]);
          sprintf(str, "run P:%d N:%d eT:%d t:%d\n", P, N, (int)(elapsedTimeRate*1000), (int)(temp*1000));
          SEGGER_RTT_WriteString(0, str);
    	  _needUpdate = 1;
    	}


       if ((P == 0) && (_select))
       {
	     tft->RectangleFilled(x, y, W, H, palitra[18]);
	     _needUpdate = 1;
       }
       //if ((N == 0) && (_select == 0))
	   //tft->RectangleFilled(x, y, 1, H, BLUE);






    }





	void select(uint8_t s)
	{

		_select = s;

		//SEGGER_RTT_printf(0, "select= %d\n", s);

		if ((_select == 1) && (_select_last ==  0))
		{
		  P = 1;
		  N = 0;
		  animation_start_time_P =  Now;
		  SEGGER_RTT_printf(0, "onChangeP P1 N0 Time:%ld \n", animation_start_time_P);
		}

		if ((_select == 0) && (_select_last ==  1))
		{
           P = 0;
		   N = 1;
		   animation_start_time_N =  Now;
		   SEGGER_RTT_printf(0, "onChangeN P0 N1 Time:%ld \n", animation_start_time_N);
		}

		_select_last = s;

	}



private:
	TFT * tft;
	List_Update_Particle U;

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
	int8_t N = 0; //Akfu njuj xnj jnhbwfntkmysq ahjyn


    uint32_t animation_start_time_P; //Время старта анимации
    uint32_t animation_start_time_N; //Время старта анимации
    float    animation_max_time_P =  200.0F; //Длинна анимации в ms
    float    animation_max_time_N =  200.0F; //Длинна анимации в ms
    float    elapsedTimeRate; //Состояние онимации от 0.0 до 1.0



};

#endif /* TFTWIGET_H_ */
