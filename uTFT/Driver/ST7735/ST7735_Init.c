#pragma GCC diagnostic ignored "-Wmissing-braces"

#include "ST7735.h"
#include "main.h"
#include "uTFT_define.h"
	
#include <stdlib.h>
#include <stdio.h> /* for printf */

void LcdWrite7735(uTFT_LCD_t* LCD, char dc, const char *data, int nbytes);

void ST7735_Software_Reset(uTFT_LCD_t* LCD)
{
	if (LCD->GPIO_CS != NULL)	
		LCD_Select(LCD);
	
	LCD_SPI_SendCmd(LCD, 0x01); 

	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD);
}

void ST7735_Display_ON_OFF(uTFT_LCD_t* LCD, char ON)
{
	if (ON)
	{ LCD_SPI_SendCmd(LCD, 0x29); HAL_Delay(100); LCD_SPI_SendCmd(LCD, 0x13); HAL_Delay(10); LCD_SPI_SendCmd(LCD, 0);}
	else
	{	LCD_SPI_SendCmd(LCD, 0x28); HAL_Delay(100); LCD_SPI_SendCmd(LCD, 0x13); HAL_Delay(10); LCD_SPI_SendCmd(LCD, 0);}
}

void ST7735_Display_Invert(uTFT_LCD_t* LCD, char ON)
{
	if (ON)
	{ LCD_SPI_SendCmd(LCD, 0x21);}
	else
	{	LCD_SPI_SendCmd(LCD, 0x20);}
}

//Обновление регистра MADCTL
void ST7735_Update_MADCTL(uTFT_LCD_t* LCD)
{
	uint8_t data;
	
	if (LCD->GPIO_CS != NULL)	
		LCD_Select(LCD);
	
	data = 0;
	data = (LCD->MADCTL.MY << 7) | (LCD->MADCTL.MX << 6) |( LCD->MADCTL.MV << 5) | (LCD->MADCTL.ML << 4) | (LCD->MADCTL.RGB << 3) | (LCD->MADCTL.MH << 2);
	LCD_SPI_SendCmd (LCD, 0x36);
  LCD_SPI_SendData(LCD, data);
	
	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD);
}

// Поворот экрана
void ST7735_Rotate(uTFT_LCD_t* LCD, uint16_t depth)
{
	if (depth == 0)
	{
		LCD->MADCTL.MV = 0;
    LCD->MADCTL.MX = 0;
		LCD->MADCTL.MY = 0;
		LCD->dx = LCD->ROTATE_DELTA.DX_0;
		LCD->dy = LCD->ROTATE_DELTA.DY_0;
		LCD->TFT_WIDTH  = LCD->ROTATE_DELTA.W_0;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_0;
	}
	if (depth == 90)
	{
		LCD->MADCTL.MV = 1;
    LCD->MADCTL.MX = 1;
		LCD->MADCTL.MY = 0;
		LCD->dx = LCD->ROTATE_DELTA.DX_90;
		LCD->dy = LCD->ROTATE_DELTA.DY_90;
		LCD->TFT_WIDTH  = LCD->ROTATE_DELTA.W_90;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_90;
	}
	if (depth == 180)
	{
		LCD->MADCTL.MV = 0;
    LCD->MADCTL.MX = 1;
		LCD->MADCTL.MY = 1;
		LCD->dx = LCD->ROTATE_DELTA.DX_180;
		LCD->dy = LCD->ROTATE_DELTA.DY_180;
		LCD->TFT_WIDTH  = LCD->ROTATE_DELTA.W_180;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_180;
	}
	if (depth == 270)
	{
		LCD->MADCTL.MV = 1;
    LCD->MADCTL.MX = 0;
		LCD->MADCTL.MY = 1;
		LCD->dx = LCD->ROTATE_DELTA.DX_270;
		LCD->dy = LCD->ROTATE_DELTA.DY_270;
		LCD->TFT_WIDTH  = LCD->ROTATE_DELTA.W_270;
		LCD->TFT_HEIGHT = LCD->ROTATE_DELTA.H_270;
	}	
	ST7735_Update_MADCTL(LCD);
}

struct ST7735_cmdBuf {
  uint8_t command;   // ST7735 command byte
  uint8_t delay;     // ms delay after
  uint8_t len;       // length of parameter data
  uint8_t data[16];  // parameter data
};

