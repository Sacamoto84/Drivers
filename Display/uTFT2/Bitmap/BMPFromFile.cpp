#include "bitmap.h"

#if (FAT_FS)

//Только для картино кратнях 4 по ширине
void BMPFromFile(TFT * tft, int32_t x0, int32_t y0, char * Name)
{
	int res;
	uint32_t index;
	uint32_t index_max;
	int32_t x, y;
	//uint32_t dobavka_x;

	BITMAPINFOHEADER bmp_header;

	uint8_t bmp_header_buffer[54]; //Буффер заголовка
	UINT bytesread;

	uint8_t BMP_From_File_buf[4100];

	res = f_open(&SDFile, Name, FA_READ);

	if (res == FR_OK)
	{

		f_read (&SDFile, &bmp_header_buffer[0], 54, &bytesread);

		bmp_header.bfType     = (uint16_t *)&bmp_header_buffer[0];
		bmp_header.bfSize     = (uint32_t *)&bmp_header_buffer[2];
		bmp_header.bfOffBits  = (uint32_t *)&bmp_header_buffer[10];
		bmp_header.biWidth    = (uint32_t *)&bmp_header_buffer[18];
		bmp_header.biHeight   = (uint32_t *)&bmp_header_buffer[22];
		bmp_header.biBitCount = (uint16_t *)&bmp_header_buffer[28];
		bmp_header.biClrUsed  = (uint32_t *)&bmp_header_buffer[46];

		if (*bmp_header.biClrUsed != 0) //Палитра используется
		{
		  uint16_t bmp_color_table[256];        //Сама палитра
		  uint32_t bmp_buffer_color_table[256]; //Буффер читаемый для патитры 1K

		  //Читаем таблицу палитры
		  f_read (&SDFile, &bmp_buffer_color_table, *bmp_header.biClrUsed*4, &bytesread);

		  //Конвертируем таблицу плитры
		  for(uint32_t i=0; i < *bmp_header.biClrUsed;i++)
			  bmp_color_table[i] = RGB888_RGB565(bmp_buffer_color_table[i]);

		  //Максимальный индекс
		  index_max = *bmp_header.biWidth * *bmp_header.biHeight;

		  for(index = 0; index < index_max; index++)
		  {
			  if (index % 4096 == 0)
					f_read (&SDFile, &BMP_From_File_buf[0], 4096, &bytesread);

			  x = (index % (*bmp_header.biWidth)) + x0;
			  y = *bmp_header.biHeight - (index / (*bmp_header.biWidth)) - 1 + y0;
				tft->SetPixel(x,y, bmp_color_table[BMP_From_File_buf[index % 4096]]);
		  }

		}
		else //Палитра не используется
		{
			{
				index_max = *bmp_header.biWidth * *bmp_header.biHeight;
				for(index = 0; index < index_max; index++)
				{
					if (index % 1365 == 0)
						f_read (&SDFile, &BMP_From_File_buf[0], 4095, &bytesread);

					x = (index % *bmp_header.biWidth) + x0;
					y = *bmp_header.biHeight - (index / *bmp_header.biWidth) - 1 + y0;
					tft->SetPixel(x,y, RGB565(BMP_From_File_buf[(index % 1365)*3+2], BMP_From_File_buf[(index % 1365)*3+1],BMP_From_File_buf[(index % 1365)*3]));
				}
			}
		}
		f_close(&SDFile);
	}


}

#endif
