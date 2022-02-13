/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

typedef struct _tFont
{    
  const uint8_t *table;
  uint16_t Width;
  uint16_t Height;
} sFONT;

extern sFONT Font24; 
extern sFONT Font20;
extern sFONT Font16;
extern sFONT Font12;
extern sFONT Font8;

//״נטפעû כוזאע ג סוךעמנו 7 מבüולמל ג 128k
extern const uint8_t Font24_Table [] ;// __attribute__((at(0x08060000)));
extern const uint8_t Font20_Table [] ;//__attribute__((at(0x08060000 +0x1ab8)));
extern const uint8_t Font16_Table [] ;//__attribute__((at(0x08061ab8 +0x0ed8)));
extern const uint8_t Font12_Table [] ;//__attribute__((at(0x08062990 +0x0be0)));
extern const uint8_t Font8_Table  [] ;//__attribute__((at(0x08063570 +0x0474)));

#define LINE(x) ((x) * (((sFONT *)BSP_LCD_GetFont())->Height))

typedef struct
{
  uint16_t TextColor;
  uint16_t BackColor;
  sFONT *pFont;
}LCD_DrawPropTypeDef;

#ifdef __cplusplus
}
#endif
  
#endif /* __FONTS_H */
