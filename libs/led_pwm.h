/**
  ******************************************************************************
  * @file    led_pwm.h
  * @author  Иван Фещенко
  * @version V2.0.1
  * @date    14-Октября-2021
  * @brief   This file contains all the functions prototypes for the Touch Screen driver.
  ******************************************************************************
  * 
  ******************************************************************************
  */

#ifndef __LED_PWM_H
#define __LED_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gpio.h"
#include "main.h"
#include <math.h>

//  Заранее запускаем ШИМ 
//  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);


enum eMODE
{
    NONE,
    HZ,
    BYPASS
}; 

enum eCCR
{
    CCR1,
    CCR2,
    CCR3,
    CCR4,
}; 

class LED_PWM {
	public:
		
		void init(TIM_TypeDef * TIMx, eCCR ccr) {	led_adress = &(TIMx->CCR1) + (ccr*0x4); *led_adress = 0; max = TIMx->ARR;	tickstart = HAL_GetTick();}
		
		void     SetValue  (uint32_t value)     {*led_adress = value;}
		uint32_t GetValue  (void)               {return *led_adress;}
		
		void     SetPersent(uint32_t persent)   {*led_adress = map(persent, 0, 100, 0, max);}
		
		void     Invert(void)                   { *led_adress = ~(*led_adress)& max; }
		void     SetMax  (void)                 {*led_adress = max;}
		void     SetMin  (void)                 {*led_adress = 0;}
		void     SetMode  (eMODE mode)          {_mode = mode;}
		void     SetHZ (float HZ)               {_HZ = HZ;}
	
		void     init_bypass (GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) { _GPIOx = GPIOx; _GPIO_Pin = GPIO_Pin; }
		 
		void run(void){	
            if (_mode==HZ)	  { if (((HAL_GetTick() - tickstart) > (500.0/_HZ))){tickstart = HAL_GetTick(); Invert();}}	
			if (_mode==BYPASS){ if (HAL_GPIO_ReadPin(_GPIOx, _GPIO_Pin)) SetMax(); else	SetMin();	}	
		}
		
		uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}
    
		////////////////////////////////
		volatile uint32_t * led_adress; //Адресс TIM CCR
		
		uint16_t max = 1;
		uint32_t tickstart = 0;;
		
        eMODE _mode = NONE; //0-обычный режим 1-HZ режим 2-байпасс
		
		/* HZ Частота*/
		float _HZ = 1.0F;
        		
		/* Байпасс */
		GPIO_TypeDef* _GPIOx; 
		uint16_t      _GPIO_Pin;
		
};

#ifdef __cplusplus
}
#endif

#endif
