#include "TFT.h"

#include "SEGGER.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
//#include "SEGGER_SYSVIEW.h"

//32 бит BMP с альфа каналом
//void uTFT_BMP_From_File_Alpha(uTFT_LCD_t *LCD, int32_t x0, int32_t y0,
//		char *Name, int offset);

////16x16
//const uint8_t lcd_image_mas[] =
//{ 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x26, 0x22,
//  0x26, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x3C, 0x66, 0x42, 0x64, 0x3F, 0x40, 0x40,
//  0x40, 0x3F, 0x64, 0x42, 0x62, 0x3C, 0x00, 0x00};

//const uint8_t sperma[] =
//{ 0x00, 0x0E, 0x3D, 0x7F, 0x3F, 0x0E, 0x00, 0x00};

//BMP pBmp10 = {
//	16,                  //x
//	16,                  //y
//	&lcd_image_mas[0],   //steam8
//	NULL,                //steam16
//	NULL,                //Palette
//	1                    //bit
//};

//BMP pBmpSp = {
//	8,                  //x
//	8,                  //y
//	&sperma[0],         //steam8
//	NULL,                //steam16
//	NULL,                //Palette
//	1                    //bit
//};

void TFT::Bitmap_From_Flash(int16_t X, int16_t Y, Bitmap *bmp) {

	if (bmp->bit == 1) {

		uint8_t pX;
		uint8_t pY;
		uint8_t tmpCh;
		uint8_t bL;
		const uint8_t *p;

		p = (uint8_t *)bmp->data;

		pY = Y;
		while (pY < Y + bmp->H) {
			pX = X;
			while (pX < X + bmp->W) {
				bL = 0;
				tmpCh = *p++;
				if (tmpCh) {
					while (bL < 8) {
						if (tmpCh & 0x01)
							SetPixel(pX, pY + bL, 1);
						tmpCh >>= 1;
						if (tmpCh)
							bL++;
						else {
							pX++;
							break;
						}
					}
				} else {
					pX++;
				}
			}
			pY += 8;
		}

	}

	if (bmp->bit == 16) {
		const uint16_t *p16;
		p16 = bmp->steam16;

		for (int16_t pY = Y; pY < bmp->H + Y; pY++) {
			for (int16_t pX = X; pX < bmp->W + X; pX++)
				SetPixel(pX, pY, *p16++);
		}
	}

}

// 756us 216MHz
void TFT::Bitmap_From_Flash_Background_16bit(Bitmap *bmp) {

	const uint32_t *p16;
	p16 = (uint32_t *)bmp->steam16;

    uint32_t *p;
	p = (uint32_t *)&LCD->buffer16[0];

	uint32_t count = LCD->TFT_HEIGHT * LCD->TFT_WIDTH / 2 / 4 / 4 / 2;

	while(count--)
	{
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
		*p++ = *p16++; *p++ = *p16++; *p++ = *p16++; *p++ = *p16++;
	}

}

void TFT::Bitmap_From_Flash_Tr(uint16_t X, uint16_t Y,	Bitmap bmp, uint16_t TrColor) {

	if (bmp.bit == 1) {
		const uint8_t *p;
		p = (uint8_t *)bmp.data;

		uint8_t pX;
		uint8_t pY;
		uint8_t tmpCh;
		uint8_t bL;

		pY = Y;
		while (pY < Y + bmp.H) {
			pX = X;
			while (pX < X + bmp.W) {
				bL = 0;
				tmpCh = *p++;
				if (tmpCh) {
					while (bL < 8) {
						if (tmpCh & 0x01)
							SetPixel(pX, pY + bL, 1);
						tmpCh >>= 1;
						if (tmpCh)
							bL++;
						else {
							pX++;
							break;
						}
					}
				} else {
					pX++;
				}
			}
			pY += 8;
		}

	}

	if (bmp.bit == 16) {
		const uint16_t *p16;
		p16 = bmp.steam16;

		uint16_t temp;
		for (uint16_t pY = Y; pY < bmp.H; pY++) {
			for (uint16_t pX = X; pX < bmp.W; pX++) {
				temp = *p16++;
				if (TrColor != temp)
				{
						LCD->buffer16[pX + pY * LCD->TFT_WIDTH] = temp;
				}

			}

		}
	}
}

