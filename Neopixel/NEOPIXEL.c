#include "NEOPIXEL.h"

uint8_t NeoPixelDataToSend[NEOPIXEL_LED*24+NEOPIXEL_RESET]={0};

void NEOPIXEL_Init(void)
{
    HAL_SPI_Transmit_DMA(&NEOPIXEL_SPI, NeoPixelDataToSend, NEOPIXEL_LED*24+NEOPIXEL_RESET);
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
    DMA1_Channel3->CCR &= ~DMA_CCR_TCIE;
	DMA1_Channel3->CCR &= ~DMA_CCR_HTIE;
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void NeoPiexelRGB(uint8_t LED, uint8_t R, uint8_t G, uint8_t B)
{
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24]    = ((G>>7)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+1]  = ((G>>6)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+2]  = ((G>>5)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+3]  = ((G>>4)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+4]  = ((G>>3)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+5]  = ((G>>2)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+6]  = ((G>>1)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+7]  = ((G>>0)&1)? 0xE0    : 0xC0;
	
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+8]   = ((R>>7)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+9]   = ((R>>6)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+10]  = ((R>>5)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+11]  = ((R>>4)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+12]  = ((R>>3)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+13]  = ((R>>2)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+14]  = ((R>>1)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+15]  = ((R>>0)&1)? 0xE0    : 0xC0;
	
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+16]  = ((B>>7)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+17]  = ((B>>6)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+18]  = ((B>>5)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+19]  = ((B>>4)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+20]  = ((B>>3)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+21]  = ((B>>2)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+22]  = ((B>>1)&1)? 0xE0    : 0xC0;
	NeoPixelDataToSend[NEOPIXEL_RESET + LED*24+23]  = ((B>>0)&1)? 0xE0    : 0xC0;
}