static const struct ST7735_cmdBuf initializers[] = {
  // SWRESET Software reset 
  { 0x01, 150, 0, 0},
  // SLPOUT Leave sleep mode
  { 0x11,  150, 0, 0},
	{ 0x0B, 0, 1, {0xFC}},
  // FRMCTR1, FRMCTR2 Frame Rate configuration -- Normal mode, idle
  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D) 
  { 0xB1, 0, 3, { 0x01, 0x2C, 0x2D }},
  { 0xB2, 0, 3, { 0x01, 0x2C, 0x2D }},
  // FRMCTR3 Frame Rate configureation -- partial mode
  { 0xB3, 0, 6, { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D }},
  // INVCTR Display inversion (no inversion)
  { 0xB4,  0, 1, { 0x07 }},
  // PWCTR1 Power control -4.6V, Auto mode
  { 0xC0,  0, 3, { 0xA2, 0x02, 0x84}},
  // PWCTR2 Power control VGH25 2.4C, VGSEL -10, VGH = 3 * AVDD
  { 0xC1,  0, 1, { 0xC5}},
  // PWCTR3 Power control, opamp current smal, boost frequency
  { 0xC2,  0, 2, { 0x0A, 0x00 }},
  // PWCTR4 Power control, BLK/2, opamp current small and medium low
  { 0xC3,  0, 2, { 0x8A, 0x2A}},
  // PWRCTR5, VMCTR1 Power control
  { 0xC4,  0, 2, { 0x8A, 0xEE}},
  { 0xC5,  0, 1, { 0x0E }},        
	
  // INVOFF Don't invert display
  { 0x21,  0, 0, 0},    //21->20 Invert
	
	// Memory access directions. row address/col address, bottom to top refesh (10.1.27)
  //{ ST7735_MADCTL,  0, 1, {0x08}},	
	
	// Color mode 16 bit (10.1.30
  { ST7735_COLMOD,   0, 1, {0x05}},
	

  //{ ST7735_CASET,   0, 4,  {0x00, 26, 0x00,  80-1 + 26}},  //80 160
  //{ ST7735_RASET,   0, 4,  {0x00,  1, 0x00, 160-1 + 1} },  //80 160
	
	//{ ST7735_RASET,   0, 4, {0x00, 0x00, 0x00, 0x7F }},
	
  // GMCTRP1 Gamma correction
  { 0xE0, 0, 16, {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
			    0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }},
  // GMCTRP2 Gamma Polarity corrction
  { 0xE1, 0, 16, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
			    0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 }},
  
 // DISPON Display on
 { 0x29, 100, 0, 0},
  // NORON Normal on
  { 0x13,  10, 0, 0},
  // End
  { 0, 0, 0, 0}
};

static const struct ST7735_cmdBuf initializers_ST7735S[] =
{
  // SWRESET Software reset 
  { 0x01, 150, 0, 0},
  // SLPOUT Leave sleep mode
  { 0x11,  150, 0, 0},
	{ 0x0B, 0, 1, {0xFC}},
  // FRMCTR1, FRMCTR2 Frame Rate configuration -- Normal mode, idle
  // frame rate = fosc / (1 x 2 + 40) * (LINE + 2C + 2D) 
  { 0xB1, 0, 3, { 0x01, 0x2C, 0x2D }},
  { 0xB2, 0, 3, { 0x01, 0x2C, 0x2D }},
  // FRMCTR3 Frame Rate configureation -- partial mode
  { 0xB3, 0, 6, { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D }},
  // INVCTR Display inversion (no inversion)
  { 0xB4,  0, 1, { 0x07 }},
  // PWCTR1 Power control -4.6V, Auto mode
  { 0xC0,  0, 3, { 0xA2, 0x02, 0x84}},
  // PWCTR2 Power control VGH25 2.4C, VGSEL -10, VGH = 3 * AVDD
  { 0xC1,  0, 1, { 0xC5}},
  // PWCTR3 Power control, opamp current smal, boost frequency
  { 0xC2,  0, 2, { 0x0A, 0x00 }},
  // PWCTR4 Power control, BLK/2, opamp current small and medium low
  { 0xC3,  0, 2, { 0x8A, 0x2A}},
  // PWRCTR5, VMCTR1 Power control
  { 0xC4,  0, 2, { 0x8A, 0xEE}},
  { 0xC5,  0, 1, { 0x0E }},
	
  // INVOFF Don't invert display
  //{ 0x21,  0, 0, 0},    //21->20 Invert
	
	
	//{ 0x26, 100, 1, {0x08}},
	
	
	
	// Memory access directions. row address/col address, bottom to top refesh (10.1.27)
  //{ ST7735_MADCTL,  0, 1, {0x08}},	
	
	// Color mode 16 bit (10.1.30
  { ST7735_COLMOD,   0, 1, {0x05}},
	

  //{ ST7735_CASET,   0, 4,  {0x00, 26, 0x00,  80-1 + 26}},  //80 160
  //{ ST7735_RASET,   0, 4,  {0x00,  1, 0x00, 160-1 + 1} },  //80 160
	
	//{ ST7735_CASET,   0, 4,  {0x00, 26, 0x00,  159}},  //80 160
  //{ ST7735_RASET,   0, 4,  {0x00,  1, 0x00, 79}},  //80 160
	
	//{ ST7735_RASET,   0, 4, {0x00, 0x00, 0x00, 0x7F }},
	
  // GMCTRP1 Gamma correction
 // { 0xE0, 0, 16, {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
	//		    0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }},
//  // GMCTRP2 Gamma Polarity corrction
 // { 0xE1, 0, 16, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
	//		    0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 }},
//  
	// DISPON Display on
  { 0x29, 100, 0, 0},
  
	// NORON Normal on
  { 0x13,  10, 0, 0},
  // End
};

