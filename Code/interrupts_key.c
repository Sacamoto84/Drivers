#include "interrupts_key.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
////////////////
//#include "usart.h"

#include "global_define.h"

Key_typedef Key;

void EXTI5_IRQHandler(void)  __attribute__((weak));
void EXTI6_IRQHandler(void)  __attribute__((weak));
void EXTI7_IRQHandler(void)  __attribute__((weak));
void EXTI8_IRQHandler(void)  __attribute__((weak));
void EXTI9_IRQHandler(void)  __attribute__((weak));
void EXTI10_IRQHandler(void) __attribute__((weak));
void EXTI11_IRQHandler(void) __attribute__((weak));
void EXTI12_IRQHandler(void) __attribute__((weak));
void EXTI13_IRQHandler(void) __attribute__((weak));
void EXTI14_IRQHandler(void) __attribute__((weak));
void EXTI15_IRQHandler(void) __attribute__((weak));

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

//Кнопка правый энкодер 1
void EXTI1_IRQHandler(void)
{	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)
{
	Encoder_Right.Press = 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void EXTI6_IRQHandler(void)
{ 
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

void EXTI7_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

//Джостик Enter
void EXTI8_IRQHandler(void)
{
	//Key.E_Press = 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
}

void EXTI9_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
}

//Кнопка левый энкодер 2
void EXTI11_IRQHandler(void)
{
	//Encoder_Left.Press = 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
}

void EXTI12_IRQHandler(void)
{
	//Key.Left_Press	= 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}

void EXTI13_IRQHandler(void)
{
	//Key.Right_Press = 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void EXTI14_IRQHandler(void)
{
	//Key.Down_Press= 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
}


void EXTI15_IRQHandler(void)
{
	//Key.Up_Press = 1;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI->PR & (1<<5))
    EXTI5_IRQHandler();
  if (EXTI->PR & (1<<6))
    EXTI6_IRQHandler();
  if (EXTI->PR & (1<<7))
    EXTI7_IRQHandler();
  if (EXTI->PR & (1<<8))
    EXTI8_IRQHandler();
  if (EXTI->PR & (1<<9))
    EXTI9_IRQHandler();
}

void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & (1<<10))
    EXTI10_IRQHandler();
  if (EXTI->PR & (1<<11))
    EXTI11_IRQHandler();
	if (EXTI->PR & (1<<12))
    EXTI12_IRQHandler();
	if (EXTI->PR & (1<<13))
    EXTI13_IRQHandler();
	if (EXTI->PR & (1<<14))
    EXTI14_IRQHandler();
	if (EXTI->PR & (1<<15))
    EXTI15_IRQHandler();
}






