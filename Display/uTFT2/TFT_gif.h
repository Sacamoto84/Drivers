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

extern HiSpeedDWT TimerT5;
extern HiSpeedDWT TimerDWT;
//https://lordicon.com/

enum ANIMATION_TRIGGERS {
	HOVER,      //Запуск без повторения застываем на последнем кадре
	LOOP,       //Запуск с повторнением
	MORPH,      //Запуск вперед пока есть поздействие и возврат
	BOOMERANG   //Запуск приводит к движению в перед и назад
};

enum ANIMATION_COMMAND_STATE {
	STOP,    //Отобразить первый кадр
	PLAY,    //Начать анимацию
	REPLAY,
	PAUSE,   //Остановить на текущем кадре
	END
};

class TFT_gif {
public:

	void init(TFT *_TFT) {tft = _TFT;}

	void command(ANIMATION_COMMAND_STATE c) {

		switch (c) {

		case PLAY:
			if (state_animation != PLAY) {
				index_current = 0;
				state_animation = PLAY;
				field.vector = 0;
			}
			break;

		case REPLAY:
			index_current = 0;
			state_animation = PLAY;
			field.vector = 0;
			break;

		case STOP:
			state_animation = STOP;
			field.vector = 0;
			break;

		default:
			break;
		}

	}
;
	void run(void) {calculate();}

