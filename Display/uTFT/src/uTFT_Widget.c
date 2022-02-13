#include "uTFT_define.h"

/*
SSD1306_progress_bar(20, 0, 100, 0, 0 ,1);
SSD1306_progress_bar(20, 10, 100, 0, 1 ,1);
SSD1306_progress_bar(20, 20, 100, 0, 2 ,1);
*/
void SSD1306_progress_bar(uint16_t x, uint16_t y, uint16_t w, uint16_t persent, uint16_t pattern)
{
//	uTFT_DrawFilledRectangle(x, y, w, 8, 0 );
//	uTFT_DrawRectangle(x, y, w, 8, 1 );
//	
//	uint16_t i;
//	
//	if (pattern==1)
//	for(i=2;i<w;i+=2)
//	{
//		uTFT_SetPixel(x+i, y+2, 1);
//		uTFT_SetPixel(x+i, y+4, 1);
//		uTFT_SetPixel(x+i, y+6, 1);
//	}
//	if (pattern==2)
//	for(i=2;i<w;i+=2)
//	{
//		uTFT_SetPixel(x+i, y+2, 1);
//		uTFT_SetPixel(x+i+1, y+4, 1);
//		uTFT_SetPixel(x+i, y+6, 1);
//	}
//	
//	if(persent>100) persent=100;
//	uTFT_DrawFilledRectangle(x+2, y+2, ((w-4)*persent)/100, 4, 1);
}
