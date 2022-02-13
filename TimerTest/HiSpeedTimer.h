/*
 * HiSpeedTimer.h
 *
 *  Created on: 29 дек. 2020 г.
 *      Author: Ivan
 */

#ifndef HISPEEDTIMER_H_
#define HISPEEDTIMER_H_

#include "main.h"

#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"

#include "tim.h"

extern __IO uint32_t uwTick;
extern uint32_t SystemCoreClock;

class HiSpeedTimer {
private:
    TIM_HandleTypeDef * htim;

    __IO uint32_t * Count; //Текущий счсетчик

    uint32_t StartCount;
    uint32_t EndCount;

public:

	void Start(void){*Count = 0;}
	void End(void){EndCount = *Count;}

	void init(TIM_HandleTypeDef * htimer)
	{
		SEGGER_RTT_printf(0, "SystemCoreClock %d\n", SystemCoreClock);
		SEGGER_RTT_printf(0, "Init TIM");
		htim = htimer;
		volatile uint32_t * temp;
		temp = &htim->Instance->CNT;
		Count = temp;
		*Count = 0;
		HAL_TIM_Base_Start(htim);
		SEGGER_RTT_printf(0, "..OK\n");
	}

	void Log(void)
	{
		uint32_t result;
		result = *Count - 2; //Поправка на вызов
		SEGGER_RTT_printf(0, ">------- LOG --------<\n",result);
		SEGGER_RTT_printf(0, "> %d ms\n>--------------------<\n",uwTick);
		SEGGER_RTT_printf(0, "> %d Tick\n",result);
		SEGGER_RTT_printf(0, "> %d us\n",result / (SystemCoreClock/1000000));
		SEGGER_RTT_printf(0, "> %d ms %d us\n",result / (SystemCoreClock/1000) , (result - 100000 *(result / 100000))/(SystemCoreClock/1000000));
		SEGGER_RTT_printf(0, ">--------------------<\n",result);
	}

	void Log(char * str)
	{
		uint32_t result;
		result = *Count - 2; //Поправка на вызов
		SEGGER_RTT_printf(0, "%s> %s\n",RTT_CTRL_TEXT_BRIGHT_GREEN,  str);
		SEGGER_RTT_printf(0, "%s%s> %d ms ",RTT_CTRL_TEXT_BLACK, RTT_CTRL_BG_BRIGHT_GREEN, uwTick);
		SEGGER_RTT_printf(0, "> %d Tick ",result);
		SEGGER_RTT_printf(0, "> %d ms %d us\n",result / (SystemCoreClock/1000) , (result - 100000 *(result / 100000))/(SystemCoreClock/1000000));
		SEGGER_RTT_printf(0, "%s\n", RTT_CTRL_RESET);

	}

};

#endif /* HISPEEDTIMER_H_ */