	void setName(char *name) {
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

	void setDelay(int i) {delay = i;}
	void setXY(int X, int Y) {x = X; y = Y;}
	void setX(int X) {x = X;}
	void setY(int Y) {y = Y;}

	List_Update_Particle info(void) {
		List_Update_Particle U;
		U.x0 = x;
		U.y0 = y;
		U.x1 = x + W - 1;
		U.y1 = y + H - 1;
		return U;
	} //Информация используемая для того чтобы отрисовать кадр


	ANIMATION_TRIGGERS      trigger = HOVER; //Выбор типа анимации
	ANIMATION_COMMAND_STATE state_animation = STOP;

	struct fieldbite {
		unsigned vector :1; //uint8_t vector = 0; //направление анимации 0- прямое 1- обратное
		unsigned enabled :1;
		unsigned needUpdate :1;
		unsigned bit32 :1;
	} field;

	uint8_t  delay = 100; //Задержка

	Bitmap bmpStop = {0}; //Картинка отображаемая при отсуствии анимации

private:



	uint8_t  index_max = 0;            //Максимальный индекс
	uint8_t  index_current = 0;        //Текущий индекс

	uint8_t H = 0;
	uint8_t W = 0;

	int16_t x = 0;
	int16_t y = 0;

	TFT *tft;
	uint32_t start_time = 0; //Записываем время начала проприсовки анимации
	char name_gif[8] = { 0 };                //Название Gif папки





    //Открыть картирку по индексу
	void openBMPfromIndex(uint8_t i) {

		//TimerT5.Start();

		char current_patch[32]; //Полный путь к файлу

		sprintf(current_patch, "/Gif/%s/res.bin", name_gif); //Собираем полный путь в файлу

		//TimerDWT.Start();
		int res = f_open(&SDFile, current_patch, FA_READ); //1667us -Of Gen off Fat32 2к  1360 Fat16 16к
		//TimerDWT.Loger("f_open");

		uint8_t BMP_From_File_buf[4100];

		uint32_t index;
		UINT bytesread;

		uint32_t max = H * W;
		int32_t _x, _y;

		uint16_t sColor;

		float sAlpha_Float;
		float oneminusalpha;
		uint16_t dColor;

		uint32_t sR, sG, sB;
		uint32_t dR, dG, dB;

		if (field.bit32) {

			//TimerT5.Start();

			if (res == FR_OK) {

				//TimerDWT.Start();
				f_lseek(&SDFile, i * max * 4); //656us -Of Gen off
				//TimerDWT.Loger("f_lseek");

				for (index = 0; index < max; index++) {

					if (index % 1024 == 0) {
						//TimerDWT.Start();
						f_read(&SDFile, &BMP_From_File_buf[0], 4096,
								&bytesread); ////915us -Of Gen off
						//TimerDWT.Loger("f_read");
					}

					_x = x + (index % W);
					_y = (index / W) - 1 + y;

					sAlpha_Float = BMP_From_File_buf[(index % 1024) * 4]
							/ 255.0;

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

			//TimerT5.Loger("Rutine");
		}
		//U = (tft->BMP_From_File_Alpha(x, y, current_patch, offset, field.swap));
		else {



		}
		//tft->BMP_From_File(x, y, current_patch);

		f_close(&SDFile);  //7uS

	}

	//Открыть картирку по индексу
	void calculate(void) {

		//Расчет следующего интекса с учетом времени кадра
		//if ((uwTick - start_time) > delay)
		{

			if (trigger == MORPH) {

				if (state_animation == STOP) {


					if ((bmpStop.H) &&	(index_current == 0))
					{

						if(field.bit32)
							tft->Bitmap_From_Flash_Alpha(x, y, &bmpStop, 1.0F);
						else
							Bitmap_From_Flash(x, y, &bmpStop);
					}
					else
					{
						openBMPfromIndex(index_current--);
					}

					//openBMPfromIndex(index_current--);
					if (index_current < 0)
						index_current = 0;


					start_time = uwTick; //Запомнили начало
					return;
				}

				if (state_animation == PLAY) {

					openBMPfromIndex(index_current++);

					if (index_current > index_max) {
						index_current = index_max;
					}


					start_time = uwTick; //Запомнили начало
					return;

				}

			}

            //////////////////////////////////////////////////////
			if (trigger == BOOMERANG) {

				if (state_animation == STOP) {

					if (bmpStop.H)
					{
						if(field.bit32)
					      tft->Bitmap_From_Flash_Alpha(x, y, &bmpStop, 1.0F);
						else
						  Bitmap_From_Flash(x, y, &bmpStop);
					}
					else{
					  openBMPfromIndex(0);
					}

					start_time = uwTick; //Запомнили начало
					return;
				}

				if (state_animation == PLAY) {

					if (field.vector == 0) {

						openBMPfromIndex(index_current++);

						if (index_current > index_max) {
							field.vector = 1;
							index_current = index_max - 1;
						}

						start_time = uwTick; //Запомнили начало
						return;

					} else {

						openBMPfromIndex(index_current--);

						if (index_current < 0) {
							field.vector = 0;
							state_animation = STOP;
						}


						start_time = uwTick; //Запомнили начало
						return;

					}

				}

			}
			//////////////////////////////////////////////////////

			if (trigger == LOOP) {

				if (state_animation == PLAY) {
					openBMPfromIndex(index_current);
					index_current++;
					if (index_current > index_max)
						index_current = 0;

					start_time = uwTick; //Запомнили начало
					return;
				}

				if (state_animation == STOP) {

					openBMPfromIndex(0);
					start_time = uwTick; //Запомнили начало
					return;
				}

			}
			//////////////////////////////////////////////////////


			//Один раз воспроизвести и остановиться на последнем кадре
			if (trigger == HOVER) {

				if (state_animation == STOP) {



					SEGGER_RTT_WriteString(0, "STOP\n");

					if (bmpStop.H)
					{

						if(field.bit32)
						{
							TimerDWT.Start();
					        Bitmap_From_Flash_32b(x, y, &bmpStop);
					        TimerDWT.Loger("Bitmap_From_Flash_Alpha 32b");
						}
						else
						{
							TimerDWT.Start();
						    Bitmap_From_Flash(x, y, &bmpStop);
						    TimerDWT.Loger("Bitmap_From_Flash 16b");
						}


					}
					else{

					  SEGGER_RTT_WriteString(0, "openBMPfromIndex (index_max)\n");
					  openBMPfromIndex(index_max);
					}

					start_time = uwTick; //Запомнили начало



					return;
				}

				if (state_animation == PLAY) {

					SEGGER_RTT_WriteString(0, "PLAY\n");

					openBMPfromIndex(index_current);
					index_current++;
					if (index_current > index_max) {
						index_current = index_max;
						state_animation = STOP;
					}

					start_time = uwTick; //Запомнили начало
					return;
				}
			}


		}

		start_time = uwTick; //Запомнили начало

	}


	void Bitmap_From_Flash(int16_t X, int16_t Y, Bitmap *bmp) {

			const uint16_t *p16;
			p16 = bmp->steam16;
			int32_t pX;
			int32_t pY;
		    int _H = bmp->H + Y;
		    int _W = bmp->W + X;

			for ( pY = Y; pY < _H; pY++) {
				for ( pX = X; pX < _W; pX++)
				    tft->LCD->buffer16[pX + pY * tft->LCD->TFT_WIDTH] = *p16++;
			}
	}

	//32 бит BMP с альфа каналом Сохранять как инвертированая альфа и свап  , customAlpha = 1.0 полная альфа
	void Bitmap_From_Flash_32b(int32_t x0, int32_t y0,	Bitmap *bmp) {

		uint32_t index;
		uint32_t index_max;

		int32_t x, y;

		uint32_t sColor;

		//float sAlpha_Float;

		uint32_t oneminusalpha;

		uint32_t dColor;

		uint32_t sR, sG, sB;
		uint32_t dR, dG, dB;

		index_max = bmp->W * bmp->H;

		uint32_t alpha;

		uint8_t *p8;
		p8 = (uint8_t *)&bmp->steam32[0];

		uint32_t deltaX;

		for (index = 0; index < index_max; index++)
		{
			x = (index % (bmp->W)) + x0;
			y =  bmp->H - ((index_max-index) / (bmp->W)) - 1 + y0;

			deltaX = x + y *  tft->LCD->TFT_WIDTH;

			//sAlpha_Float = (float) ( *p8++ / 255.0F);
			alpha = *p8++;
			sR = *p8++;
			sG = *p8++;
			sB = *p8++;

			dColor = tft->LCD->buffer16[deltaX]; //GetPixel(x, y);










			dR = (dColor & 0xF800) >> 8;
			dG = (dColor & 0x7E0) >> 3;
			dB = (dColor & 0x1F) << 3;

			oneminusalpha = 255 - alpha;

			sR = ((sR * alpha) + (oneminusalpha * dR));
			sG = ((sG * alpha) + (oneminusalpha * dG));
			sB = ((sB * alpha) + (oneminusalpha * dB));

		    sColor = tft->RGB565(sR, sG, sB);
			tft->LCD->buffer16[deltaX] = sColor;



			// For speed use fixed point maths and rounding to permit a power of 2 division
			//uint16_t fgR = ((fgc >> 10) & 0x3E) + 1;
			//uint16_t fgG = ((fgc >> 4) & 0x7E) + 1;
			//uint16_t fgB = ((fgc << 1) & 0x3E) + 1;

			//uint16_t bgR = ((bgc >> 10) & 0x3E) + 1;
			//uint16_t bgG = ((bgc >> 4) & 0x7E) + 1;
			//uint16_t bgB = ((bgc << 1) & 0x3E) + 1;

			// Shift right 1 to drop rounding bit and shift right 8 to divide by 256
			//uint16_t r = (((fgR * alpha) + (bgR * (255 - alpha))) >> 9);
			//uint16_t g = (((fgG * alpha) + (bgG * (255 - alpha))) >> 9);
			//uint16_t b = (((fgB * alpha) + (bgB * (255 - alpha))) >> 9);

			// Combine RGB565 colours into 16 bits
			//return (r << 11) | (g << 5) | (b << 0);







		}

	}



};

#endif /* TFT_GIF_H_ */
