
#ifndef uTFT_WEAK_H
#define uTFT_WEAK_H

#include "main.h"

#include "uTFT_Buffer.h"

__weak void ILI9225_UpdateScreen();
__weak void ILI9225_init();
__weak void ST7789_Init();
__weak void ST7789_Update();
__weak void SSD1306_Invert();
__weak void SSD1306_Init();
__weak void ST7735_init();
__weak void ST7735S_init();
__weak void ST7735_Update();
__weak void SSD1306_UpdateScreen();
__weak void uTFT_USB_Update();

#endif