//32 бит BMP с альфа каналом Сохранять как инвертированая альфа и свап  , customAlpha = 1.0 полная альфа
void TFT::Bitmap_From_Flash_Alpha(int32_t x0, int32_t y0,
		Bitmap *bmp, float customAlpha) {

	volatile uint32_t index;
	volatile uint32_t index_max;
	volatile int32_t x, y;
	volatile uint16_t sColor;
	volatile float sAlpha_Float, oneminusalpha;
	volatile uint16_t dColor;
	volatile uint8_t sR, sG, sB;
	volatile uint8_t dR, dG, dB;

	index_max = bmp->W * bmp->H;

	for (index = 0; index < index_max; index++) {
		x = (index % (bmp->W)) + x0;
		y =  bmp->H - ((index_max-index) / (bmp->W)) - 1 + y0;

		sAlpha_Float = (float) ((bmp->steam32[index] >> 24) / 255.0F)* customAlpha;

		sR = bmp->steam32[index] >> 16;
		sG = bmp->steam32[index] >> 8;
		sB = bmp->steam32[index] & 0xFF;

		dColor = LCD->buffer16[x + y * LCD->TFT_WIDTH]; //GetPixel(x, y);

		dR = (dColor & 0xF800) >> 8;
		dG = (dColor & 0x7E0) >> 3;
		dB = (dColor & 0x1F) << 3;

		oneminusalpha = 1.0F - sAlpha_Float;

		sR = ((sR * sAlpha_Float) + (oneminusalpha * dR));
		sG = ((sG * sAlpha_Float) + (oneminusalpha * dG));
		sB = ((sB * sAlpha_Float) + (oneminusalpha * dB));

		sColor = RGB565(sR, sG, sB);
		LCD->buffer16[x + y * LCD->TFT_WIDTH] = sColor;
	}
}

#if (LCD_COLOR)
void uTFT_Bitmap_From_Flash_Direct(uTFT_LCD_t* LCD, uint16_t X, uint16_t Y, BMP * bmp)
{	
	int16_t xmin, xmax;
	int16_t ymin, ymax;

	int16_t deltaX1, deltaX2; 
	int16_t deltaY1, deltaY2; 
	const int16_t *p;
	
	//Измерение размера окна
	xmin = X;
  xmax = X + bmp->x - 1;
	
	if (xmin >= LCD->TFT_WIDTH ) return;
	if (xmax < 0) return;
	if (xmax >= LCD->TFT_WIDTH)  xmax = LCD->TFT_WIDTH - 1;
	if (xmin<0)	xmin = 0;

	
	ymin = Y;
  ymax = Y + bmp->y - 1;
	
	if (ymin >= LCD->TFT_HEIGHT) return;
	if (ymax < 0) return;
	if (ymax >= LCD->TFT_HEIGHT)  ymax = LCD->TFT_HEIGHT - 1;
	if (ymin<0) ymin = 0;
	
	if ((xmin==xmax)||(ymin==ymax)) return;
	
	deltaX1 = X;
	if (deltaX1 > 0) deltaX1 = 0;
	deltaX1 = deltaX1 * -1;
	
	deltaX2 = X + bmp->x - LCD->TFT_WIDTH;
	if (deltaX2 < 0) deltaX2 = 0;
	
	deltaY1 = Y;
	if (deltaY1 > 0) deltaY1 = 0;
	deltaY1 = deltaY1 * -1;
	
	deltaY2 = Y + bmp->y - LCD->TFT_HEIGHT;
	if (deltaY2 < 0) deltaY2 = 0;
	
	p = &bmp->steam16[deltaY1 * bmp->x];
	
	Spi16to8(LCD);
	if (LCD->GPIO_CS != NULL)	
		LCD_Select(LCD);
	__nop;
	__nop;
	
		switch (LCD->LCD_Driver)
	{
		case ST7789  : ST7789_AddrSet(LCD, xmin, ymin, xmax, ymax);  break;
		case ST7735  : ST7735_AddrSet(LCD, xmin, ymin, xmax, ymax);  break;
		default:
			break;
	}
		
	//ST7735_AddrSet(LCD, 0, 0, 79, 159);
	HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, 1);
	
	Spi8to16(LCD);
	__nop;
	__nop;
	  for(uint32_t i = 0; i < (bmp->y - deltaY2 - deltaY1)  ;i++)
		{	  
			p += deltaX1;		
			for(uint32_t ii = 0; ii< (bmp->x - deltaX2 - deltaX1) ; ii++)
			{
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 ); 
			  LCD->hspi->Instance->DR = *p++;   
			}	
			p += deltaX2;	
		}		

	__nop;
	__nop;
	__nop;
	__nop;
	__nop;
	__nop;
	__nop;
	__nop;
	Spi16to8(LCD);
	__nop;
	__nop;
	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD);	
}
#endif

