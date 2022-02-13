#include "../TFT.h"

#include "Font.h"

//////////////////////////////////////////////////////////////////////////////////////
//  MicroElecrtonika 
//////////////////////////////////////////////////////////////////////////////////////

// Устновна фонта
void TFT::Font_Micro_SetFont(FontDefMicroElectronika_t *uFont) {

	Font.FontHeightEng = uFont->FontHeightEng;
	Font.FontWidthEng = uFont->FontWidthEng;
	Font.dataEng = uFont->dataEng;

	Font.FontHeightRus = uFont->FontHeightRus;
	Font.FontWidthRus = uFont->FontWidthRus;
	Font.dataRus = uFont->dataRus;

}

// Вывод символа
void TFT::Font_Micro_Putc(uint8_t ch, uint8_t dx, uint8_t transparrent) {
	int32_t i, j, index;
	const uint8_t *peng = &Font.dataEng[0];
	const uint8_t *prus = &Font.dataRus[0];
	const uint8_t *p;
	const uint8_t *offset;

	int16_t temp;
	int16_t Height;

	if (ch < 0xC0) //ENG
			{

		if (Font.dataEng == NULL)
			return;
		p = peng;
		p += (ch - 32) * ((Font.FontHeightEng) / 8 + 1) * Font.FontWidthEng
				+ (ch - 32);
		offset = p;
		p++;
		Height = Font.FontHeightEng;

	} else //RUS
	{
		if (Font.dataRus == NULL)
			return;
		p = prus;
		p += (ch - 0xC0) * ((Font.FontHeightRus) / 8 + 1) * Font.FontWidthRus
				+ (ch - 0xC0);
		offset = p;
		p++;
		Height = Font.FontHeightRus;
	}

	for (i = 0; i < *offset; i++) {
		for (j = 0; j <= Height / 8; j++) {
			for (index = 0; index < 8; index++) {
				temp = *p;
				if ((temp >> index) & 0x01) {
					SetPixel(uTFT.CurrentX + i, (uTFT.CurrentY + j * 8 + index),
							uTFT.Color);
				} else {
					if (transparrent == 0)
						SetPixel(uTFT.CurrentX + i,
								(uTFT.CurrentY + j * 8 + index), uTFT.BColor);
				}
			}
			p++;
		}
	}

	uTFT.CurrentX += *offset + dx;
}

void TFT::Font_Micro_Puts(char *str, uint8_t dx, uint8_t transparrent) {
	while (*str) {
		Font_Micro_Putc(*str, dx, transparrent);
		str++;
	}
}

//Определение длинны строки в пикселях по типу текущего шрифта
uint16_t TFT::Font_Micro_Find_Len_Str(char *str,
		FontDefMicroElectronika_t *uFont) {
	const uint8_t *peng = &uFont->dataEng[0]; //Начало массива символов
	const uint8_t *p;

	uint16_t lengs;
	uint16_t lenSum;

	lenSum = 0;
	//Количество символов в строке
	lengs = strlen(str); //Количество символов

	//Перебираем каждый символ
	for (uint8_t i = 0; i < lengs; i++) {
		p = peng;
		p += (str[i] - 32) * ((uFont->FontHeightEng) / 8 + 1)
				* uFont->FontWidthEng + (str[i] - 32);

		lenSum += *p + 1;

	}
	lenSum -= 1;

	return lenSum;
}