void LcdWrite7735_CPP(uTFT_LCD_t* LCD, char dc, uint8_t *data, int nbytes)
{  
	if (dc)
	  HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_SET);
	else
	  HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_RESET);
	
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	
	
	HAL_SPI_Transmit(LCD->hspi, (uint8_t *)data, nbytes, 1000);
}

void LcdWrite7735(uTFT_LCD_t* LCD, char dc, const char *data, int nbytes)
{  
	if (dc)
	  HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_SET);
	else
	  HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_RESET);
	
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	
	
	HAL_SPI_Transmit(LCD->hspi, (uint8_t *)data, nbytes, 1000);
}

void ST7735_init(uTFT_LCD_t* LCD) 
{
	const struct ST7735_cmdBuf *cmd;

	if (LCD->GPIO_CS != NULL) LCD_Select(LCD);
	if (LCD->GPIO_RESET != NULL) LCD_Reset(LCD);

    HAL_Delay(100);
		
	for (cmd = initializers; cmd->command; cmd++)
			{
				LcdWrite7735(LCD, LCD_C, &(cmd->command), 1);
				if (cmd->len)
				LcdWrite7735(LCD, LCD_D, cmd->data, cmd->len);
				if (cmd->delay)
					HAL_Delay(cmd->delay);
			}
			
	
	//LCD->MADCTL.RGB = 1;
  //ST7735_Update_MADCTL(LCD);			
	ST7735_Rotate(LCD, 90);		
	ST7735_Display_Invert(LCD, 1);		
	ST7735_Display_ON_OFF(LCD, 1);		
			
			
			
	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD);

}

void ST7735S_init(uTFT_LCD_t* LCD) 
{
	const struct ST7735_cmdBuf *cmd;

	if (LCD->GPIO_CS != NULL)	
	  LCD_Select(LCD);
		
	if (LCD->GPIO_RESET != NULL)	
		LCD_Reset(LCD);
		
	for (cmd = initializers_ST7735S; cmd->command; cmd++)
			{
				LcdWrite7735(LCD, LCD_C, &(cmd->command), 1);

				if (cmd->len)
				LcdWrite7735(LCD, LCD_D, cmd->data, cmd->len);

				if (cmd->delay)
					HAL_Delay(cmd->delay);
			}
				
			
	//
			
	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD);
	
	
	
//	HAL_Delay(1);
//	if (LCD->GPIO_CS != NULL)	
//	  LCD_Select(LCD);
//	
//	LcdWrite7735(LCD, LCD_C, 0x20, 1);	
//	
//	LcdWrite7735(LCD, LCD_C, 0x29, 2);HAL_Delay(100);
//	LcdWrite7735(LCD, LCD_C, 0x13, 2);HAL_Delay(100);
//	
//	if (LCD->GPIO_CS != NULL)	
//		LCD_Unselect(LCD);

}

 

