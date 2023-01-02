#include "FontClassic.h"

/**
  * @brief  Вывод символа
  * @param  ch символ.
  * @param  Font указатель на структуру
  * @param  NoBack true если не нужен задний фон за символом, по умолчанию 0
  * @retval ch status
  */
char FontClassicPutc(TFT * tft, char ch, FontDef_t* Font, uint8_t NoBack ) {
	uint32_t i, b, j;

	if (NoBack)
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				tft->SetPixel(tft->uTFT.CurrentX + j, (tft->uTFT.CurrentY + i), tft->uTFT.Color);}
		}
	}
	else
	for (i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for (j = 0; j < Font->FontWidth; j++) {
			if ((b << j) & 0x8000) {
				tft->SetPixel(tft->uTFT.CurrentX + j, (tft->uTFT.CurrentY + i), tft->uTFT.Color);}
			else {
				tft->SetPixel(tft->uTFT.CurrentX + j, (tft->uTFT.CurrentY + i), tft->uTFT.BColor);}
		}
	}

	/* Increase pointer */
	tft->uTFT.CurrentX += Font->FontWidth-1;

	/* Return character written */
	return ch;
}

/**
  * @brief  Вывод строки
  * @param  htim TIM handle.
  * @param  sMasterConfig pointer to a TIM_MasterConfigTypeDef structure that
  *         contains the selected trigger output (TRGO) and the Master/Slave
  *         mode.
  * @retval HAL status
  */
char FontClassicPuts(TFT * tft, char* str, FontDef_t* Font, uint8_t NoBack) {
	while (*str) {
		if (FontClassicPutc(tft, *str, Font, NoBack) != *str) {
			return *str;
		}
		str++;
	}
	/* Everything OK, zero should be returned */
	return *str;
}

// Длина строки
char* FontClassicGetStringSize(char* str, FONTS_SIZE_t* SizeStruct, FontDef_t* Font) {
	/* Fill settings */
	SizeStruct->Height = Font->FontHeight;
	SizeStruct->Length = Font->FontWidth * strlen(str);

	/* Return pointer */
	return str;
}
