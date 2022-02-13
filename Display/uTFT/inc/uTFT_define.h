#ifndef UTFT_DEFINE_H
#define UTFT_DEFINE_H

#ifdef __cplusplus
 extern "C" {
#endif 

//#include "uTFT_Config_Template.h"	

#include "..\Fonts\uTFT_Font.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h> 

#include "uTFT_Message.h"	
#include "uTFT_Color.h"	
#include "uTFT_Buffer.h"	

extern volatile uint8_t uTFT_DMA_completed; //1-Completed

////Выделение буффера
//#if (LCD_Buffer_Color_Bit != 16)
//	extern volatile  uint8_t LCD_Buffer[TFT_WIDTH*TFT_WIDTH*LCD_Buffer_Color_Bit/8];
//		#else
//	extern volatile uint16_t LCD_Buffer[TFT_HEIGHT*TFT_WIDTH];
//#endif
	
////Созданиние палитры
//#if (LCD_Buffer_Color_Bit == 8)       //256 цветов
//	extern  volatile uint16_t LCD_Buffer_Palette[256];  
//#endif

//#if (LCD_Buffer_Color_Bit == 4)       //16 цвета
//	extern  volatile uint16_t LCD_Buffer_Palette[16];  
//#endif

//#if (LCD_Buffer_Color_Bit == 2)       //4  цвета
//	extern  volatile uint16_t LCD_Buffer_Palette[4];  
//#endif	

#define A0_H  HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, 1)
#define A0_L  HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, 0)
#define A0_DC HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, dc)
	
#if (LCD_USE_CS == 1)
	#define CS_H  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1)
  #define CS_L  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0)
#endif

#define RESET_H HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 1);
#define RESET_L HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 0);

#define LOW  0
#define HIGH 1

#define LCD_C LOW
#define LCD_D HIGH








#define ABS(x)   ((x) > 0 ? (x) : -(x))	
	
//#if defined(uSSD1306)
//  #include "ssd1306.h"
//  #define TFT_WIDTH            128
//  #define TFT_HEIGHT           64
//#endif		
//	
//#if defined(uST7735)
//  #define TFT_WIDTH            128
//  #define TFT_HEIGHT           160
//#endif		
//	
//#if defined(uPCD8544)
//  #define TFT_WIDTH            84
//  #define TFT_HEIGHT           48
//#endif		
//	
//#if defined(TFT9341)
//  #define TFT_WIDTH            320
//  #define TFT_HEIGHT           240
//#endif

//#if defined(ILI9225)
//  #define TFT_WIDTH            176
//  #define TFT_HEIGHT           220
//	
//	#include "ILI9225.h"	
//	
//	#include <spi.h>
//	
//  extern SPI_HandleTypeDef hspi1;
//  extern DMA_HandleTypeDef hdma_spi1_tx;
//		
//	
//#endif
	
extern uint16_t RGB565(uint8_t R,uint8_t G,uint8_t B);	
uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

#define convert24to32(x) (x|0xFF000000)
#define convert24to16(x) (((x & 0x00f80000) >> 19) | ((x & 0x0000fc00) >> 5) |((x & 0x000000f8) << 8))
#define defineRGB565(R,G,B)  (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))

#define CMD  HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_RESET);
#define DATA HAL_GPIO_WritePin(LCD->GPIO_DC, LCD->GPIO_Pin_DC, GPIO_PIN_SET);


typedef struct {
	int16_t CurrentX;
	int16_t CurrentY;
	uint8_t Inverted;
	uint16_t Color;
	uint16_t BColor;
} uTFT_t;

typedef struct
{
  unsigned short  x;
  unsigned short  y;  
  const unsigned char   *steam8;
  const unsigned short  *steam16;
  const unsigned int    *Palette;
  const unsigned int    bit; //1,8,16,32
}Bitmap;

extern uTFT_t uTFT;	
extern uint16_t LCD_Palette;

extern uint32_t dma_color;  


//==================================================================================================================================
//		uTFT_LL                                                                                                                      |
//=================================================================================================================================|
void uTFT_Init(uTFT_LCD_t* LCD);                                                                            // Инициализация дисплея         |
void uTFT_SetPixel(uTFT_LCD_t* LCD, int32_t x, int32_t y, uint16_t color)  ;//__attribute__((always_inline));  // Установка пикселях            |
uint16_t uTFT_GetPixel(uTFT_LCD_t* LCD, int32_t x, int32_t y)              ;//__attribute__((always_inline));  // Чтение пикселях               |
void uTFT_Fill(uTFT_LCD_t* LCD, uint16_t color)                            ;//__attribute__((always_inline));  // Полная заливка цветом экрана  |
void uTFT_Update(uTFT_LCD_t* LCD);

//Установка цвета в палитре
void uTFT_SetColorToPallete(uTFT_LCD_t * LCD, uint8_t index, uint16_t color);


//==================================================================================================================================

//==================================================================================================================================
//		uTFT_Font                                                                                                                   //
//==================================================================================================================================
void uTFT_SetFontColor(uint16_t Color, uint16_t BColor);                 // Установка цвета шрифта и его подложки
void uTFT_GotoXY      (int16_t x, int16_t y);               										// Установка координат шрифта																										

void uTFT_SetFont   (FontDefMicroElectronika_t* uFont);                      // Установкашрифта типа Micro
void uTFT_Putc_Micro(uint8_t ch, uint8_t dx, uint8_t transparrent);       // Вывод символа типа Micro
void uTFT_Puts_Micro(char* str, uint8_t dx, uint8_t transparrent);        // Вывод строки типа Micro




void uTFT_SetColor(uint16_t Color);

//==================================================================================================================================

//==================================================================================================================================
//		uTFT_Line                                                                                                                   //
//==================================================================================================================================
void uTFT_DrawLine          (uTFT_LCD_t* LCD, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t c);                                                   //
void uTFT_VLine             (uTFT_LCD_t* LCD, int32_t X,  int32_t Y1, int32_t Y2, uint16_t color);                                                        //
void uTFT_HLine             (uTFT_LCD_t* LCD, int32_t Y , int32_t X1, int32_t X2, uint16_t color);	                                                      //
void uTFT_MoveToLine        (uTFT_LCD_t*, int32_t x, int32_t y, uint16_t c);                                                                           // 
void uTFT_MoveSetXY         (uint32_t x, uint32_t y);                                                                                      //
void uTFT_DrawCircle        (uTFT_LCD_t* LCD, int16_t x0, int16_t y0, int16_t r, uint16_t c);                                                        //
void uTFT_DrawFilledCircle  (uTFT_LCD_t* LCD, int16_t x0, int16_t y0, int16_t r, uint16_t c);                                                        //
void uTFT_DrawTriangle      (uTFT_LCD_t* LCD, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);       //
void uTFT_DrawFilledTriangle(uTFT_LCD_t* LCD, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);       //
//==================================================================================================================================

//==================================================================================================================================
//Rectagle
//==================================================================================================================================
void uTFT_DrawRectangle        (uTFT_LCD_t* LCD, int32_t x, int32_t y, uint16_t w, uint16_t h, uint16_t c);
void uTFT_DrawFilledRectangle(uTFT_LCD_t * LCD, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c);
void uTFT_ChangeColorRectangle (uTFT_LCD_t* LCD, int32_t x, int32_t y, uint32_t w, int32_t h, uint16_t sColor, uint16_t dColor);
//==================================================================================================================================

//==================================================================================================================================
//Widget	
void SSD1306_progress_bar(uint16_t x, uint16_t y, uint16_t w, uint16_t persent, uint16_t pattern);	
	














extern uint16_t uTFT_oldX;
extern uint16_t uTFT_oldY;

//Определение длинны строки в пикселях по типу текущего шрифта
uint16_t uTFT_Font_Find_Len_Str(char * str, FontDefMicroElectronika_t* uFont);

extern void ST7789_Init(uTFT_LCD_t* LCD);
extern void ST7789_Update(uTFT_LCD_t* LCD);
extern void ST7789_Update_DMA_Cicle_On(void);
extern void ST7789_Update_DMA_Cicle_Off(void);

void uTFT_BackLight(uint8_t i);

void uTFT_Gradient_Vertical(uTFT_LCD_t* LCD, uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t otR, uint8_t otG ,uint8_t otB, uint8_t doR, uint8_t doG, uint8_t doB);


//==================================================================================================================================
//Bitmap
//==================================================================================================================================
void uTFT_Bitmap_From_Flash(uTFT_LCD_t* LCD, int16_t X, int16_t Y, Bitmap * bmp);
void uTFT_Bitmap_From_Flash_Tr(uTFT_LCD_t* LCD, uint16_t X, uint16_t Y, Bitmap bmp, uint16_t TrColor);
//32 бит BMP с альфа каналом Сохранять как инвертированая альфа и свап  , customAlpha = 1.0 полная альфа
void uTFT_Bitmap_From_Flash_Alpha(uTFT_LCD_t* LCD, int32_t x0, int32_t y0, Bitmap * bmp, float customAlpha);

void uTFT_Bitmap_From_Flash_Direct(uTFT_LCD_t* LCD, uint16_t X, uint16_t Y, Bitmap * bmp);
void uTFT_BMP_From_File(uTFT_LCD_t* LCD,int32_t x0, int32_t y0, char * Name);
void uTFT_BMP_From_File_Tr(uTFT_LCD_t* LCD, int32_t x0, int32_t y0, char * Name, uint16_t tr_color);

//32 бит BMP с альфа каналом
void uTFT_BMP_From_File_Alpha(uTFT_LCD_t* LCD, int32_t x0, int32_t y0, char * Name, int offset);
//==================================================================================================================================



void LCD_Reset(uTFT_LCD_t * LCD);
void LCD_Select(uTFT_LCD_t* LCD);
void LCD_Unselect(uTFT_LCD_t* LCD);

void ST7735_Rotate(uTFT_LCD_t* LCD, uint16_t depth);


#ifdef __cplusplus
}
#endif

 
#endif
