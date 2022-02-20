#ifndef TFT_GIF_H_
#define TFT_GIF_H_

#include "main.h"
#include "TFT.h"



class TFT_gif{
public:

	void init(TFT * _TFT){tft = _TFT;}


	void run(void)
	{

		sprintf(current_patch, "/Gif/%s/%d.bmp", name_gif, index_current); //Собираем полный путь в файлу
       if( use_alpha)
   		 U = (tft->BMP_From_File_Alpha(x, y, current_patch, offset, swap));
       else
		 tft->BMP_From_File(x, y, current_patch);


       if ((uwTick - start_time) > delay)
       {
         index_current++;
         if (index_current > index_max)
    	   index_current = 0;
       }

       start_time = uwTick; //Запомнили начало

	}

	void useAplpha(uint8_t i)  {use_alpha = i;}
	void setOffset( int i)     {offset = i;}
    void setName(char * name)  {sprintf(name_gif, "%s", name);}
    void setDelay(int i)       {delay = i;}
    void SetXY(int X, int Y)   {x = X; y = Y;}
    void SetX(int X)           {x = X;}
    void SetY(int Y)           {y = Y;}
    void setMaxIndex(int i)    {index_max = i;}
    void setSwap(int i)        {swap = i;}

    List_Update_Particle info(void) {return U;}


  private:
	TFT * tft;
    int delay = 100; //Задержка
	uint8_t use_alpha;
	float alpha = 1.0F; //Уровень альфы
	int x = 0;
	int y = 0;
	int index_max;            //Максимальный индекс
	int index_current = 0;    //Текущий индекс
	char name_gif[8];                 //Название Gif папки
	char current_patch[32];           //Полный путь к файлу
	uint32_t start_time; //Записываем время начала проприсовки анимации

	int swap;
	int offset = 0;

	List_Update_Particle U;


};

#endif /* TFT_GIF_H_ */
