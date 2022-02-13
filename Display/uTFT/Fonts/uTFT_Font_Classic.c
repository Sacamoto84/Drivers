#include "uTFT_define.h"
#include "uTFT_Font_Classic.h"

extern uTFT_t uTFT;	
FontDefMicroElectronika_t Font;

// Вывод символа
char uTFT_Classic_Putc(uTFT_LCD_t* LCD, char ch, FontDef_t* Font) {
	uint32_t i, b, j;
	
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				uTFT_SetPixel(LCD, uTFT.CurrentX + j, (uTFT.CurrentY + i), uTFT.Color);} 
			else {
				uTFT_SetPixel(LCD, uTFT.CurrentX + j, (uTFT.CurrentY + i), uTFT.BColor);}
		}
	}
	
	/* Increase pointer */
	uTFT.CurrentX += Font->FontWidth-1;
	
	/* Return character written */
	return ch;
}

// Вывод строки
char uTFT_Classic_Puts(uTFT_LCD_t* LCD, char* str, FontDef_t* Font) {
	while (*str) {
		if (uTFT_Classic_Putc(LCD, *str, Font) != *str) {
			return *str;
		}
		str++;
	}
	/* Everything OK, zero should be returned */
	return *str;
}

// Длина строки
char* uTFT_Classic_GetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font) {
	/* Fill settings */
	SizeStruct->Height = Font->FontHeight;
	SizeStruct->Length = Font->FontWidth * strlen(str);
	
	/* Return pointer */
	return str;
}
