
#include "main.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/*
#include "uTFT_Buffer.h"
#include "usbd_cdc_if.h"


USBD_StatusTypeDef res;
uint32_t USB_TX_Complete;
extern USBD_HandleTypeDef hUsbDeviceFS;

static USBD_CDC_HandleTypeDef *hcdc1;


volatile uint8_t buffer[5000];

void uTFT_USB_Update(uTFT_LCD_t* LCD)
{
	
	hcdc1 = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
	
	uint8_t count;
	count = (LCD->TFT_WIDTH * LCD->TFT_HEIGHT * 2)/4088;
	
	const uint8_t * p;
  	
	uint16_t y;
	uint16_t ii;
	ii = 0;
	for(y = 0; y<LCD->TFT_HEIGHT;y+=count)
	{
		p = LCD->buffer16 + LCD->TFT_WIDTH * count * ii++;
    
		buffer[0] = 1;
		buffer[1] = 2;
		buffer[2] = 3;
		buffer[3] = 4;
		buffer[4] = y;
   	buffer[5] = y + count - 1;	
		buffer[6] = LCD->TFT_WIDTH  & 0xFF;
   	buffer[7] = LCD->TFT_HEIGHT & 0xFF;
		for (uint16_t i = 8;i <= 4096; i++)
		{
				buffer[i] = *p++;
		}
		
	 CDC_Transmit_FS( (uint8_t*)buffer, 4096);
	 HAL_Delay(5);
		
  }	
}
 */

//void uTFT_USB_Update(uTFT_LCD_t* LCD)
//{
//	
//	hcdc1 = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
//	
//	const uint8_t * p;
//  	
//	uint16_t y;
//	uint16_t ii;
//	ii = 0;
//	for(y = 0; y<240;y+=15)
//	{
//		p = LCD->buffer16 + (4050/2) * ii++;

//		buffer[0] = y;
//   	buffer[1] = y + 14;	
//		
//		for (uint16_t i = 2;i <= 4050+2; i++)
//		{
//				buffer[i] = *p++;
//		}
//	 

//	
//	 CDC_Transmit_FS( (uint8_t*)buffer, 4096+4);
//	 HAL_Delay(6);
//		
//  }	
//}