#if (FAT_FS)

#include "fatfs.h"

#define WORD  uint16_t
#define DWORD uint32_t
#define LONG  uint64_t
 
//структура BITMAPINFOHEADER
 typedef struct tagBITMAPINFOHEADER
 {
	 uint16_t *   bfType;      //Сигнатура "BM"
   uint32_t *   bfSize;      //Размер файла
   uint32_t *   bfOffBits;   //Смещение изображения от начала файл
   uint32_t * biWidth;      //Ширина изображения, точки
   uint32_t *  biHeight;     //Высота изображения, точки
   uint16_t  *  biBitCount;  //Глубина цвета, бит на точку
   uint32_t   biCompression; //Тип компрессии (0 - несжатое изображение)
   uint32_t   biSizeImage;   //Размер изображения, байт
   uint32_t *  biClrUsed;    //Число используемых цветов (0 - максимально возможное для данной глубины цвета)
 } BITMAPINFOHEADER, *PBITMAPINFOHEADER;


BITMAPINFOHEADER bmp_header;

//uint8_t buf[5100] CCMRAM; //Буффер данных

extern char str1[60]; 
 
//Только для картино кратнях 4 по ширине
void TFT::BMP_From_File(int32_t x0, int32_t y0, char * Name)
{
	int res;
	uint32_t index;
	uint32_t index_max;
	int32_t x, y;
	//uint32_t dobavka_x;
	
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
		  for(uint32_t i=0; i < *bmp_header.biClrUsed;i++) bmp_color_table[i] = RGB888_RGB565(bmp_buffer_color_table[i]);
			
		  //Максимальный индекс
		  index_max = *bmp_header.biWidth * *bmp_header.biHeight;
		  
		  for(index = 0; index < index_max; index++)
		  {
			  if (index % 4096 == 0)
					f_read (&SDFile, &BMP_From_File_buf[0], 4096, &bytesread);
				
			  x = (index % (*bmp_header.biWidth)) + x0;
			  y = *bmp_header.biHeight - (index / (*bmp_header.biWidth)) - 1 + y0;	
				SetPixel(x,y, bmp_color_table[BMP_From_File_buf[index % 4096]]);
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
					SetPixel(x,y, RGB565(BMP_From_File_buf[(index % 1365)*3+2], BMP_From_File_buf[(index % 1365)*3+1],BMP_From_File_buf[(index % 1365)*3]));
				}
			}
		}
		f_close(&SDFile);
	}


}

void TFT::BMP_From_File_Tr(int32_t x0, int32_t y0, char * Name, uint16_t tr_color)
{
	int res;
	uint32_t index;
	uint32_t index_max;
	int32_t x, y;
	//uint32_t dobavka_x;
	uint16_t color;
	
	uint8_t bmp_header_buffer[54]; //Буффер заголовка
	UINT bytesread;

	uint8_t * buf =  NULL;

	res = f_open(&SDFile, Name, FA_READ);
	
	if (res == FR_OK)
	{
		buf = (uint8_t*)malloc(4100);
		
		f_read (&SDFile, &bmp_header_buffer, 54, &bytesread);
		
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
		  for(uint32_t i=0; i < *bmp_header.biClrUsed;i++) bmp_color_table[i] = RGB888_RGB565(bmp_buffer_color_table[i]);
			
		  //Максимальный индекс
		  index_max = *bmp_header.biWidth * *bmp_header.biHeight;
		  
		  for(index = 0; index < index_max; index++)
		  {
			  if (index % 4096 == 0)
					f_read (&SDFile, &buf, 4096, &bytesread);
			  x = (index % (*bmp_header.biWidth)) + x0;
			  y = *bmp_header.biHeight - (index / (*bmp_header.biWidth)) - 1 + y0;		  
				if (x <= (int32_t)*bmp_header.biWidth)
				{	
					color =  bmp_color_table[buf[index % 4096]];
                    if (color != tr_color )
					  SetPixel(x,y, color);
				}					
		  }
		
		}
		else //Палитра не используется
		{
			if (*bmp_header.biBitCount == 24)
			{
							
				index_max = *bmp_header.biWidth * *bmp_header.biHeight;				
				for(index = 0; index < index_max; index++)
				{
					if (index % 1365 == 0) 
						f_read (&SDFile, &buf, 4095, &bytesread);
					x = (index % (*bmp_header.biWidth)) + x0;
					y = *bmp_header.biHeight - (index / (*bmp_header.biWidth)) - 1 + y0;	
					if (x <= (int32_t)*bmp_header.biWidth)
					{
						color = RGB565(buf[(index % 1365)*3+2], buf[(index % 1365)*3+1],buf[(index % 1365)*3]);
						if (color != tr_color )
					    SetPixel(x,y, color);
					}
				}
			}
		}
		f_close(&SDFile);
	}

	if (buf)
	{
		free(buf);
		buf = NULL;
	}

}

