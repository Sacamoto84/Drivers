#ifndef HISPEEDTIMER_H_
#define HISPEEDTIMER_H_

#include "main.h"

#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

#include "global_define.h"

#include <math.h>

#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC

extern __IO uint32_t uwTick;
extern uint32_t SystemCoreClock;

typedef char __attribute__((aligned (4))) char4;

class HiSpeedDWT{
private:

	TIM_HandleTypeDef * htim;

    __IO uint32_t * Count; //Текущий счсетчик

    uint32_t StartCount;
    uint32_t EndCount;

    uint32_t tickToMs;

    uint32_t result;
public:

    uint32_t tickToUs;// получаем кол-во тактов за 1 мкс

	void Start(void){*Count = 0;}
	uint32_t End(void)  {EndCount = *Count; return EndCount;}

	void init(void)
	{
		SEGGER_RTT_printf(0, "%s%sDWT>%s",RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_YELLOW,RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, "%s%s Init SystemCoreClock %d %s\n",RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_GREEN, SystemCoreClock, RTT_CTRL_RESET);
		tickToUs = SystemCoreClock / 1000000; //получаем кол-во тактов за 1 мкс
		tickToMs = tickToUs * 1000;
		SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
        DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик
		Count = (&DWT->CYCCNT); // обнуляем счётчик
		*Count = 0;
	}


	void init(TIM_HandleTypeDef * htimer)
	{
		SEGGER_RTT_printf(0, "%s%sDWT>%s",RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_YELLOW,RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, "%s%s Init Timer SystemCoreClock %d %s\n",RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_GREEN, SystemCoreClock, RTT_CTRL_RESET);
		tickToUs = SystemCoreClock / 1000000 / 2; //получаем кол-во тактов за 1 мкс
		tickToMs = tickToUs * 1000;
		htim = htimer;
		volatile uint32_t * temp;
		temp = &htim->Instance->CNT;
		Count = temp;
		*Count = 0;
		HAL_TIM_Base_Start(htim);
	}

	void Loger(char * str)
	{
		result = *Count - 2; //Поправка на вызов
		char _str[64];
		SEGGER_RTT_printf(0, "%s%sDWT>%s",RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_YELLOW,RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, "%s%s [ %s ]%s>",RTT_CTRL_TEXT_BRIGHT_WHITE, RTT_CTRL_BG_BLACK,  str, RTT_CTRL_RESET);
		uint32_t timeus =  result/ tickToUs;
        sprintf(_str, "%s%s %lu Tick > %lu us %s\n", RTT_CTRL_TEXT_BRIGHT_GREEN, RTT_CTRL_BG_BLACK, result, timeus, RTT_CTRL_RESET);
		SEGGER_RTT_printf(0, _str);

	}

};

#endif /* HISPEEDTIMER_H_ */

