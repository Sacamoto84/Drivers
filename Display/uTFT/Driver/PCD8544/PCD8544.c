#include "main.h"
#include "PCD8544.h"



#define DC_H  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 1)
#define DC_L  HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, 0)
#define DC_DC HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, dc)
 
#define CS_H  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 1)
#define CS_L  HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, 0)
 
#define RESET_H HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 1);
#define RESET_L HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, 0);

unsigned char PCD8544_Buffer[PCD8544_BUFFER_SIZE];
unsigned char PCD8544_UpdateXmin = 0, PCD8544_UpdateXmax = 0, PCD8544_UpdateYmin = 0, PCD8544_UpdateYmax = 0;
unsigned char PCD8544_x;
unsigned char PCD8544_y;

void PCD8544_Init(unsigned char contrast) {
	CS_H;
	//Reset
	RESET_L;
	HAL_Delay(10);
	RESET_H;

	// Go in extended mode
	PCD8544_Write(PCD8544_COMMAND, PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION);

	// LCD bias select
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETBIAS | 0x4);

	// set VOP
	if (contrast > 0x7F) {
		contrast = 0x7F;
	}
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETVOP | contrast);

	// normal mode
	PCD8544_Write(PCD8544_COMMAND, PCD8544_FUNCTIONSET);

	// Set display to Normal
	PCD8544_Write(PCD8544_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

	//Set cursor to home position
	PCD8544_Home();

	//Normal display
	PCD8544_Write(PCD8544_COMMAND, PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

	//Clear display
	PCD8544_Clear(0);
}

void PCD8544_Write(PCD8544_WriteType_t cd, unsigned char data) {
	switch (cd) {
		//Send data to lcd's ram
		case PCD8544_DATA:
			//Set DC pin HIGH
		  DC_H;
			break;
		//Send command to lcd
		case PCD8544_COMMAND:
			//Set DC pin LOW
			DC_L;
			break;
		default: break;
	}
	//Send data
	//PCD8544_send(data);
	CS_L;
	SPI1->DR = data;
	while( SPI1->SR & SPI_FLAG_BSY ); // wait until SPI is not busy anymore
  while( !(SPI1->SR & SPI_FLAG_TXE) ); // wait until transmit complete
	CS_H;
}

void PCD8544_send(unsigned char data) {
	CS_L;
	SPI1->DR = data;
	while( SPI1->SR & SPI_FLAG_BSY ); // wait until SPI is not busy anymore
  while( !(SPI1->SR & SPI_FLAG_TXE)); // wait until transmit complete
	CS_H;
}

void PCD8544_Clear(uint8_t color) {
	unsigned int i;
	PCD8544_Home();
	for (i = 0; i < PCD8544_BUFFER_SIZE; i++) {
		PCD8544_Buffer[i] = color;
	}
	PCD8544_GotoXY(0, 0);
	PCD8544_UpdateArea(0, 0, PCD8544_WIDTH - 1, PCD8544_HEIGHT - 1);
	//PCD8544_Refresh();
}

void PCD8544_Home(void) {
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETXADDR | 0);
	PCD8544_Write(PCD8544_COMMAND, PCD8544_SETYADDR | 0);
}

void PCD8544_Refresh(void) {
	unsigned char i, j;
	for (i = 0; i < 6; i++) {
		//Not in range yet
		if (PCD8544_UpdateYmin > ((i + 1) * 8)) {
			continue;
		}
		//Over range, stop
		if ((i * 8) > PCD8544_UpdateYmax) {
			break;
		}

		PCD8544_Write(PCD8544_COMMAND, PCD8544_SETYADDR | i);
		PCD8544_Write(PCD8544_COMMAND, PCD8544_SETXADDR | PCD8544_UpdateXmin);

		for (j = PCD8544_UpdateXmin; j <= PCD8544_UpdateXmax; j++) {
			PCD8544_Write(PCD8544_DATA, PCD8544_Buffer[(i * PCD8544_WIDTH) + j]);
		}
	}

	PCD8544_UpdateXmin = PCD8544_WIDTH - 1;
	PCD8544_UpdateXmax = 0;
	PCD8544_UpdateYmin = PCD8544_HEIGHT - 1;
	PCD8544_UpdateYmax = 0;
}

void PCD8544_UpdateArea(unsigned char xMin, unsigned char yMin, unsigned char xMax, unsigned char yMax) {
	if (xMin < PCD8544_UpdateXmin) {
		PCD8544_UpdateXmin = xMin;
	}
	if (xMax > PCD8544_UpdateXmax) {
		PCD8544_UpdateXmax = xMax;
	}
	if (yMin < PCD8544_UpdateYmin) {
		PCD8544_UpdateYmin = yMin;
	}
	if (yMax > PCD8544_UpdateYmax) {
		PCD8544_UpdateYmax = yMax;
	}
}

void PCD8544_GotoXY(unsigned char x, unsigned char y) {
	PCD8544_x = x;
	PCD8544_y = y;
}


void PCD8544_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, PCD8544_Pixel_t color) {
	short dx, dy;
	short temp;

	if (x0 > x1) {
		temp = x1;
		x1 = x0;
		x0 = temp;
	}
	if (y0 > y1) {
		temp = y1;
		y1 = y0;
		y0 = temp;
	}

	dx = x1 - x0;
	dy = y1 - y0;

	if (dx == 0) {
		do {
			PCD8544_SetPixel(x0, y0, color);
			y0++;
		} while (y1 >= y0);
		return;
	}
	if (dy == 0) {
		do {
			PCD8544_SetPixel(x0, y0, color);
			x0++;
		} while (x1 >= x0);
		return;
	}

	/* Based on Bresenham's line algorithm  */
	if (dx > dy) {
		temp = 2 * dy - dx;
		while (x0 != x1) {
			PCD8544_SetPixel(x0, y0, color);
			x0++;
			if (temp > 0) {
				y0++;
				temp += 2 * dy - 2 * dx;
			} else {
				temp += 2 * dy;
			}
		}
		PCD8544_SetPixel(x0, y0, color);
	} else {
		temp = 2 * dx - dy;
		while (y0 != y1) {
			PCD8544_SetPixel(x0, y0, color);
			y0++;
			if (temp > 0) {
				x0++;
				temp += 2 * dy - 2 * dx;
			} else {
				temp += 2 * dy;
			}
		}
		PCD8544_SetPixel(x0, y0, color);
	}
}

void PCD8544_SetPixel(uint16_t x, uint16_t y, uint16_t pixel) {
	
	if ( x >= PCD8544_WIDTH ||	y >= PCD8544_HEIGHT	) return;	
	
	if (pixel) {
		PCD8544_Buffer[x + (y / 8) * PCD8544_WIDTH] |= 1 << (y % 8);
	} else {
		PCD8544_Buffer[x + (y / 8) * PCD8544_WIDTH] &= ~(1 << (y % 8));
	}
}

char PCD8544_GetPixel(uint16_t x, uint16_t y) {
	return PCD8544_Buffer[x + (y / 8) * PCD8544_WIDTH] >> (y % 8);
}






