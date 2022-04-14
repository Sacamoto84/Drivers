#ifndef HISPEEDTIMER_H_
#define HISPEEDTIMER_H_

//Используется для подсчета времени
//Таймера используем из списка APB1 медленные, тайминг на них SystemCoreClock/2 , наиболее подходят таймера 32бит
//Internal clock
//PSR=0 Up ARR-Максимальное 4294967295 Остальное по умолчанию
//Для использования с 16 бит таймерами нужно использовать void init(&htim5, 1); для 32b init(&htim5, true);
//16 битовые таймера настраиваем на отсчет на 1us за тик значением PSR, максимальное время 65mS

#include "main.h"

#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

//#include "global_define.h"

#include <math.h>

#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC

extern __IO uint32_t uwTick;
extern uint32_t SystemCoreClock;

typedef char __attribute__((aligned (4))) char4;

class HiSpeedDWT{
private:

    bool bit16 = false; //признак тогочто это 16 бит таймер

	TIM_HandleTypeDef * htim;

    __IO uint32_t * Count; //Текущий счсетчик

    uint32_t StartCount;
    uint32_t EndCount;

    uint32_t tickToMs;

    uint32_t result;
public:

    uint32_t tickToUs;// получаем кол-во тактов за 1 мкс

	void     Start(void)  {*Count = 0;}
	uint32_t End  (void)  {EndCount = *Count; return EndCount;}

	//Без параметров использует DWT
	void init(void)
	{
		SEGGER_RTT_printf(0, "%s%sDWT>%s",RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_YELLOW,RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, "%s%s Init SystemCoreClock %d %s\r\n",RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_GREEN, SystemCoreClock, RTT_CTRL_RESET);
		tickToUs = SystemCoreClock / 1000000; //получаем кол-во тактов за 1 мкс
		tickToMs = tickToUs * 1000;
		SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
        DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик
		Count = (&DWT->CYCCNT); // обнуляем счётчик
		*Count = 0;
	}

    //Используем Таймера 32bit и шина APB1 половина частоты
	void init(TIM_HandleTypeDef * htimer, bool timer16 = false)
	{
		SEGGER_RTT_printf(0, "%s%sDWT>%s",RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_YELLOW,RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, "%s%s Init Timer SystemCoreClock %d %s\r\n",RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_GREEN, SystemCoreClock, RTT_CTRL_RESET);

		bit16 = timer16;

		if (timer16)
		  tickToUs = 1;
		else

		  tickToUs = SystemCoreClock / 1000000 / 2; //получаем кол-во тактов за 1 мкс

		tickToMs = tickToUs * 1000;
		htim = htimer;
		volatile uint32_t * temp;
		temp = &htim->Instance->CNT;
		Count = temp;
		*Count = 0;
		HAL_TIM_Base_Start(htim);

	}

    //Используем Таймера 16bit и задание количество тиков на 1us

	void Loger(char * str)
	{
		result = *Count - 2; //Поправка на вызов
		char _str[64];
		SEGGER_RTT_printf(0, "%s%sDWT>%s",RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_YELLOW,RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, "%s%s [ %s ]%s>",RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_BLACK,  str, RTT_CTRL_RESET);
		if (bit16) result = *Count + 2; //Вернем поправку на место
		uint32_t timeus =  result/ tickToUs;
        sprintf(_str, "%s%s %lu Tick > %lu us %s\r\n", RTT_CTRL_TEXT_BRIGHT_GREEN, RTT_CTRL_BG_BLACK, result, timeus, RTT_CTRL_RESET);
		SEGGER_RTT_WriteString(0, _str);
	}



};

#endif /* HISPEEDTIMER_H_ */

