#ifndef TFT_DRIVER_H_
#define TFT_DRIVER_H_

#include "TFT_config.h"

#include "TFT_define.h"

#ifdef TFT_USE_SPI
  #include "TFT_SPI.h"
#endif

class TFT_Driver {
public:

	TFT_LCD_t *LCD;

#ifdef TFT_USE_SPI
	  TFT_SPI SPI;
    #endif

	uint32_t DMA_TX_Complete; //����� ������ ��������� DMA �������� ������ ��� �����
	uint32_t blockUpdate;
	uint32_t needUpdate;

	void init(TFT_LCD_t *_LCD) {
		LCD = _LCD;

		switch (LCD->LCD_Driver) {

#if defined(TFT_DRIVER_SSD1306)
		case SSD1306:
			SSD1306_Init();
			break;
#endif

#if defined(TFT_DRIVER_ST7789)
		    case ST7789:
			  SPI.init(_LCD);
			  ST7789_Init();
			  break;   // 1 - ST7789
#endif

#if defined(TFT_DRIVER_ILI9225)
		    case ILI9225:
			  SPI.init(_LCD);
			  ILI9225_init();
			  break;	 // 2 - ILI9225
#endif

#if defined(TFT_DRIVER_ST7735)
		    case ST7735:
			  ST7735_init();
			  break;
#endif

#if defined(TFT_DRIVER_ST7735S)
			case ST7735S:
			  ST7735S_init();
			  break;
#endif

		default:
			break;
		}
	}

	void Update(void) {
		switch (LCD->LCD_Driver) {

#if defined(TFT_DRIVER_SSD1306)
		case SSD1306:
			SSD1306_UpdateScreen();
			break;
#endif

#if defined(TFT_DRIVER_ILI9225)
		case ILI9225:
			ILI9225_UpdateScreen();
			break; // 2 - ILI9225
#endif

#if defined(TFT_DRIVER_ST7789)
		case ST7789:
			ST7789_Update();
			break; //1-ST7789
#endif

#if defined(TFT_DRIVER_ST7735)
		case ST7735:
			ST7735_Update(); //-V1037
			break; //1-ST7789
#endif

#if defined(TFT_DRIVER_ST7735S)
		case ST7735S:
			ST7735_Update();
			break; //1-ST7789
#endif

#if defined(TFT_DRIVER_LCD_USB)
		case LCD_USB:
			uTFT_USB_Update();
			break;
#endif

		default:
			break;
		}
	}
	;

	void UpdateWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {

		switch (LCD->LCD_Driver) {
#if defined(TFT_DRIVER_ST7789)
		case ST7789:
			ST7789_Update_Window(x1, y1, x2, y2);
			break; //1-ST7789
#endif

		default:
			break;
		}
	}
	;







#if defined(TFT_DRIVER_SSD1306)
	void SSD1306_Init();
	void SSD1306_WRITECOMMAND(uint8_t data);
	void SSD1306_WRITEDATA(uint8_t data);
	void SSD1306_UpdateScreen(void);
	void SSD1306_Contrast(uint8_t c);
#endif

#if defined(TFT_DRIVER_ILI9225)
	void ILI9225_init();
	void ILI9225_init_table16(const uint16_t *table, int16_t size);
	void ILI9225_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void ILI9225_UpdateScreen();
#endif

#if defined(TFT_DRIVER_ST7789)
	void ST7789_Init(void);
	void ST7789_Update(void);
	void ST7789_Update(List_Update_Particle U);
	void ST7789_Update(int x0, int y0, int x1, int y1);
	void ST7789_AddrSet(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	// Поворот экрана
	void ST77XX_Rotate(uint16_t depth);
	//Обновление регистра MADCTL
	void ST77XX_Update_MADCTL(void);
	void ST7789_Update_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

	void ST7789_UpdateDMA4bit(void);
	void ST7789_UpdateDMA4bitV2(void);
	void ST7789_UpdateDMA8bitV2(void);
	void ST7789_UpdateDMA16bitV2(void);
	//DMA с блокировкой
	void ST7789_UpdateDMA16bitV3(void);
	void ST7789_Transmit_Array(char dc, uint8_t *data, int nbytes);
	void ST7789_Update_DMA_Cicle_On(void);
	void ST7789_Update_DMA_Cicle_Off(void);
#endif

#if defined(TFT_DRIVER_ST7735)
	void ST7735_init();
	void ST7735_Update();
#endif

#if defined(TFT_DRIVER_ST7735S)
	void ST7735S_init();
#endif

#if defined(TFT_DRIVER_LCD_USB)
	void uTFT_USB_Update();
#endif

}
;

#endif /* TFT_DRIVER_H_ */
