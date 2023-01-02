#include "bitmap.h"

#if (FAT_FS)

//32 бит BMP с альфа каналом
List_Update_Particle BMPFromFile32b(TFT * tft, int32_t x0, int32_t y0, char * Name, int offset, int swap)
{
	List_Update_Particle result;

	int res;
	uint32_t index;
	uint32_t index_max;
	int32_t x, y;
	//uint32_t dobavka_x;
	uint16_t sColor;
	//uint8_t  sAlpha;
	float    sAlpha_Float, oneminusalpha;
	uint16_t dColor;
	uint8_t  sR,sG,sB;
	uint8_t  dR,dG,dB;

	uint8_t bmp_header_buffer[54]; //Буффер заголовка
	UINT bytesread;

	uint8_t BMP_From_File_buf[4100];

	BITMAPINFOHEADER bmp_header;

	res = f_open(&SDFile, Name, FA_READ);

	if (res == FR_OK)
	{
		f_read (&SDFile, &bmp_header_buffer, 54, &bytesread);

		bmp_header.bfType     = (uint16_t *)&bmp_header_buffer[0];
		bmp_header.bfSize     = (uint32_t *)&bmp_header_buffer[2];
		bmp_header.bfOffBits  = (uint32_t *)&bmp_header_buffer[10];
		bmp_header.biWidth    = (uint32_t *)&bmp_header_buffer[18];
		bmp_header.biHeight   = (uint32_t *)&bmp_header_buffer[22];
		bmp_header.biBitCount = (uint16_t *)&bmp_header_buffer[28];
		bmp_header.biClrUsed  = (uint32_t *)&bmp_header_buffer[46];

		result.H = *bmp_header.biHeight;
		result.W = *bmp_header.biWidth;

		result.x0  = x0;
		result.y0  = y0;

		result.x1  = x0 + result.W - 1;
		result.y1  = y0 + result.H - 1;



		if (offset)
			f_read (&SDFile, &BMP_From_File_buf, offset, &bytesread);

		index_max = result.H * result.W;

		for(index = 0; index < index_max; index++)
		{
			if (index % 1024 == 0)
				f_read (&SDFile, &BMP_From_File_buf[0], 4096, &bytesread);

			    x = (index % (*bmp_header.biWidth)) + x0;
			    y = *bmp_header.biHeight - (index / (*bmp_header.biWidth)) - 1 + y0;

			    sAlpha_Float = BMP_From_File_buf[(index % 1024)*4+3] / 255.0;

			    if (swap == 0)
			    {
				 sR = BMP_From_File_buf[(index % 1024)*4+2];
				 sG = BMP_From_File_buf[(index % 1024)*4+1];
				 sB = BMP_From_File_buf[(index % 1024)*4];
			    }
			    else
			    {
					 sB = BMP_From_File_buf[(index % 1024)*4+2];
					 sG = BMP_From_File_buf[(index % 1024)*4+1];
					 sR = BMP_From_File_buf[(index % 1024)*4];
			    }

				dColor = tft->GetPixel(x, y);

				dR = (dColor & 0xF800) >> 8;
				dG = (dColor & 0x7E0) >> 3;
				dB = (dColor & 0x1F) << 3;

				oneminusalpha = 1.0F - sAlpha_Float;

				sR = ((sR * sAlpha_Float) + (oneminusalpha * dR));
                sG = ((sG * sAlpha_Float) + (oneminusalpha * dG));
                sB = ((sB * sAlpha_Float) + (oneminusalpha * dB));

			    sColor = RGB565(sR, sG, sB);
				tft->SetPixel(x, y, sColor);
		}
		f_close(&SDFile);
	}

	return result;
}

#endif
