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

#include "fatfs.h"

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
	cREPLAY,
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
			if (state_animation != sPLAY) {
				index_current = 0;
				state_animation = sPLAY;
				field.vector = 0;
			}
			break;

		case cREPLAY:
			index_current = 0;
			state_animation = sPLAY;
			field.vector = 0;
			break;

		case cSTOP:
			state_animation = sSTOP;
			field.vector = 0;
			break;

		default:
			break;
		}

	}
	;

	void run(void) {

		//if (field.enabled) {

		calculate();

		//}

	}

	void setOffset(int i) {
		offset = i;
	}

	void setName(char *name) {
		UINT bytesread;

		sprintf(name_gif, "%s", name);

		//Читаем настройки
		char current_patch[32]; //Полный путь к файлу
		sprintf(current_patch, "/Gif/%s/i.txt", name_gif); //Собираем полный путь в файлу
		int res = f_open(&SDFile, current_patch, FA_READ);
		char BMP_From_File_buf[64];

		if (res == FR_OK) {
			f_gets(BMP_From_File_buf, 16, &SDFile);
			H = atoi(BMP_From_File_buf);
			f_gets(BMP_From_File_buf, 16, &SDFile);
			W = atoi(BMP_From_File_buf);

			f_gets(BMP_From_File_buf, 16, &SDFile);

			if (atoi(BMP_From_File_buf) == 32)
				field.bit32 = 1;
			else
				field.bit32 = 1;

			f_gets(BMP_From_File_buf, 16, &SDFile);
			index_max = atoi(BMP_From_File_buf) - 1;

			SEGGER_RTT_printf(0,
					"(+) GIF name: %s  H: %d  W: %d  32bit: %d  frame: %d\n",
					name_gif, H, W, field.bit32, index_max + 1);
		}
		f_close(&SDFile);

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
		field.swap = i;
	}

	List_Update_Particle info(void) {
		List_Update_Particle U;
		U.x0 = x;
		U.y0 = y;
		U.x1 = x+W-1;
		U.y1 = y+H-1;
		return U;
	} //Информация используемая для того чтобы отрисовать кадр

	ANIMATION_TRIGGERS trigger = HOVER; //Выбор типа анимации

	struct fieldbite {
		unsigned vector :1; //uint8_t vector = 0; //направление анимации 0- прямое 1- обратное
		unsigned enabled :1;
		unsigned needUpdate :1;
		unsigned bit32 :1;
		unsigned swap :1;
	} field;

private:

	TFT *tft;

	int8_t index_max = 0;                //Максимальный индекс
	int8_t index_current = 0;        //Текущий индекс
	uint8_t delay = 100; //Задержка
	uint8_t offset = 0;

	ANIMATION_STATE state_animation = sSTOP;

	int16_t x = 0;
	int16_t y = 0;
	uint8_t H = 0;
	uint8_t W = 0;

	uint32_t start_time = 0; //Записываем время начала проприсовки анимации

	char name_gif[4] = { 0 };                //Название Gif папки

	uint8_t x0 = 0;
	uint8_t x1 = 0;
	uint8_t y0 = 0;
	uint8_t y1 = 0;

	//List_Update_Particle U;

	//bit
	//uint8_t vector = 0; //направление анимации 0- прямое 1- обратное
	//bool enabled = true;
	//bool needUpdate;
	//uint8_t use_alpha;



	//Открыть картирку по индексу
	void openBMPfromIndex(uint8_t i) {

		char current_patch[32]; //Полный путь к файлу

		sprintf(current_patch, "/Gif/%s/res.bin", name_gif); //Собираем полный путь в файлу

		int res = f_open(&SDFile, current_patch, FA_READ);

		uint8_t BMP_From_File_buf[4100];

		uint32_t index;
		UINT bytesread;

		uint32_t max = H * W;
		int32_t _x, _y;

		uint16_t sColor;
		//uint8_t  sAlpha;
		float    sAlpha_Float, oneminusalpha;
		uint16_t dColor;
		uint8_t  sR,sG,sB;
		uint8_t  dR,dG,dB;

		if (field.bit32) {

			if (res == FR_OK) {


				f_lseek(&SDFile, i * max * 4);

				for (index = 0; index < max; index++) {

					if (index % 1024 == 0)
						f_read(&SDFile, &BMP_From_File_buf[0], 4096, &bytesread);

					_x = x + (index % W);
					_y = (index / W)- 1 + y;

					sAlpha_Float = BMP_From_File_buf[(index % 1024) * 4] / 255.0;

					sR = BMP_From_File_buf[(index % 1024) * 4 + 1];
					sG = BMP_From_File_buf[(index % 1024) * 4 + 2];
					sB = BMP_From_File_buf[(index % 1024) * 4 + 3];

					dColor = tft->LCD->buffer16[_x + _y * tft->LCD->TFT_WIDTH];

					dR = (dColor & 0xF800) >> 8;
					dG = (dColor & 0x7E0) >> 3;
					dB = (dColor & 0x1F) << 3;

					oneminusalpha = 1.0F - sAlpha_Float;

					sR = ((sR * sAlpha_Float) + (oneminusalpha * dR));
					sG = ((sG * sAlpha_Float) + (oneminusalpha * dG));
					sB = ((sB * sAlpha_Float) + (oneminusalpha * dB));

					sColor = tft->RGB565(sR, sG, sB);
					tft->LCD->buffer16[_x + _y * tft->LCD->TFT_WIDTH] = sColor;
				}

			}

		}
		//U = (tft->BMP_From_File_Alpha(x, y, current_patch, offset, field.swap));
		else {




		}
		//tft->BMP_From_File(x, y, current_patch);
		f_close(&SDFile);
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

					if (field.vector == 0) {
						openBMPfromIndex(index_current++);
						if (index_current > index_max) {
							field.vector = 1;
							index_current = index_max - 1;
						}

					} else {

						openBMPfromIndex(index_current--);

						if (index_current < 0) {
							field.vector = 0;
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
