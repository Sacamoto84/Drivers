#ifndef UTFT_DEFINE_H
#define UTFT_DEFINE_H

#ifdef __cplusplus
 extern "C" {
#endif 

//#include "uTFT_Config_Template.h"	

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h> 

//#define CMD  HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_RESET);
//#define DATA HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_SET);

extern uint16_t LCD_Palette;

extern uint32_t dma_color;  

#ifdef __cplusplus
}
#endif

 
#endif