void ST7735_AddrSet(uTFT_LCD_t* LCD, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{	
	
LCD_SPI_SendCmd(LCD, ST7735_CASET);
LCD_SPI_SendData(LCD, 0);	
LCD_SPI_SendData(LCD, x0 + LCD->dx);	
LCD_SPI_SendData(LCD, 0);
LCD_SPI_SendData(LCD, x1 + LCD->dx);
	
LCD_SPI_SendCmd(LCD, ST7735_RASET);
LCD_SPI_SendData(LCD, 0);	
LCD_SPI_SendData(LCD, y0+LCD->dy);	
LCD_SPI_SendData(LCD, 0);
LCD_SPI_SendData(LCD, y1+LCD->dy);	

LCD_SPI_SendCmd(LCD, ST7735_RAMWR);	
		
}

void ST7735_Update(uTFT_LCD_t* LCD)
{
  uint32_t i;

	static uint8_t p;
	
	if (LCD->GPIO_CS != NULL)	
		LCD_Select(LCD);
	
	__NOP();
	__NOP();
	ST7735_AddrSet(LCD, 0,0,LCD->TFT_WIDTH - 1, LCD->TFT_HEIGHT - 1);
	HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, 1);		
	Spi8to16(LCD);
	__NOP();
	__NOP();
	if (LCD->Bit == 4)
	{
		for(uint16_t i = 0; i<LCD->TFT_WIDTH*LCD->TFT_HEIGHT;i++)
		{
			if (i % 2)
			{
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i/2]) & 0x0F]; //4 bit	
			}
			else
			{
				while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
				LCD->hspi->Instance->DR = LCD->palete[(LCD->buffer8[i/2]) >> 4]; //4 bit
			}
		}
	}
	
	if (LCD->Bit == 16)
	{
		for(uint16_t i = 0; i<LCD->TFT_WIDTH*LCD->TFT_HEIGHT;i++)
		{
			  
				LCD->hspi->Instance->DR = LCD->buffer16[i];
			  while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 );
		}
	}
	__NOP();
	__NOP();
	Spi16to8(LCD);
	__NOP();
	__NOP();
	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD); 
}



//typedef struct
//{
//  unsigned short  x; ???
//  unsigned short  y;  
//  const unsigned char   *steam8;
//  const unsigned short  *steam16;
//  const unsigned int    *Palette;
//  const unsigned int    bit; //1,8,16
//}BMP;



void uTFT_Bitmap_From_Flash_Direct_7735(uTFT_LCD_t* LCD, uint16_t X, uint16_t Y, Bitmap * bmp)
{	
	int16_t xmin, xmax;
	int16_t ymin, ymax;

	int16_t deltaX1, deltaX2; 
	int16_t deltaY1, deltaY2; 
	const unsigned short *p;
	
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
	  __NOP();
	  __NOP();
	ST7735_AddrSet(LCD, xmin, ymin, xmax, ymax);
	//ST7735_AddrSet(LCD, 0, 0, 79, 159);
	HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, 1);		
	Spi8to16(LCD);
	  __NOP();
	  __NOP();
		
	
	
	
		//for(uint16_t i = 0; i<(xmax-xmin+1)*(ymax-ymin+1)-1;i++)
	  for(uint16_t i = 0; i < (bmp->y - deltaY2 - deltaY1)  ;i++)
		{	  
			p += deltaX1;	
			
			
			
			for(uint16_t ii = 0; ii< (bmp->x - deltaX2 - deltaX1) ; ii++)
			{
			  LCD->hspi->Instance->DR = *p++;
			    while( (LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0 ); 
			}	
				
			p += deltaX2;	
		}		

	  __NOP();
	  __NOP();
	  __NOP();
	  __NOP();
	  __NOP();
		
	
	  __NOP();
	  __NOP();
	Spi16to8(LCD);
	  __NOP();
	  __NOP();
	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD);
	
	
	
	
}









//Кастомная инициализация из файла
#if (FAT_FS)

#include "fatfs.h"

extern uint8_t retSD;    /* Return value for SD */

extern FRESULT fr;


struct ST7735_cmdBuf initializers_custom[] __attribute__((section(".my_ccmram"))) = {
  { 0x01, 150, 0, 0},                         //0
  { 0x11,  150, 0, 0},                        //1
	{ 0x0B, 0, 1, {0xFC}},                      //2
  { 0xB1, 0, 3, { 0x01, 0x2C, 0x2D }},        //3 FPS
  { 0xB2, 0, 3, { 0x01, 0x2C, 0x2D }},        //4
  { 0xB3, 0, 6, { 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D }},  //5
  { 0xB4,  0, 1, { 0x07 }},            //6
  { 0xC0,  0, 3, { 0xA2, 0x02, 0x84}}, //7
  { 0xC1,  0, 1, { 0xC5}},            //8
  { 0xC2,  0, 2, { 0x0A, 0x00 }},     //9
  { 0xC3,  0, 2, { 0x8A, 0x2A}},      //10
  { 0xC4,  0, 2, { 0x8A, 0xEE}},      //11
  { 0xC5,  0, 1, { 0x0E }},            //12 //VCOM
  { ST7735_COLMOD,   0, 1, {0x05}},    //13
  //{ 0xE0, 0, 16, {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }},  //14 GP
  //{ 0xE1, 0, 16, {0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 }},  //15 GN
	//{ 0x26, 0, 1, 1},		//16	Gammaset
					
 // DISPON Display on
  {0x29, 100, 0, 0},
  // NORON Normal on
  { 0x13,  10, 0, 0},
  // End
  { 0, 0, 0, 0}
};

void ST7735_custom_init(uTFT_LCD_t* LCD) 
{
	//Открываем файлы
	int value;
	uint8_t fps[3];
	uint8_t gp[32];
	uint8_t gn[32];
	uint32_t testByte;
	char str1[64]={0};
    //char str2[64]={0};
	uint8_t VCOM = 14;
	uint8_t GAMMASET = 1;
	//uint8_t i;
	
	struct ST7735_cmdBuf init_gp[] =	{ 0xE0, 0, 16, 
		       {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }};
	
	struct ST7735_cmdBuf init_gn[] =	{ 0xE0, 0, 16, 
		       {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 }};
	
	struct ST7735_cmdBuf init_gs[] = { 0x26, 0, 1, 0};		//16	Gammaset
	
	//Подсветка
	if (f_open(&USERFile, "/Config/lcd_bl.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
     f_gets((char*)str1, sizeof(str1), &USERFile);
		 f_close(&USERFile);
		 value = atoi(str1);
		 TIM9->CCR1 = value;
		 SEGGER_RTT_printf(0,">Init>Read Ok /Config/lcd_bl.txt\n");
		 BT_Send_String(99, "Read OK > lcd_bl.txt");		
  }
	else
	{
		 SEGGER_RTT_printf(0,">Init>Read Error /Config/lcd_bl.txt\n");
		 BT_Send_String(99, "Read Error > lcd_bl.txt");
	}
	
	//VCOM
	if (f_open(&USERFile, "/Config/lcd_vcom.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
     f_gets((char*)str1, sizeof(str1), &USERFile);
		 f_close(&USERFile);
		 VCOM = atoi(str1);
		 SEGGER_RTT_printf(0,">Init>>Read Ok /Config/lcd_vcom.txt\n");
		 BT_Send_String(99, "Read OK > lcd_vcom.txt");		
  }
	else
	{
		 SEGGER_RTT_printf(0,">Init>Read Error /Config/lcd_vcom.txt\n");
		 BT_Send_String(99, "Read Error > lcd_vcom.txt");	
	}
	
	//Gammaset
	if (f_open(&USERFile, "/Config/lcd_gs.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{

     f_gets((char*)str1, sizeof(str1), &USERFile);
		 f_close(&USERFile);
		 GAMMASET = atoi(str1);
		 SEGGER_RTT_printf(0,">Init>Read Ok /Config/lcd_gs.txt\n");
     BT_Send_String(99, ">Read OK > lcd_gs.txt");		
  }
	else
	{
		 SEGGER_RTT_printf(0,">Read Error /Config/lcd_gs.txt\n");
		 BT_Send_String(99, ">Read Error > lcd_gs.txt");
	}
	
	//fps
	if (f_open(&USERFile, "/Config/lcd_fps.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
     f_gets((char*)str1, sizeof(str1), &USERFile);
		 fps[0] = atoi(str1);
		
		 f_gets((char*)str1, sizeof(str1), &USERFile);
		 fps[1] = atoi(str1);
		
		 f_gets((char*)str1, sizeof(str1), &USERFile);
		 fps[2] = atoi(str1);

     f_close(&USERFile);
		 SEGGER_RTT_printf(0,">Read Ok /Config/lcd_fps.txt\n");
		 BT_Send_String(99, "Read OK > lcd_fps.txt");		
  }
	else
	{
		 SEGGER_RTT_printf(0,"Read Error /Config/lcd_fps.txt\n");
		 BT_Send_String(99, "Read Error > lcd_fps.txt");
	}
	
	//gp
	if (f_open(&USERFile, "/Config/lcd_gp.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		for(uint8_t i=0;i<16;i++)
		{
			f_gets((char*)str1, sizeof(str1), &USERFile);
		  gp[i] = atoi(str1);
		}
		
     f_close(&USERFile);
		 SEGGER_RTT_printf(0,"Read Ok /Config/lcd_gp.txt\n");
		 BT_Send_String(99, "Read Ok > lcd_gp.txt");		
  }
	else
	{
		 SEGGER_RTT_printf(0,"Read Error lcd_gp.txt\n");
		 BT_Send_String(99, "Read Error > lcd_gp.txt"); 
	}	
	
	//gn
	if (f_open(&USERFile, "/Config/lcd_gn.txt", FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		for(uint8_t i=0;i<16;i++)
		{
			f_gets((char*)str1, sizeof(str1), &USERFile);
		  gn[i] = atoi(str1);
		}
		
     f_close(&USERFile);
		 SEGGER_RTT_printf(0,">Read Ok /Config/lcd_gn.txt\n");
		 BT_Send_String(99, ">Read Ok /Config/lcd_gn.txt");		
  }
	else
	{
		 SEGGER_RTT_printf(0,">Read Error /Config/lcd_gn.txt\n");
		 BT_Send_String(99, ">Read Error /Config/lcd_gn.txt"); 
	}
	

	struct ST7735_cmdBuf *cmd;
	
	//fps
	initializers_custom[3].data[0] = fps[0];
	initializers_custom[3].data[1] = fps[1];
	initializers_custom[3].data[2] = fps[2];
	
	//VCOM
	initializers_custom[12].data[0]=VCOM;
	
	if (LCD->GPIO_CS != NULL)	
	  LCD_Select(LCD);
		
	if (LCD->GPIO_RESET != NULL)	
		LCD_Reset(LCD);
		HAL_Delay(100);

	for (cmd = initializers_custom; cmd->command; cmd++)
			{
				LcdWrite7735(LCD, LCD_C, &(cmd->command), 1);

				if (cmd->len)
				LcdWrite7735(LCD, LCD_D, &(cmd->data), cmd->len);

				if (cmd->delay)
					HAL_Delay(cmd->delay);
			}

			
	//ST7735_Display_Invert(LCD, 1);		
	//LCD->MADCTL.RGB = 1;
  //ST7735_Update_MADCTL(LCD);			
	//ST7735_Rotate(LCD, 90);		
			
	//ST7735_Display_ON_OFF(LCD, 1);		
				
	ST7735_Display_Invert(LCD, 1);
		
	//if (LCD->GPIO_CS != NULL)	
	//	LCD_Unselect(LCD);

	ST7735_Update_MADCTL(LCD);
	if (LCD->GPIO_CS != NULL)	
		LCD_Unselect(LCD);	 
			
	//uTFT_Update(LCD);		
	
	if(GAMMASET)
	{
		sprintf(str1,"LCD_GAMSET %d", GAMMASET);
		SendToCli(str1, strlen(str1));
		SEGGER_RTT_printf(0,"%s\n", str1);
	}

	sprintf(str1,"LCD_Gamma_P %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", gp[0], gp[1], gp[2], gp[3], gp[4], gp[5], gp[6], gp[7], gp[8], gp[9], gp[10], gp[11], gp[12], gp[13], gp[14], gp[15]);
	SendToCli(str1, strlen(str1));
	SEGGER_RTT_printf(0,"%s\n", str1);
	HAL_Delay(10);
	sprintf(str1,"LCD_Gamma_N %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", gn[0], gn[1], gn[2], gn[3], gn[4], gn[5], gn[6], gn[7], gn[8], gn[9], gn[10], gn[11], gn[12], gn[13], gn[14], gn[15]);
	SendToCli(str1, strlen(str1));
	SEGGER_RTT_printf(0,"%s\n", str1);
	HAL_Delay(10);	

		 
}
#endif







