/*
 * TFT.cpp
 *
 *  Created on: 11 мая 2021 г.
 *      Author: Ivan
 */

#include "TFT.h"

#include "TFT_config.h"

#include <math.h>
#include <stdio.h>

void TFT::SetPixel(int32_t x, int32_t y, uint16_t color) {

	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return;

#if defined(TFT_USE_1BIT)
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 1)
#endif
	{
		if (color != 0)
			LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] |= 1 << (y % 8);
		else
			LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] &= ~(1 << (y % 8));
		return;
	}
#endif

#if defined(TFT_USE_4BIT)
	// 16 цветов
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 4)
#endif
	{
		uint32_t stride = ((uint32_t)LCD->TFT_WIDTH + 1) / 2;
		if (x % 2 == 0) {
			LCD->buffer8[x / 2 + y * stride] = (LCD->buffer8[x / 2 + y * stride] & 0x0F) | ((color & 0x0F) << 4);
		} else {
			LCD->buffer8[x / 2 + y * stride] = (LCD->buffer8[x / 2 + y * stride] & 0xF0) | (color & 0x0F);
		}

		return;
	}
#endif

#if defined(TFT_USE_8BIT)
	//OK
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 8)
#endif
	{
		LCD->buffer8[x + y * LCD->TFT_WIDTH] = color;
		return;
	}
#endif

#if defined(TFT_USE_16BIT)
	//OK
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 16)
#endif
	{
		LCD->buffer16[x + y * LCD->TFT_WIDTH] = color;
		return;
	}
#endif

}

void TFT::SetPixel1(int32_t x, int32_t y, uint16_t color) {

	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return;

	if (color != 0)
		LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] |= 1 << (y % 8);
	else
		LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] &= ~(1 << (y % 8));
}

void TFT::SetPixel4(int32_t x, int32_t y, uint16_t color) {

	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return;

	uint32_t stride = ((uint32_t)LCD->TFT_WIDTH + 1) / 2;
	if (x % 2 == 0) {
		LCD->buffer8[x / 2 + y * stride] = (LCD->buffer8[x / 2 + y * stride] & 0x0F) | ((color & 0x0F) << 4);
	} else {
		LCD->buffer8[x / 2 + y * stride] = (LCD->buffer8[x / 2 + y * stride] & 0xF0) | (color & 0x0F);
	}
}

void TFT::SetPixel8(int32_t x, int32_t y, uint16_t color) {

	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return;

	LCD->buffer8[x + y * LCD->TFT_WIDTH] = color;

}

void TFT::SetPixel16(int32_t x, int32_t y, uint16_t color) {

	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return;

	LCD->buffer16[x + y * LCD->TFT_WIDTH] = color;
}

uint16_t TFT::GetPixel(int32_t x, int32_t y) {
	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return 0;

#if defined(TFT_USE_1BIT)
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 1)
#endif
	{
		return (LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] >> (y % 8)) & 0x01;
	}
#endif

#if defined(TFT_USE_4BIT)
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 4)
#endif
	{
		//Старший ниббель - четный x, младший - нечетный (см. SetPixel4)
		uint32_t stride = ((uint32_t)LCD->TFT_WIDTH + 1) / 2;
		uint8_t b = LCD->buffer8[x / 2 + y * stride];
		return (x % 2) ? (b & 0x0F) : (b >> 4);
	}
#endif

#if defined(TFT_USE_8BIT)
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 8)
#endif
	{
		return LCD->buffer8[x + y * LCD->TFT_WIDTH];
	}
#endif

#if defined(TFT_USE_16BIT)
#if !defined (TFT_USE_ONLY_ONE_BIT_COLOR)
  if (LCD->Bit == 16)
#endif
  {
    return LCD->buffer16[x + y * LCD->TFT_WIDTH];
  }
#endif

	return 0; //Битность не поддержана
}

uint16_t TFT::GetPixel1(int32_t x, int32_t y) {

	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return 0;

//		if (color != 0)
//				LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] |= 1 << (y % 8);
//			else
//				LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] &= ~(1 << (y % 8));

	return (LCD->buffer8[x + (y / 8) * LCD->TFT_WIDTH] >> (y % 8)) & 0x01;

}

uint16_t TFT::GetPixel16(int32_t x, int32_t y) {
	if (LCD == NULL || (x < 0) || (y < 0) || (x >= LCD->TFT_WIDTH) || (y >= LCD->TFT_HEIGHT))
		return 0;
	return LCD->buffer16[x + y * LCD->TFT_WIDTH];
}

//Установка цвета в палитре
//Быстрая заливка буфера байтовым шаблоном.
//Если буфер выровнен на 4 байта - заливает словами, хвост - побайтно.
static void TFT_FillPattern8(uint8_t *buf, uint32_t bytes, uint8_t pat) {
	if (buf == NULL) return;
	if (((uintptr_t)buf & 3u) == 0) {
		uint32_t pat32 = (uint32_t)pat | ((uint32_t)pat << 8)
				| ((uint32_t)pat << 16) | ((uint32_t)pat << 24);
		uint32_t *p32 = (uint32_t *)buf;
		while (bytes >= 4) {
			*p32++ = pat32;
			bytes -= 4;
		}
		buf = (uint8_t *)p32;
	}
	while (bytes--)
		*buf++ = pat;
}

