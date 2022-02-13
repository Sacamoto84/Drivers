#ifndef _GUI_MESSAGE
#define _GUI_MESSAGE

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "fonts.h"

//Структура для системы сообщений
typedef struct
{
	 char str[16];
		 
	 uint16_t COLOR_BACK;              //Цвет прямоугольника
	 uint16_t COLOR_Text;             //Цвет Текста
	 uint16_t COLOR_Text_BACK;        //Цвет Фона текста используется как цвет выбранного элемента
	 
	 uint8_t x;  //Место появления текста
	 uint8_t y;  
	 
	 //Данные по прямоугольнику
	 uint8_t rx;
	 uint8_t ry;
	 uint8_t rw;
	 uint8_t rh;
	
	 uint8_t  Visible; //Признак того что нужно появиться, или текущий статус
	 uint32_t Timeout; //Время удержания сообщения ms
	 uint32_t LiveTick; //Тик с которого создалося сообщение
	 uint8_t  CreateMessage; //Что сообщение нужно создать
	 uint8_t  DeleteMessage; //Признак того что сообщение нужно удалить
	
	 FontDefMicroElectronika_t * wFont; //Шрифт используемый в виджете
	
}Wiget_Message_HandleTypeDef;

extern Wiget_Message_HandleTypeDef Message_Array[2];











#ifdef __cplusplus
}
#endif

#endif