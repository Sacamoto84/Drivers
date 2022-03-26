/*

 //Все гиф находятся в папке /Gif/%s/%d.bmp
 //Имя папки не более 8 символов
 

 
 TFT_gif gif;
 gif.init(&tft);
 gif.setName("BT");
 gif.setMaxIndex(44);
 gif.setDelay(0);
 gif.useAplpha(1);
 gif.setSwap(0);

 TFT_gif gif2;
 gif2.init(&tft);
 gif2.setName("Q1");
 gif2.setMaxIndex(23);
 gif2.setDelay(0);
 gif2.useAplpha(1);
 gif2.setSwap(0);
 gif2.SetXY(120, 120);
 gif2.setOffset(4*21);


 gif.run(); Обработчик пеперисовки

 tft.ST7789_Update(gif.info());

 */

#ifndef TFT_GIF_H_
#define TFT_GIF_H_

#include "main.h"
#include "TFT.h"

//https://lordicon.com/

enum ANIMATION_TRIGGERS {
	HOVER,      //Запуск без повторения застываем на последнем кадре
	LOOP,       //Запуск с повторнением
	MORPH,      //Запуск вперед пока есть поздействие и возврат
	BOOMERANG   //Запуск приводит к движению в перед и назад
};

enum ANIMATION_STATE {
	sSTOP, sPLAY, //Состояние того вижемся
	sEND,  //Дошли до конца анимации
};

enum ANIMATION_COMMAND {
	cSTOP,    //Отобразить первый кадр
	cPLAY,    //Начать анимацию
	cPAUSE,   //Остановить на текущем кадре
	cENABLE,  //Показать обьект
	cDISABLE  //Скрыть обьект

};

class TFT_gif {
public:

	void init(TFT *_TFT) {
		tft = _TFT;
	}

	void command(ANIMATION_COMMAND c) {

		switch (c) {

		case cPLAY:
			index_current = 0;
			state_animation = sPLAY;
			vector = 0;
			break;

		case cSTOP:
			state_animation = sSTOP;
			vector = 0;
			break;

		default:
			break;
		}

	}
	;

	void run(void) {

		if (enabled) {

			calculate();

		}

	}

	void useAplpha(uint8_t i) {
		use_alpha = i;
	}
	void setOffset(int i) {
		offset = i;
	}
	void setName(char *name) {
		sprintf(name_gif, "%s", name);
	}
	void setDelay(int i) {
		delay = i;
	}
	void SetXY(int X, int Y) {
		x = X;
		y = Y;
	}
	void SetX(int X) {
		x = X;
	}
	void SetY(int Y) {
		y = Y;
	}
	void setMaxIndex(int i) {
		index_max = i;
	}
	void setSwap(int i) {
		swap = i;
	}

	List_Update_Particle info(void) {
		return U;
	} //Информация используемая для того чтобы отрисовать кадр

	ANIMATION_TRIGGERS trigger = HOVER; //Выбор типа анимации
	ANIMATION_STATE state_animation = sSTOP;

private:
	TFT *tft;
	ANIMATION_COMMAND com;
	bool enabled = true;

	bool needUpdate;

	uint8_t vector = 0; //направление анимации 0- прямое 1- обратное


	uint16_t delay = 100; //Задержка
	uint8_t use_alpha;
	float alpha = 1.0F; //Уровень альфы
	int16_t x = 0;
	int16_t y = 0;

	int8_t index_max;                //Максимальный индекс
	int8_t index_current = 0;        //Текущий индекс
	char name_gif[8];                //Название Gif папки

	uint32_t start_time; //Записываем время начала проприсовки анимации

	uint8_t swap;
	int offset = 0;

	List_Update_Particle U;

	//Открыть картирку по индексу
	void openBMPfromIndex(uint8_t i) {
		char current_patch[32]; //Полный путь к файлу

		sprintf(current_patch, "/Gif/%s/%d.bmp", name_gif, i); //Собираем полный путь в файлу

		if (use_alpha)
			U = (tft->BMP_From_File_Alpha(x, y, current_patch, offset, swap));
		else
			tft->BMP_From_File(x, y, current_patch);

	}

	//Открыть картирку по индексу
	void calculate(void) {

		//Расчет следующего интекса с учетом времени кадра
		if ((uwTick - start_time) > delay) {

			if (trigger == MORPH) {

				if (state_animation == sSTOP) {

					openBMPfromIndex(index_current--);
					if (index_current < 0)
						index_current = 0;
				}

				if (state_animation == sPLAY) {



						openBMPfromIndex(index_current++);

						if (index_current > index_max) {
							index_current = index_max;
						}


				}

			}

			if (trigger == BOOMERANG) {

				if (state_animation == sSTOP) {

					openBMPfromIndex(0);
				}

				if (state_animation == sPLAY) {

					if (vector == 0) {
						openBMPfromIndex(index_current++);
						if (index_current > index_max) {
							vector = 1;
							index_current = index_max - 1;
						}

					} else {

						openBMPfromIndex(index_current--);

						if (index_current < 0) {
							vector = 0;
							state_animation = sSTOP;
						}

					}

				}

			}

			if (trigger == LOOP) {

				if (state_animation == sPLAY) {
					openBMPfromIndex(index_current);
					index_current++;
					if (index_current > index_max)
						index_current = 0;
				}

				if (state_animation == sSTOP) {

					openBMPfromIndex(0);
				}

			}

			//Один раз воспроизвести и остановиться на последнем кадре
			if (trigger == HOVER) {

				if (state_animation == sSTOP) {

					openBMPfromIndex(index_max);
				}

				if (state_animation == sPLAY) {
					openBMPfromIndex(index_current);
					index_current++;
					if (index_current > index_max) {
						index_current = index_max;
						state_animation = sSTOP;
					}
				}
			}

		}

		start_time = uwTick; //Запомнили начало

	}

};

#endif /* TFT_GIF_H_ */