void TFT::Fill(uint16_t color) {

	if (LCD == NULL) return;

	uint32_t pixels = (uint32_t)LCD->TFT_HEIGHT * LCD->TFT_WIDTH;

	if (LCD->Bit == 1) {
		uint32_t total_bytes = (((uint32_t)LCD->TFT_HEIGHT + 7) / 8) * (uint32_t)LCD->TFT_WIDTH;
		TFT_FillPattern8(LCD->buffer8, total_bytes, color ? 0xFF : 0x00);
		return;
	}

	if (LCD->Bit == 4) {
		uint32_t total_bytes = (((uint32_t)LCD->TFT_WIDTH + 1) / 2) * LCD->TFT_HEIGHT;
		TFT_FillPattern8(LCD->buffer8, total_bytes,
				(uint8_t)(color | (color << 4)));
		return;
	}

	if (LCD->Bit == 8) {
		TFT_FillPattern8(LCD->buffer8, pixels, (uint8_t)color);
		return;
	}

	if (LCD->Bit == 16) {
		Fill16(color);
		return;
	}
}

void TFT::Fill1(uint16_t color) {
	if (LCD == NULL) return;
	uint32_t total_bytes = (((uint32_t)LCD->TFT_HEIGHT + 7) / 8) * (uint32_t)LCD->TFT_WIDTH;
	TFT_FillPattern8(LCD->buffer8, total_bytes, color ? 0xFF : 0x00);
}

void TFT::Fill4(uint16_t color) {
	if (LCD == NULL) return;
	uint32_t total_bytes = (((uint32_t)LCD->TFT_WIDTH + 1) / 2) * LCD->TFT_HEIGHT;
	TFT_FillPattern8(LCD->buffer8, total_bytes,
				(uint8_t)(color | (color << 4)));
}

void TFT::Fill8(uint16_t color) {
	if (LCD == NULL) return;
	uint32_t pixels = (uint32_t)LCD->TFT_HEIGHT * LCD->TFT_WIDTH;
	TFT_FillPattern8(LCD->buffer8, pixels, (uint8_t)color);
}

void TFT::Fill16(uint16_t color) {
	if (LCD == NULL || LCD->buffer16 == NULL) return;
	uint32_t pixels = (uint32_t)LCD->TFT_HEIGHT * LCD->TFT_WIDTH;
	uint32_t Color = ((uint32_t)color << 16) | color;

	if ((pixels & 1u) == 0 && ((uintptr_t)&LCD->buffer16[0] & 3u) == 0) {
		//Выровненный быстрый путь словами
		uint32_t count = pixels / 2;
		uint32_t *p = (uint32_t *)&LCD->buffer16[0];
		while (count >= 4) {
			p[0] = Color;
			p[1] = Color;
			p[2] = Color;
			p[3] = Color;
			p += 4;
			count -= 4;
		}
		while (count--)
			*p++ = Color;
	} else {
		for (uint32_t i = 0; i < pixels; i++)
			LCD->buffer16[i] = color;
	}
}

uint16_t TFT::alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc) {
	if (alpha == 255) return fgc;
	if (alpha == 0) return bgc;

	// For speed use fixed point maths and rounding to permit a power of 2 division
	uint16_t fgR = ((fgc >> 10) & 0x3E) + 1;
	uint16_t fgG = ((fgc >> 4) & 0x7E) + 1;
	uint16_t fgB = ((fgc << 1) & 0x3E) + 1;

	uint16_t bgR = ((bgc >> 10) & 0x3E) + 1;
	uint16_t bgG = ((bgc >> 4) & 0x7E) + 1;
	uint16_t bgB = ((bgc << 1) & 0x3E) + 1;

	// Shift right 1 to drop rounding bit and shift right 8 to divide by 256
	uint16_t r = (((fgR * alpha) + (bgR * (255 - alpha))) >> 9);
	uint16_t g = (((fgG * alpha) + (bgG * (255 - alpha))) >> 9);
	uint16_t b = (((fgB * alpha) + (bgB * (255 - alpha))) >> 9);

	// Combine RGB565 colours into 16 bits
	return (r << 11) | (g << 5) | (b << 0);
}

#if defined(TFT_USE_BL) || defined(LCD_USE_BL)
#if defined(TFT_PLATFORM_PICO)
#include "hardware/pwm.h"

void TFT::initBacklight(uint8_t pin, uint8_t initial_percent) {
	if (LCD != NULL) {
		LCD->pin_bl = pin;
	}
	gpio_set_function(pin, GPIO_FUNC_PWM);
	uint slice = pwm_gpio_to_slice_num(pin);
	pwm_config config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, 100.0f);
	pwm_init(slice, &config, true);
	pwm_set_wrap(slice, 255);
	setBrightness(initial_percent);
}

void TFT::setBrightness(uint8_t percent) {
	if (percent > 100) percent = 100;
	if (LCD != NULL && LCD->pin_bl != 0xFF) {
		uint slice = pwm_gpio_to_slice_num(LCD->pin_bl);
		uint channel = pwm_gpio_to_channel(LCD->pin_bl);
		pwm_set_chan_level(slice, channel, (percent * 255) / 100);
	}
}
#else
void TFT::initBacklight(uint8_t pin, uint8_t initial_percent) {
	(void)pin;
	(void)initial_percent;
}
void TFT::setBrightness(uint8_t percent) {
	(void)percent;
}
#endif
#endif

