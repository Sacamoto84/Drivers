#include "uTFT_Message.h"

uTFT_Message_HandleTypeDef Message_Array[MAX_MESSAGE]; 

void uTFT_Message(void)
{
	uint8_t i;
	//Пробегаем по массиву какой нужно отобразить
	for(i=0; i<2; i++)
	{
		//Проверка того что сообщение нужно удалить
		if(Message_Array[i].DeleteMessage)
		{
			Message_Array[i].DeleteMessage = 0; //Поняли что нужно создать и подготив нужные данные 
			Message_Array[i].Visible = 0;       //Прекращаем отображение
		}
		
		//Проверка того что сообщение нужно создать
		if(Message_Array[i].CreateMessage)
		{
			Message_Array[i].CreateMessage = 0; //Поняли что нужно создать и подготив нужные данные 
			Message_Array[i].Visible = 1;
			Message_Array[i].LiveTick = uwTick; //Запишем время создания сообщения
		}
		
		//Если требуется отобразить
		if(Message_Array[i].Visible)
		{
			//Проверяем сколько уже живет данное сообщение, если меньше то отображаем, или постоянное отображение
			if (((uwTick - Message_Array[i].LiveTick) < Message_Array[i].Timeout) || (Message_Array[i].Timeout == -1))
			{
				uTFT_SetFont(Message_Array[i].wFont);
				uTFT_SetFontColor(Message_Array[i].COLOR_Text, Message_Array[i].COLOR_Text_BACK);
				_uTFT_DrawFilledRectangle(Message_Array[i].rx, Message_Array[i].ry, Message_Array[i].rw, Message_Array[i].rh, Message_Array[i].COLOR_BACK);
				uTFT_GotoXY(Message_Array[i].x, Message_Array[i].y);
				uTFT_Puts_Micro(Message_Array[i].str, 1, 0);
			}
			else
			{
				//Таймаут прошел
				Message_Array[i].Visible = 0;
			}	
		}
	}
}

//Вывести сообщение на экран
void uTFT_Message_Create(uint32_t i)
{
	Message_Array[i].CreateMessage = 1;
}

//Скрыть сообщение на экран
void uTFT_Message_Delete(uint32_t i)
{
	Message_Array[i].DeleteMessage = 1;
}

void uTFT_Message_Visible(uint32_t i, uint32_t visible)
{
	if (visible)
		Message_Array[i].CreateMessage = 1;
	else 
	  Message_Array[i].DeleteMessage = 1;
}

