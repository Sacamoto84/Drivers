#include "stm32f1xx_hal.h"
#include "global.h"
#include <stdlib.h>
#include <stdio.h> /* for printf */

#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

/* USER CODE BEGIN 0 */
#include "global.h"
#include "usart.h"

static void UART_Decode(void); 

static char  decoder_index_dec;
static float decoder_value_dec;

static char decoder_float_value; //������� ������� ��������

static char temp;
static char buffer[2];  //������
static char buffer1[8]; //��������

static unsigned char cycleBuf[256];
static unsigned char tail = 0;   //��������� ������ �������
static unsigned char head = 0;   //��������� ������ �������
static unsigned char count = 0;  //������� ��������

static char osSignalSet_Decode; //������ ������ ����������� ��������� ������
static char array[16] = "s";

//������� �������
static void FlushBuf(void)
{
  tail = 0;
  head = 0;
  count = 0;
}

//�������� ������ � ������
static void PutChar(unsigned char sym)
{
  //if (count < 256){                //���� � ������� ���� �����
      cycleBuf[tail] = sym;             //�������� � ���� ������
      count++;                          //��������� �������� ��������
      tail++;                           //� ������ ������ �������
      //if (txBufTail == SIZE_BUF) txBufTail = 0;
  //  }
}

//����� ������ �� �������
static unsigned char GetChar(void)
{
   unsigned char sym = 0;
   if (count > 0){                       //���� ����� �� ������
      sym = cycleBuf[head];              //��������� ������ �� �������
      count--;                           //��������� ������� ��������
      head++;                            //��������� ������� ������ ������
   }
   return sym;
}


void USART1_IRQHandler(void)
{
	SEGGER_SYSVIEW_RecordEnterISR();
	uint8_t USART_data;
	
 //��� ������ �������� � ������ ������
  if (USART1->SR & USART_SR_RXNE) {
		USART_data = USART1->DR; 
    PutChar(USART_data);//�������� ����
		//array[0] = USART_data;
		//SEGGER_SYSVIEW_Print(array);
		if(USART_data == 'C') FlushBuf();
		if(USART_data == 0x24) //$
			UART_Decode();//���� �������� ����� �������� ������ ������ �������������
  }	
	HAL_UART_IRQHandler(&huart1);
	SEGGER_SYSVIEW_RecordExitISR();
}

//����� ���������� ������ �� Blynk � Virtuino
static void UART_Decode (void) {

	  static char i;
	  
	  for(i=0;i<7;i++) buffer1[i] = 0;
	
    //������ ������ ������ �� �������
     if (GetChar()=='!') //������� ��� ������ ������
		 { 
			 temp = GetChar();
			 buffer[0] = GetChar();
			 decoder_index_dec = buffer[0]-0x30;//atoi(buffer[0]); //�������� ������ ������ ����� 00.31
			 
			 if (GetChar() != '=') goto rel1; //������ ���� �����
			 // = ���� ������ �������� ��� ����� ? ���  ������� ��������
			   temp = GetChar();
				 if (temp == '?') {goto rel1;}// ����� �������� �������� �� ������� decoder_index_dec
				 else  {i=0; goto rel2;}
			 
			 for(i=0;i < 8 ;i++)
			 {
				  temp = GetChar();
				  if (temp == '$') break;
				  rel2: buffer1[i] = temp;
			 }
			 buffer1[i+1] = 0; //����� ������

       decoder_value_dec =atof(buffer1);
			 vmemory[decoder_index_dec] = decoder_value_dec; //������ � �����
			 		 
		 }
		 else 
	   {	
		   rel1:	 
			 //temp = GetChar();
		   while(GetChar() != '$');  //������ ��� ��������� �� ������ ����� ����� ��������� �����
		 }
}