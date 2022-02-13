#ifndef _UTFT_MESSAGE
#define _UTFT_MESSAGE

#ifdef __cplusplus
extern "C" {
#endif

#include "uTFT_define.h"
extern __IO uint32_t uwTick;

//Структура для системы сообщений
typedef struct
{
	 char str[16];
		 
	 uint16_t COLOR_BACK;             //Цвет прямоугольника
	 uint16_t COLOR_Text;             //Цвет Текста
	 uint16_t COLOR_Text_BACK;        //Цвет Фона текста используется как цвет выбранного элемента
	 
	 uint8_t x;  //Место появления текста
	 uint8_t y;  
	 
	 //Данные по прямоугольнику
	 uint8_t rx;
	 uint8_t ry;
	 uint8_t rw;
	 uint8_t rh;
	
	 uint8_t  Visible;       //Признак того что нужно появиться, или текущий статус
	 int32_t  Timeout;       //Время удержания сообщения ms
	 uint32_t LiveTick;      //Тик с которого создалося сообщение
	 uint8_t  CreateMessage; //Что сообщение нужно создать
	 uint8_t  DeleteMessage; //Признак того что сообщение нужно удалить
	
	 FontDefMicroElectronika_t * wFont; //Шрифт используемый в виджете
	
}uTFT_Message_HandleTypeDef;

#define MAX_MESSAGE 2

extern uTFT_Message_HandleTypeDef Message_Array[MAX_MESSAGE];

////////////////////////////////////////////////////////////
void uTFT_Message(void);                                  //
void uTFT_Message_Create(uint32_t i);                     //
void uTFT_Message_Delete(uint32_t i);                     //
void uTFT_Message_Visible(uint32_t i, uint32_t visible);  //
////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif
