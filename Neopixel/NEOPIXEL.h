#ifndef __NEOPIXEL_H
#define __NEOPIXEL_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#if defined(STM32F103xB)
  #include "stm32f1xx_hal.h"	
#endif

#if defined(STM32F373xC)
  #include "stm32f3xx_hal.h"
#endif	 
	 
#define NEOPIXEL_RESET 32	 
#define NEOPIXEL_LED   8	
#define NEOPIXEL_SPI1          //Выбранный SPI для работы

#if defined(NEOPIXEL_SPI1)
  extern SPI_HandleTypeDef hspi1;
	#define NEOPIXEL_SPI hspi1
#endif

#if defined(NEOPIXEL_SPI2)
  extern SPI_HandleTypeDef hspi2;
	#define NEOPIXEL_SPI hspi2
#endif	 
	 
void NEOPIXEL_Init(void);
void NeoPiexelRGB(uint8_t LED, uint8_t R, uint8_t G, uint8_t B);	 
	 
#ifdef __cplusplus
}
#endif
#endif