//32 бит BMP с альфа каналом
List_Update_Particle TFT::BMP_From_File_Alpha(int32_t x0, int32_t y0, char * Name, int offset, int swap)
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

				dColor = GetPixel(x, y);
			
				dR = (dColor & 0xF800) >> 8;
				dG = (dColor & 0x7E0) >> 3;
				dB = (dColor & 0x1F) << 3;
				
				oneminusalpha = 1.0F - sAlpha_Float;
				
				sR = ((sR * sAlpha_Float) + (oneminusalpha * dR));
                sG = ((sG * sAlpha_Float) + (oneminusalpha * dG));
                sB = ((sB * sAlpha_Float) + (oneminusalpha * dB));
		
			    sColor = RGB565(sR, sG, sB);
				SetPixel(x, y, sColor);
		}	
		f_close(&SDFile);
	}

	return result;
}

#endif




void TFT::Bitmap_From_Flash_16b(int16_t X, int16_t Y, Bitmap *bmp) {

		const uint16_t *p16;
		p16 = (uint16_t *)bmp->data;
		int32_t pX;
		int32_t pY;
	    int _H = bmp->H + Y;
	    int _W = bmp->W + X;

		for ( pY = Y; pY < _H; pY++) {
			for ( pX = X; pX < _W; pX++)
			    LCD->buffer16[pX + pY * LCD->TFT_WIDTH] = *p16++;
		}
}

void TFT::Bitmap_From_Flash_24b(int16_t X, int16_t Y, Bitmap *bmp) {

	    SEGGER_RTT_WriteString(0, "Bitmap_From_Flash_24b\r\n");


		const uint8_t *p8;
		p8 = (uint8_t *)bmp->data;
		int32_t pX;
		int32_t pY;
	    int _H = bmp->H + Y;
	    int _W = bmp->W + X;

	    uint8_t A, HI;
		uint16_t Color;
		uint16_t dColor;
		uint32_t delta;

		for ( pY = Y; pY < _H; pY++) {
			for ( pX = X; pX < _W; pX++)
			{
				A = *p8++;
			    HI = *p8++;
			    Color = HI | ( *p8++ << 8 );
			    delta = pX + pY * 240;
			    dColor = LCD->buffer16[delta];
			    LCD->buffer16[delta] = alphaBlend(A, Color, dColor);
			}
		}

}
//32 бит BMP с альфа каналом Сохранять как инвертированая альфа и свап  , customAlpha = 1.0 полная альфа
void TFT::Bitmap_From_Flash_32b(int16_t x0, int16_t y0,	Bitmap *bmp) {
	uint32_t sColor;
	uint32_t dColor;

	uint32_t sR, sG, sB;
	uint32_t dR, dG, dB;
	uint32_t R, G, B;

	int32_t alpha;
	int32_t oneminusalpha;

	int _H = bmp->H + y0;
	int _W = bmp->W + x0;

	int32_t pX;
	int32_t pY;

	uint32_t deltaX;

	uint8_t *p8;
	p8 = (uint8_t *)&bmp->data[0];

	for ( pY = y0; pY < _H; pY++) {
		for ( pX = x0; pX < _W; pX++)
		{
			deltaX = pX + pY * 240;

			alpha = *p8++;
			sR    = *p8++;
			sG    = *p8++;
			sB    = *p8++;

			dColor = LCD->buffer16[deltaX]; //GetPixel(x, y);

			dR = (dColor & 0xF800) >> 8;
			dG = (dColor & 0x7E0)  >> 3;
			dB = (dColor & 0x1F)   << 3;

			oneminusalpha = 255 - alpha;

			R = (uint8_t)(((sR * alpha) + (oneminusalpha * dR)) >> 11 );
			G = (uint8_t)(((sG * alpha) + (oneminusalpha * dG)) >> 10 );
			B = (uint8_t)(((sB * alpha) + (oneminusalpha * dB)) >> 11 );

			sColor = (R << 11) | (G << 5) | B; //tft->RGB565(R, G, B); //(R << 11) | (G << 5) | (B << 0); //
			LCD->buffer16[deltaX] = sColor;
		}
	}
};
