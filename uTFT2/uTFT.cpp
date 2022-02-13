/*#include "uTFT.h"
#include "uTFT_weak.h"
#include "ST7789.h"

// Clipping macro for pushImage
#define PI_CLIP                                        \
  if (_vpOoB) return;                                  \
  x+= _xDatum;                                         \
  y+= _yDatum;                                         \
                                                       \
  if ((x >= _vpW) || (y >= _vpH)) return;              \
                                                       \
  int32_t dx = 0;                                      \
  int32_t dy = 0;                                      \
  int32_t dw = w;                                      \
  int32_t dh = h;                                      \
                                                       \
  if (x < _vpX) { dx = _vpX - x; dw -= dx; x = _vpX; } \
  if (y < _vpY) { dy = _vpY - y; dh -= dy; y = _vpY; } \
                                                       \
  if ((x + dw) > _vpW ) dw = _vpW - x;                 \
  if ((y + dh) > _vpH ) dh = _vpH - y;                 \
                                                       \
  if (dw < 1 || dh < 1) return;


//uTFT::uTFT() {
//	// TODO Auto-generated constructor stub
//	SEGGER_RTT_printf(0, "Constructor uTFT\n");
//}

//uTFT::~uTFT() {
//	// TODO Auto-generated destructor stub
//	SEGGER_RTT_printf(0, "Destructor uTFT\n");
//}

//Инициализация дисплея
void uTFT::init(const uTFT_LCD_t * LCD) {
	//SEGGER_RTT_printf(0, "Init uTFT\n");

	_LCD = LCD;

	//switch (_LCD->LCD_Driver) {
	//case SSD1306:
	//	SSD1306_Init();
	//	break;
	//case ST7789:
	//	ST7789_Init();
	//	break;  // 1 - ST7789
	//case ILI9225:
	//	ILI9225_init();
	//	break;	 // 2 - ILI9225
	//case ST7735:
	//	ST7735_init();
	//	break;
	//case ST7735S:
	//	ST7735S_init();
	//	break;
	//default:
	//	break;

	//}
}

void uTFT::LCD_Select(void) { CS_0; }
void uTFT::LCD_Unselect(void) {	CS_1; }

//Сброс экрана
void uTFT::LCD_Reset(void) {
	HAL_GPIO_WritePin(_LCD->GPIO_RESET, _LCD->GPIO_Pin_RESET, GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(_LCD->GPIO_RESET, _LCD->GPIO_Pin_RESET, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(_LCD->GPIO_RESET, _LCD->GPIO_Pin_RESET, GPIO_PIN_SET);
	HAL_Delay(50);
}

//HAL 8 bit вариант
void uTFT::LCD_SPI_SendData(uint8_t data) {
	DATA;
	HAL_SPI_Transmit(_LCD->hspi, &data, 1, 1);
}

void uTFT::LCD_SPI_SendCmd(uint8_t cmd) {
	CMD;
	HAL_SPI_Transmit(_LCD->hspi, &cmd, 1, 1);
}

// Перевод SPI на 16 бит слово
void uTFT::Spi8to16(void) {
	_LCD->hspi->Instance->CR1 &= ~SPI_CR1_SPE; //OFF SPI
	_LCD->hspi->Instance->CR1 |= SPI_CR1_DFF;
	_LCD->hspi->Instance->CR1 |= SPI_CR1_SPE; //ON  SPI
}

// Перевод SPI на 8 бит слово
void uTFT::Spi16to8(void) {
	_LCD->hspi->Instance->CR1 &= ~SPI_CR1_SPE; //OFF SPI
	_LCD->hspi->Instance->CR1 &= ~SPI_CR1_DFF;
	_LCD->hspi->Instance->CR1 |= SPI_CR1_SPE; //ON  SPI
}

void uTFT::ST7789_Init(void) {

	if (_LCD->GPIO_CS != NULL) {
		LCD_Select();
	}

	if (_LCD->GPIO_RESET != NULL) {
		LCD_Reset();
	}

	LCD_SPI_SendCmd(0x01);   //SWRESET
	HAL_Delay(150);

	LCD_SPI_SendCmd(0x11);   //SLPOUT
	HAL_Delay(150);

	LCD_SPI_SendCmd(0x3A);   //COLMOD RGB444(12bit) 0x03, RGB565(16bit) 0x05,
	LCD_SPI_SendData(0x05);  //RGB666(18bit) 0x06
	LCD_SPI_SendCmd(0x36);   //MADCTL
	LCD_SPI_SendData(0x08);  //0x08 B-G-R, 0x14 R-G-B  14

	LCD_SPI_SendCmd(ST77XX_GAMSET);               // ST77XX_GAMSET         0x26
	LCD_SPI_SendData(0x02); // Gamma curve 2 (G1.8)  ST77XX_DGMEN          0xBA
	LCD_SPI_SendCmd(ST77XX_DGMEN);
	LCD_SPI_SendData(0x04); // Enable gamma

	// Positive Voltage Gamma Control
	LCD_SPI_SendCmd(0xe0);
	LCD_SPI_SendData(0xd0);
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(0x03);
	LCD_SPI_SendData(0x08);
	LCD_SPI_SendData(0x0a);
	LCD_SPI_SendData(0x17);
	LCD_SPI_SendData(0x2e);
	LCD_SPI_SendData(0x44);
	LCD_SPI_SendData(0x3f);
	LCD_SPI_SendData(0x29);
	LCD_SPI_SendData(0x10);
	LCD_SPI_SendData(0x0e);
	LCD_SPI_SendData(0x14);
	LCD_SPI_SendData(0x18);

	// Negative Voltage Gamma Control
	LCD_SPI_SendCmd(0xe1);
	LCD_SPI_SendData(0xd0);
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(0x03);
	LCD_SPI_SendData(0x08);
	LCD_SPI_SendData(0x0a);
	LCD_SPI_SendData(0x17);
	LCD_SPI_SendData(0x2e);
	LCD_SPI_SendData(0x44);
	LCD_SPI_SendData(0x3f);
	LCD_SPI_SendData(0x29);
	LCD_SPI_SendData(0x10);
	LCD_SPI_SendData(0x0e);
	LCD_SPI_SendData(0x14);
	LCD_SPI_SendData(0x18);

	LCD_SPI_SendCmd(0x11); // Exit Sleep Mode
//	//HAL_Delay(120);
	HAL_Delay(350);

	LCD_SPI_SendCmd(0x29); // Display on
	HAL_Delay(350); //HAL_Delay(120);

	LCD_SPI_SendCmd(0x21);   //INVON
	LCD_SPI_SendCmd(0x13);   //NORON
	LCD_SPI_SendCmd(0x29);   //DISPON

	if (_LCD->GPIO_CS != NULL) {
		LCD_Unselect();
	}

}

void uTFT::ST7789_AddrSet(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
	LCD_SPI_SendCmd(0x2A);
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(x0 + _LCD->dx);
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(x1 + _LCD->dx);
	LCD_SPI_SendCmd(0x2B);
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(y0 + _LCD->dy);
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(y1 + _LCD->dy);
	LCD_SPI_SendCmd(0x2C);
}

void uTFT::ST7789_Update(void) {

	if (_LCD->GPIO_CS != NULL) {
		LCD_Select();
	}

	int32_t i;

	uint8_t HI;
	uint8_t LO;

	HI = (_LCD->dx + _LCD->TFT_WIDTH - 1) >> 8;
	LO = (_LCD->dx + _LCD->TFT_WIDTH - 1) & 0xFF;

	LCD_SPI_SendCmd(0x2A); // Column addr set
	LCD_SPI_SendData(0x00);     //??????? ??? XSTART
	LCD_SPI_SendData(_LCD->dx);     //??????? ??? XSTART
	LCD_SPI_SendData(HI);     //???????  ??? XEND
	LCD_SPI_SendData(LO);    //??????? ??? XEND

	LCD_SPI_SendCmd(0x2B); // Row addr set
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(_LCD->dy); //TFT_YSTART
	LCD_SPI_SendData(0x00);
	LCD_SPI_SendData(_LCD->dy + _LCD->TFT_HEIGHT - 1); //TFT_YSTART

//	//ST7789_AddrSet(LCD, 0, 0, LCD->TFT_WIDTH - 1, LCD->TFT_HEIGHT - 1);

	LCD_SPI_SendCmd(0x2C); //Memory write

//	//LCD->hspi->Instance->CR1 |= SPI_CR1_DFF;

	HAL_GPIO_WritePin(_LCD->GPIO_DC, _LCD->GPIO_Pin_DC, GPIO_PIN_SET); //DATA

	Spi8to16(); //16bit mode

	if (_LCD->Bit == 4) {
		for (i = 0; i < (_LCD->TFT_HEIGHT * _LCD->TFT_WIDTH) - 1; i++) {
			if (i % 2) {
				while ((_LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0);
				_LCD->hspi->Instance->DR = _LCD->palete[(_LCD->buffer8[i / 2]) & 0x0F]; //4 bit
			} else {
				while ((_LCD->hspi->Instance->SR & SPI_FLAG_TXE) == 0);
				_LCD->hspi->Instance->DR = _LCD->palete[(_LCD->buffer8[i / 2]) >> 4]; //4 bit
			}
		}
	}

	if (_LCD->Bit == 16) {
		for (i = 0; i < (_LCD->TFT_HEIGHT * _LCD->TFT_WIDTH); i++) {
			// HAL_SPI_Transmit_DMA(&hspi1,(uint8_t *)LCD_Buffer, 32400*2);
			while (!(_LCD->hspi->Instance->SR & SPI_SR_TXE))
				;
			_LCD->hspi->Instance->DR = _LCD->buffer16[i];
		}
	}

//////	uTFT_DMA_completed = 0;
//////
//////	HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)LCD->buffer16[0], LCD->TFT_HEIGHT*LCD->TFT_WIDTH);
//////
//////	while (uTFT_DMA_completed == 0)
//////	{
//////	}
	while (!(_LCD->hspi->Instance->SR & SPI_SR_TXE))
		;
	Spi16to8(); //8bit mode

	if (_LCD->GPIO_CS != NULL) {
		LCD_Unselect();
	}

}

////////////////////
//tft_espi

**************************************************************************************
** Function name:           setSwapBytes
** Description:             Used by 16 bit pushImage() to swap byte order in colours
**************************************************************************************
void uTFT::setSwapBytes(bool swap)
{
  _swapBytes = swap;
}


**************************************************************************************
** Function name:           getSwapBytes
** Description:             Return the swap byte order for colours
**************************************************************************************
bool uTFT::getSwapBytes(void)
{
  return _swapBytes;
}*/


