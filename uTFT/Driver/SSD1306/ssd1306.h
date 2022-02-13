#ifndef SSD1306_H
#define SSD1306_H


//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
// <c> SSD1306 USE I2C  
#define SSD1306_USED_I2C
// </c>
// <o>   PAGES: Page Size      <0=>I2C1    <1=>I2C2
#define SSD1306_I2C 0
// <c> SSD1306 USE SPI  
//#define SSD1306_USED_SPI
// </c>
// <o>   PAGES: Page Size      <0=>SPI1    <1=>SPI2  <2=>SPI3
#define SSD1306_SPI 0

// <c> SSD1306 USE 2 Display 
//#define SSD1306_USED_2_DISPLAY
// </c>
//------------- <<< end of configuration section >>> ---------------------------

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "uTFT_Font.h"	
#include "stdlib.h"
#include "string.h"
#include "main.h"

//#define DC_H  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1)
//#define DC_L  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 0)

//#define CS_H  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 1)
//#define CS_L  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, 0)

//#define RESET_H HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 1);
//#define RESET_L HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 0);

//#define LOW  0
//#define HIGH 1

//#define LCD_C LOW
//#define LCD_D HIGH

#if defined(SSD1306_USED_SPI)
	
	#define SSD1306_WRITECOMMAND(command)       ssd1306_SPI_Write_COMMAND(command)
	
	#if SSD1306_SPI == 0
    #define LCD_SPI          SPI1
	  #define LCD_SPI_Handle   hspi1
  #endif

  #if SSD1306_SPI == 1
    #define LCD_SPI          SPI2
	  #define LCD_SPI_Handle   hspi2
  #endif

  #if SSD1306_SPI == 2
    #define LCD_SPI          SPI3
	  #define LCD_SPI_Handle   hspi3
  #endif
	
extern SPI_HandleTypeDef LCD_SPI_Handle;

#endif

//#if defined(SSD1306_USED_I2C)
//	#define SSD1306_I2C_ADDR         0x78 //0x78
//  #define SSD1306_I2C_ADDR2        0x7A //0x7A

////  #define SSD1306_WRITECOMMAND(command)       ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
////  #define SSD1306_WRITEDATA(data)             ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))



//  #if SSD1306_I2C == 0
//    #define LCD_I2C_Handle hi2c1
//  #endif
//  #if SSD1306_I2C == 1
//    #define LCD_I2C_Handle hi2c2
//  #endif
//  
//  extern I2C_HandleTypeDef LCD_I2C_Handle;
//#endif

#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64

//typedef struct {
//	uint16_t CurrentX;
//	uint16_t CurrentY;
//	uint8_t  Inverted;
//} SSD1306_t;

//extern SSD1306_t SSD1306;

/* Absolute value */
//extern  volatile uint8_t SSD1306_Buffer_S[1] __attribute__((at(0x20000190+adress_offet)));
//extern  volatile uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8 +10 ]  __attribute__((at(0x20000191+adress_offet)));


void SSD1306_SetPixel(uint16_t x, uint16_t y, uint16_t color);
char SSD1306_GetPixel(uint16_t x, uint16_t y);
void SSD1306_Fill(uint16_t color);
void SSD1306_Invert(void);

void SetDisplay(uint32_t displ);

//ssd1306_LL.c
void ssd1306_I2C_Init(void);
void SSD1306_Init(uTFT_LCD_t* LCD);
void SSD1306_UpdateScreen(uTFT_LCD_t* LCD);
void SSD1306_ON(void);
void SSD1306_OFF(void);
void ssd1306_SPI_I2C_WriteMulti(void);
void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);
void ssd1306_SPI_I2C_Write(uint8_t address, uint8_t reg, uint8_t data);

void ssd1306_Reset(void);

void ssd1306_SPI_Write_COMMAND(uint8_t data);
void ssd1306_SPI_Write_DATA(uint8_t data);



void SSD1306_WRITECOMMAND(uTFT_LCD_t* LCD, uint8_t data);
void SSD1306_WRITEDATA(uTFT_LCD_t* LCD, uint8_t data);
void SSD1306_Contrast(uTFT_LCD_t* LCD, uint8_t c);








/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
