#include "myili9341.h"
#include "myili9341_bmp_ico.h"
//#include "res.h"

#include "lcd_global.h"

//extern unsigned short home[1024];
//extern unsigned short right[1024];

//extern const unsigned short bmp1[];
//extern BMP gif[];



//void TFT9341_GetPixelArray(uint16_t x, uint16_t y,uint16_t x1, uint16_t y1)
//{
//  	uint16_t dt;
//   	uint16_t i,imax;
//    if((x<0)||(y<0)||(x>=320)||(y>=240)) return;
//	
//    TFT_SetAddrWindow(x, y, x1, y1);
//    TFT9341_SendCommand(0x2E);
//	
//	dt = ADDR_DATA;
//	dt = ADDR_DATA;
//	imax=(x1-x)*(y1-y);
//	for(i=0;i<imax;i++)
//  {  
//	  dt = ADDR_DATA;
//	  //dt = dt << 8;
//	  TFT9341_Delay(1);
//	  dt |= ADDR_DATA << 8;
//	  tftPixelReadArray[i]=dt;
//	}
//}

//void TFT9341_Icon_Transparrent(uint16_t x, uint16_t y, uint16_t trColor)
//{

//	TFT9341_GetPixelArray(x, y,x+31,y+31);	
//  TFT_SetAddrWindow(x,y,x+31,y+31);
//	
//	uint16_t i,r;
//	
//	TFT9341_SendCommand(0x2C);
//	for(i=0;i<32*32;i++)
//	{
//	  if(right[i]==trColor)
//      r = tftPixelReadArray[i];
//		else
//			r = right[i];
//		
//    TFT9341_SendData(r >> 8);
//    TFT9341_SendData(r & 0xFF);
//	}
//}

////»конка с заливкой
//void TFT9341_Icon(uint16_t x, uint16_t y )
//{
//  TFT_SetAddrWindow(x,y,x+31,y+31);
//	uint16_t i;
//	TFT9341_SendCommand(0x2C);
//	for(i=0;i<32*32;i++)
//	{ 
//    TFT9341_SendData(right[i] >> 8);
//    TFT9341_SendData(right[i] & 0xFF);
//	}
//}	

//void TFT9341_BMP_Palette(uint16_t x, uint16_t y,  BMP res)
//{
//	static uint32_t i;
//	static uint32_t r;
//	TFT_SetAddrWindow(x,y,res.x+x-1,res.y+y-1);
//	TFT9341_SendCommand(0x2C);
//	
//	if (res.steam16 == NULL)
//	{
//	  for(i=0;i<res.x*res.y;i++)
//	  {
//	    r = convert24to16(res.Palette[res.steam8[i]]); 
//		  TFT9341_SendData(r >> 8);
//      TFT9341_SendData(r & 0xFF);
//	  }
//  }
//	else
//	 for(i=0;i<res.x*res.y;i++)
//	 {
//	  r = res.steam16[i];  
//		TFT9341_SendData(r >> 8);
//    TFT9341_SendData(r & 0xFF);	 
//	 }
//}

void TFT9341_GIF(uint16_t x, uint16_t y)
{
	static uint8_t i;

	for(i=0;i<17;i++)
	{
	 //TFT9341_BMP_Palette(0, 0,  gif[i]);
	 //osDelay(30);
	}
	

}
