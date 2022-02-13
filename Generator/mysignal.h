#ifndef MYSIGNAL_H
#define MYSIGNAL_H 120

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#if STM32F103xB
  #include "stm32f1xx_hal.h"
#endif	
	
#if STM32F373xC
  #include "stm32f3xx_hal.h"
#endif	

#if STM32F407xx
  #include "stm32f4xx_hal.h"
#endif	

#include "string.h"

extern const uint16_t CHIRP_1024[];
extern const uint16_t TENS2_1024[];
extern const uint16_t TENS1_1024[];
extern const uint16_t MY_1024[];
extern const uint16_t AmpALT_1024[1024];
extern const uint16_t AttALT_1024[1024];
extern const uint16_t Dnramp_1024[1024];
extern const uint16_t Ramp_1024[1024];
extern const uint16_t Sine_1024[1024];
	
extern const uint16_t TRIAGLE_8b_1024[1024];
extern const uint16_t TRIAGLE_12b_1024[1024];
extern const uint16_t SINE_8b_1024[1024];
extern const uint16_t SINE_12b_1024[1024];

extern const uint16_t HWave_1024[1024];
extern const uint16_t HWave2_1024[1024];

/* C++ detection */
#ifdef __cplusplus
}
#endif

 
#endif
