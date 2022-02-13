#ifndef FONTS_H
#define FONTS_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "main.h"

#include "stdlib.h"
#include "string.h"

typedef struct {
	int32_t  CurrentX;
	int32_t  CurrentY;
	uint8_t  Inverted;
	uint16_t Color;
	uint16_t BColor;
} uTFT_t;


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

extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;


///////////////////////////////////////////////////////////////////
//  Microelectronika

typedef struct {
	uint8_t FontWidthEng; /*!< Font width in pixels */
	uint8_t FontHeightEng; /*!< Font height in pixels */
	const uint8_t *dataEng; /*!< Pointer to data font data array */
	uint8_t FontWidthRus; /*!< Font width in pixels */
	uint8_t FontHeightRus;
	const uint8_t *dataRus; /*!< Pointer to data font data array */
} FontDefMicroElectronika_t;

extern FontDefMicroElectronika_t Unispace_10x19;
extern FontDefMicroElectronika_t Wingdings2_31x26;
extern FontDefMicroElectronika_t Courier_12x18; //������� �����
extern FontDefMicroElectronika_t Fewture_12x12Eng;
extern FontDefMicroElectronika_t Arial_Narrow9x15;
extern FontDefMicroElectronika_t Verdana39x32;
extern FontDefMicroElectronika_t MicrosoftYaHeiUI24_41x45;
extern FontDefMicroElectronika_t MicrosoftYaHeiUI12_20x22;
extern FontDefMicroElectronika_t Microsoft_YaHeiUI16_27x30;

//////////////////////////////////////// SMOOTH ////////////////////////////////////////
extern const unsigned char _acNotoSansBold15[];
extern const unsigned char _acNotoSansBold36[];
extern const unsigned char _acMyriadPro_Semibold_36[];
extern const unsigned char _acMicrosoftYaHeiUI_24[];
extern const unsigned char _acAAAANumber_Regular_24[];
extern const unsigned char _acFuturisXShadowCTT_24[];

#ifdef __cplusplus
}
#endif

#endif
