#include "uTFT_define.h"

#include "uTFT_Font.h"

uTFT_t uTFT;	

//Работает на все фонты//////////////////////////////////////////////////////////////
void uTFT_SetFontColor(uint16_t Color, uint16_t BColor) {                         //
  uTFT.Color = Color;                                                            // 
  uTFT.BColor = BColor;	                                                           //
}

void uTFT_SetColor(uint16_t Color) {                                              //
  uTFT.Color = Color;                                                            //                                                           //
}

void uTFT_SetBColor(uint16_t BColor) {                                            //
  uTFT.BColor = BColor;                                                            //                                                           //
}

void uTFT_GotoXY(int16_t x, int16_t y) {
	uTFT.CurrentX = x;
	uTFT.CurrentY = y;
}
