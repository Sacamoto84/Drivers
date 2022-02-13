#ifndef FONTS_H
#define FONTS_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "main.h"
		
#include "uTFT_Font_Classic.h"

	
#if defined(LCD_FONT_USE_U8G) 
  #include "u8g.h"
#endif	

#include "stdlib.h"
#include "string.h"

///////////////////////////////////////////////////////////////////

typedef struct {
	uint8_t FontWidthEng;    /*!< Font width in pixels */
	uint8_t FontHeightEng;   /*!< Font height in pixels */
	const uint8_t *dataEng; /*!< Pointer to data font data array */
	uint8_t FontWidthRus;    /*!< Font width in pixels */
	uint8_t FontHeightRus;
  const uint8_t *dataRus; /*!< Pointer to data font data array */
} FontDefMicroElectronika_t;

extern FontDefMicroElectronika_t Unispace_10x19;
extern FontDefMicroElectronika_t Wingdings2_31x26;
extern FontDefMicroElectronika_t Courier_12x18; //������� �����
extern FontDefMicroElectronika_t Fewture_12x12Eng;
extern FontDefMicroElectronika_t Arial_Narrow9x15;
extern FontDefMicroElectronika_t Verdana39x32;

//////////////////////////////////////// SMOOTH ////////////////////////////////////////
 // This is for the whole font
  typedef struct
  {
    uint16_t gCount;     // Total number of characters
    uint16_t yAdvance;   // Line advance
    uint16_t spaceWidth; // Width of a space character
    int16_t  ascent;     // Height of top of 'd' above baseline, other characters may be taller
    int16_t  descent;    // Offset to bottom of 'p', other characters may have a larger descent
    uint16_t maxAscent;  // Maximum ascent found in font
    uint16_t maxDescent; // Maximum descent found in font
  } fontMetrics;


extern const unsigned char _acNotoSansBold15[];
extern const unsigned char _acNotoSansBold36[];
extern const unsigned char _acMyriadPro_Semibold_36[];
	
#ifdef __cplusplus
}
#endif

 
#endif
