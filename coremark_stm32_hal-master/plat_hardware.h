/**
 ******************************************************************************
 * @file    
 * @author  wind
 * @version
 * @date
 * @brief
 ******************************************************************************
 */

#ifndef __PLAT_HARDWARE_H
#define __PLAT_HARDWARE_H

#include "time.h"

#define ITERATIONS 10000

#define EE_TICKS_PER_SEC 1000

#define COMPILER_VERSION "MDK Compiler V5.06 update 4 (build 422)"
#define COMPILER_FLAGS  "-Ohs -no_size_constraints"

#include "main.h"

#include "stm32f4xx_hal.h"

//#include "usart.h"
//#include "gpio.h"

#define PRINTF_HUART    huart3

#endif/*__PLAT_HARDWARE_H*/
