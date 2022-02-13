#ifndef uTFT_H
#define uTFT_H

#include "main.h"

//#include "uTFT_weak.h"
//#include "uTFT_Buffer.h"

//#include"SEGGER.h"
//#include"SEGGER_SYSVIEW.h"

//#define CS_0    HAL_GPIO_WritePin(_LCD->GPIO_CS, _LCD->GPIO_Pin_CS, GPIO_PIN_RESET) //CS -> 0
//#define CS_1    HAL_GPIO_WritePin(_LCD->GPIO_CS, _LCD->GPIO_Pin_CS, GPIO_PIN_RESET) //CS -> 1

//#define CMD     HAL_GPIO_WritePin(_LCD->GPIO_DC, _LCD->GPIO_Pin_DC, GPIO_PIN_RESET)
//#define DATA    HAL_GPIO_WritePin(_LCD->GPIO_DC, _LCD->GPIO_Pin_DC, GPIO_PIN_SET)

//#define begin_tft_write() CS_0
//#define end_tft_write()   CS_1

//#define setWindow ST7789_AddrSet

// Swap any type
//template <typename T> static inline void
//swap_coord(T& a, T& b) { T t = a; a = b; b = t; }


//template <LCD_DRIVER _TFTtype>








//class uTFT {
//public:
	//int16_t  _xPivot;

	//const uTFT_LCD_t * _LCD;

	//uTFT();
	//virtual ~uTFT();
/*
	void init(const uTFT_LCD_t *LCD);
	

	const uTFT_LCD_t * _LCD;


	void LCD_Select(void);
	void LCD_Unselect(void);
	void LCD_Reset(void);    //Сброс экрана

	void LCD_SPI_SendData(uint8_t data); //HAL 8 бит
	void LCD_SPI_SendCmd(uint8_t cmd);   //HAL 8 бит

	void Spi8to16(void); // Перевод SPI на 16 бит слово
	void Spi16to8(void); // Перевод SPI на 8 бит слово

	//ST7789
	void ST7789_Init(void);
	void ST7789_AddrSet(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
	void ST7789_Update(void);



	  // Image rendering
	           // Swap the byte order for pushImage() and pushPixels() - corrects endianness
	  void     setSwapBytes(bool swap);
	  bool     getSwapBytes(void);


	  uint32_t textcolor, textbgcolor;         // Text foreground and background colours
	  uint32_t bitmap_fg, bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

	  uint8_t  textfont,  // Current selected font number
	           textsize,  // Current font size multiplier
	           textdatum, // Text reference datum
	           rotation;  // Display rotation (0-3)

	  uint8_t  decoderState = 0;   // UTF8 decoder state        - not for user access
	  uint16_t decoderBuffer;      // Unicode code-point buffer - not for user access

	 //-------------------------------------- protected ----------------------------------//
protected:

 //int32_t  win_xe, win_ye;          // Window end coords - not needed

 int32_t  _init_width, _init_height; // Display w/h as input, used by setRotation()
 int32_t  _width, _height;           // Display w/h as modified by current rotation
 int32_t  addr_row, addr_col;        // Window position - used to minimise window commands

 int16_t  _xPivot;   // TFT x pivot point coordinate for rotated Sprites
 int16_t  _yPivot;   // TFT x pivot point coordinate for rotated Sprites

 // Viewport variables
 int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
 int32_t  _xDatum;
 int32_t  _yDatum;
 int32_t  _xWidth;
 int32_t  _yHeight;
 bool     _vpDatum;
 bool     _vpOoB;

 int32_t  cursor_x, cursor_y, padX;       // Text cursor x,y and padding setting

 uint32_t fontsloaded;               // Bit field of fonts loaded

 uint8_t  glyph_ab,   // Smooth font glyph delta Y (height) above baseline
          glyph_bb;   // Smooth font glyph delta Y (height) below baseline

 bool     isDigits;   // adjust bounding box for numbers to reduce visual jiggling
 bool     textwrapX, textwrapY;  // If set, 'wrap' text at right and optionally bottom edge of display
 bool     _swapBytes; // Swap the byte order for TFT pushImage()
 bool     locked, inTransaction; // SPI transaction and mutex lock flags

 bool     _booted;    // init() or begin() has already run once

                      // User sketch manages these via set/getAttribute()
 bool     _cp437;        // If set, use correct CP437 charset (default is ON)
 bool     _utf8;         // If set, use UTF-8 decoder in print stream 'write()' function (default ON)
 bool     _psram_enable; // Enable PSRAM use for library functions (TBD) and Sprites

 uint32_t _lastColor; // Buffered value of last colour used
 */
//};


#endif
