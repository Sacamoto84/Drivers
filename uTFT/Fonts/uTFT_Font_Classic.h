#ifndef UTFT_FONT_CLASSIC_H
#define UTFT_FONT_CLASSIC_H

/* C++ detection */
#ifdef __cplusplus
 extern "C" {
#endif 

#include "main.h"

#include "stdlib.h"
#include "string.h"

#include "uTFT_Buffer.h"

/**
 *
 * Default fonts library. It is used in all LCD based libraries.
 * 
 * Currently, these fonts are supported:
 *  - 7 x 10 pixels
 *  - 11 x 18 pixels
 *  - 16 x 26 pixels
 */

typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */ //ENG
} FontDef_t;

extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;

typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} FONTS_SIZE_t;

char uTFT_Classic_Putc(uTFT_LCD_t* LCD,  char ch,   FontDef_t* Font);         // Вывод символа
char uTFT_Classic_Puts(uTFT_LCD_t* LCD,  char* str, FontDef_t* Font);      // Вывод строки
char* uTFT_Classic_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);            // Длина строки

/* C++ detection */
#ifdef __cplusplus
}
#endif

 
#endif
