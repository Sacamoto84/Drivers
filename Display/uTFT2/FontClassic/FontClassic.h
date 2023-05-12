/*
 * FontClassic.h
 *
 *  Created on: 1 янв. 2023 г.
 *      Author: Ivan
 */

#ifndef FONTCLASSIC_FONTCLASSIC_H_
#define FONTCLASSIC_FONTCLASSIC_H_

#include "../TFT.h"

#include "main.h"

#include "stdlib.h"
#include "string.h"

// Classic
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */ //ENG
} FontDef_t;

typedef struct {
	uint16_t Length;
	uint16_t Height;
} FONTS_SIZE_t;

//extern FontDef_t Font_7x10;
//extern FontDef_t Font_11x18;
//extern FontDef_t Font_16x26;

extern char   FontClassicPutc(TFT * tft, char ch, FontDef_t* Font, uint8_t NoBack );
extern char   FontClassicPuts(TFT * tft, char * str, FontDef_t* Font, uint8_t NoBack);
extern char * FontClassicGetStringSize(char * str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font);


#endif /* FONTCLASSIC_FONTCLASSIC_H_ */
