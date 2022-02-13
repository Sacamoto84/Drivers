#ifndef ST7335_H
#define ST7335_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "spi.h"

#include "main.h"

#include "uTFT_define.h"

//#define ST7735_SPI 2

//#if (ST7735_SPI == 1)
//  #define LCD_SPI        SPI1
//	extern SPI_HandleTypeDef hspi1;
//	#define LCD_SPI_Handle   hspi1
//#endif

//#if (ST7735_SPI == 2)
//  #define LCD_SPI          SPI2
//	extern SPI_HandleTypeDef hspi2;
//	#define LCD_SPI_Handle   hspi2
//#endif

//#if (ST7735_SPI == 3)
//  #define LCD_SPI          SPI3
//	extern SPI_HandleTypeDef hspi3;
//	#define LCD_SPI_Handle   hspi3
//#endif

#define convert24to32(x) (x|0xFF000000)
#define convert24to16(x) (((x & 0x00f80000) >> 19) | ((x & 0x0000fc00) >> 5) |((x & 0x000000f8) << 8))

/* MADCTL [MY MX MV]
 *    MY  row address order   1 (bottom to top), 0 (top to bottom)
 *    MX  col address order   1 (right to left), 0 (left to right)
 *    MV  col/row exchange    1 (exchange),      0 normal
 */

#define ST7735_width  128
#define ST7735_height 128

#define MADCTLGRAPHICS 0x6
#define MADCTLBMP      0x2

//#define A0_GPIO_Port GPIOB
//#define A0_Pin       GPIO_PIN_4

//#define CS_GPIO_Port GPIOB
//#define CS_Pin       GPIO_PIN_7

//#define RESET_GPIO_Port GPIOB
//#define RESET_Pin    GPIO_PIN_6

//#define A0_H  HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, 1)
//#define A0_L  HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, 0)
//#define A0_DC HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, dc)

//#define CS_H  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1)
//#define CS_L  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0)

//#define RESET_H HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 1);
//#define RESET_L HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 0);

//#define LOW  0
//#define HIGH 1

//#define LCD_C LOW
//#define LCD_D HIGH

#define ST7735_CASET  0x2A
#define ST7735_RASET  0x2B
#define ST7735_MADCTL 0x36
#define ST7735_RAMWR  0x2C
#define ST7735_RAMRD  0x2E
#define ST7735_COLMOD 0x3A

#define MADVAL(x) (((x) << 5) | 8)

void LcdWrite7735_CPP(uTFT_LCD_t* LCD, char dc, uint8_t *data, int nbytes);
void LcdWrite7735(uTFT_LCD_t* LCD, char dc, const char *data, int nbytes);
void ST7735_Display_Invert(uTFT_LCD_t* LCD, char ON);
void ST7735_Software_Reset(uTFT_LCD_t* LCD);

void LCD_SPI_SendCmd(uTFT_LCD_t * LCD,  uint8_t cmd);
void LCD_SPI_SendData(uTFT_LCD_t * LCD, uint8_t data);
void ST7735_init(uTFT_LCD_t* LCD) ;
void ST7735_Rotate(uTFT_LCD_t* LCD, uint16_t depth);

void ST7735_custom_init(uTFT_LCD_t* LCD);

#ifdef __cplusplus
}
#endif

#endif