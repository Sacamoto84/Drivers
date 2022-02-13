#ifndef UTFT_BUFFER_H
#define UTFT_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"	
	
//Биты регистра _MADCTL	
typedef struct {
 uint8_t MY;
 uint8_t MX;
 uint8_t MV;
 uint8_t ML;		
 uint8_t RGB;
 uint8_t MH;
}_MADCTL;	

//Смещение по XY в режимах 0 90 180 270
typedef struct {
	uint16_t W_0;
	uint16_t H_0;
	uint8_t  DX_0;    
	uint8_t  DY_0;
	uint16_t W_90;
	uint16_t H_90;
	uint8_t  DX_90;
	uint8_t  DY_90;
	uint16_t W_180;
	uint16_t H_180;
	uint8_t  DX_180;
	uint8_t  DY_180;
	uint16_t W_270;
	uint16_t H_270;
	uint8_t  DX_270;
	uint8_t  DY_270;
} _ROTATE_DELTA;

typedef enum {
	OFF = 0,		/* (0) */
	ON, 			  /* (1) */
} ON_OFF;	
	
typedef enum {
	ST7735 = 0,		/* (0) */
	ST7789,			  /* (1) */
	ILI9225,			/* (2) */
	SSD1306,			/* (3) */ 
	PCD8544,			/* (4) */
	ST7735S,
	LCD_USB
} LCD_DRIVER;

typedef enum {
	LCD_SPI = 0,		/* (0) */
	LCD_I2C  			  /* (1) */
} enumI2C_SPI;

typedef struct {
	int16_t     TFT_WIDTH;       // Ширина текущего экрана
	int16_t     TFT_HEIGHT;      // Высота экрана	
	LCD_DRIVER  LCD_Driver;      // 0-ST7735, 1-ST7789, 2-ILI9225 3-SSD1306 4-PCD8544 
	uint8_t     Bit;             // Битность 16 8 4 2 1
	SPI_HandleTypeDef * hspi;
	I2C_HandleTypeDef * hi2c;
	uint16_t *  buffer16;        // Указатель на 16 бит буффер 
	uint8_t  *  buffer8;         // Указатель на 8 бит буффер    
	uint16_t *  palete;          // Указатель на На палитру 
	uint16_t    dx;              // Указатель на На палитру 
	uint16_t    dy;              // Указатель на На палитру 
	GPIO_TypeDef* GPIO_CS;
	uint16_t      GPIO_Pin_CS;
	GPIO_TypeDef* GPIO_DC;
	uint16_t      GPIO_Pin_DC;
	GPIO_TypeDef* GPIO_RESET;
	uint16_t      GPIO_Pin_RESET;
	uint8_t       I2C_Adress;   //
	_MADCTL       MADCTL;
  _ROTATE_DELTA	ROTATE_DELTA; //
} uTFT_LCD_t;
	
// Палитра на 16 цветов
typedef struct {
	uint16_t C0; 
	uint16_t C1;
	uint16_t C2;
	uint16_t C3;
	uint16_t C4;
	uint16_t C5;
	uint16_t C6;
	uint16_t C7;
	uint16_t C8;
	uint16_t C9;
	uint16_t C10;
	uint16_t C11;
	uint16_t C12;
	uint16_t C13;
	uint16_t C14;
	uint16_t C15;	
} uTFT_LCD_Pallete;

extern uTFT_LCD_t current;
extern uTFT_LCD_t LCD_0;
extern uTFT_LCD_t LCD_1;
extern uTFT_LCD_t LCD_2;
extern uTFT_LCD_t LCD_3;

extern uint16_t LCD_Buffer16_0[];
extern uint16_t LCD_Buffer16_1[];
extern uint16_t LCD_Buffer16_2[];
extern uint16_t LCD_Buffer16_3[];

extern volatile uint8_t LCD_Buffer8_0[];
extern uint8_t LCD_Buffer8_1[];
extern uint8_t LCD_Buffer8_2[];	
extern uint8_t LCD_Buffer8_3[];	

extern uint16_t LCD_Pallete_0[];
extern uint16_t LCD_Pallete_1[];
extern uint16_t LCD_Pallete_2[];
extern uint16_t LCD_Pallete_3[];

/* C++ detection */
#ifdef __cplusplus
}
#endif

 
#endif
	