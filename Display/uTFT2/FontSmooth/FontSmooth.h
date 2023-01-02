#ifndef FONTSMOOTH_H
#define FONTSMOOTH_H

#include "../TFT.h"

#include "main.h"

#include "stdlib.h"
#include "string.h"

//Тест вывод на экран всего алфавита
extern void Font_Smooth_showFont(TFT * tft, uint32_t _timeDelay);
extern void Font_Smooth_drawGlyph(TFT * tft, uint16_t code);
extern void Font_Smooth_Load(const unsigned char *massiv);
//void Font_Smooth_drawStr(int x, int y, const char *str, uint16_t color, uint8_t getcolor);

extern void Font_Smooth_drawStr(TFT * tft, const char *str);
extern void Font_Smooth_drawStr(TFT * tft, int x, int y, const char *str);
extern void Font_Smooth_drawStr(TFT * tft, int x, int y, const char *str, uint16_t color);

//Для поддержки русского шрифта
extern void Font_Smooth_drawStr1251(TFT * tft ,int x, int y, const char *str, uint16_t color);

//////////////////////////////////////// SMOOTH ////////////////////////////////////////
extern const unsigned char _acNotoSansBold15[];
extern const unsigned char _acNotoSansBold36[];
extern const unsigned char _acMyriadPro_Semibold_36[];
extern const unsigned char _acMicrosoftYaHeiUI_24[];
extern const unsigned char _acAAAANumber_Regular_24[];
extern const unsigned char _acFuturisXShadowCTT_24[];

extern const unsigned char _acRoboto_Medium_en_ru_14[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_18[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_20[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_24[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_26[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_28[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_30[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_32[]; //en ru
extern const unsigned char _acRoboto_Medium_en_ru_34[]; //en ru

extern const unsigned char _acRoboto_Medium_ru_14[]; //ru
extern const unsigned char _acRoboto_Medium_ru_16[]; //ru
extern const unsigned char _acRoboto_Medium_ru_28[]; //ru
extern const unsigned char _acRoboto_Medium_ru_30[]; //ru
extern const unsigned char _acRoboto_Medium_ru_32[]; //ru
extern const unsigned char _acRoboto_Medium_ru_34[]; //ru
extern const unsigned char _acRoboto_Medium_ru_36[]; //ru

extern const unsigned char _acJetBrainsMono_Medium_en_ru_30[]; //en ru

extern const unsigned char _acJetBrainsMono_Bold_en_ru_32[]; //en ru

#endif
