#include "uTFT_define.h"

//#include <math.h>       /* round, floor, ceil, trunc */



//Вертикальный градиент
void uTFT_Gradient_Vertical(uTFT_LCD_t* LCD, uint32_t x0, uint32_t y0, uint32_t w, uint32_t h, uint8_t otR, uint8_t otG ,uint8_t otB, uint8_t doR, uint8_t doG, uint8_t doB)
{
	uint8_t b[2][3];
  uint8_t a[3];
	 
	float faktor;
	uint8_t z;
	uint16_t bis; 
	uint16_t color;
	
	uint32_t x;
	
	bis = h - 1;
	
	b[0][0] = otR;
	b[0][1] = otG;
	b[0][2] = otB;
	
	b[1][0] = doR;
	b[1][1] = doG;
	b[1][2] = doB;

	for( x = 0; x < bis; x++ )
{
  faktor = (float)x / (float)bis;
	
	for( z = 0; z < 3;z++)
   a[z] = trunc((float)b[0][z] + (((float)b[1][z] - (float)b[0][z]) * faktor));
	
	color = RGB565(a[0],a[1],a[2]);
	
  uTFT_HLine(LCD, x+y0 , x0, x0 + w-1, color);
}	

}
