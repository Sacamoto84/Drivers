#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "cmsis_os.h" 
#include "res.h"
#include "lcd_global.h"

#define XA_Output GPIOB->MODER |= 1
#define XB_Output GPIOB->MODER |= (1<<2)
#define YA_Output GPIOE->MODER |= (1<<22)
#define YB_Output GPIOE->MODER |= (1<<24)

#define XA_Input  GPIOB->MODER &= ~1
#define XB_Input  GPIOB->MODER &= ~(1<<2)
#define YA_Input  GPIOE->MODER &= ~(1<<22)
#define YB_Input  GPIOE->MODER &= ~(1<<24)

#define XA_0 GPIOB->BSRR = XA_Pin<<8
#define XB_0 GPIOB->BSRR = XB_Pin<<8
#define YA_0 GPIOE->BSRR = YA_Pin<<8
#define YB_0 GPIOE->BSRR = YB_Pin<<8

#define XA_1 GPIOB->BSRR = XA_Pin
#define XB_1 GPIOB->BSRR = XB_Pin
#define YA_1 GPIOE->BSRR = YA_Pin
#define YB_1 GPIOE->BSRR = YB_Pin

#define YA_PP_ON GPIOE->PUPDR |= 1<<22
#define YB_PP_ON GPIOE->PUPDR |= 1<<24

#define YA_PP_OFF GPIOE->PUPDR &= ~(1<<22)
#define YB_PP_OFF GPIOE->PUPDR &= ~(1<<24)

void ScanTouch(void);
void GUI_Update_NU(void); //Обновление признаков необходимости перерисовки обьекта

extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

uint16_t TouchState;
uint16_t coord_X, coord_Y;

short TS_MINX=330;
short TS_MINY=400;
short TS_MAXX=3040;
short TS_MAXY=2900;

_TS TS;

//Кнопки создаваемые на Page 0
extern BUTTON Button_Page_0[2];
extern BUTTON Button_Page_1[2];

//////////////////////////////////////////////////////////
//                Поток обновления экрана               //                
//////////////////////////////////////////////////////////
void TOUCH (void const *argument) {	
  while (1) {             
    osDelay(100);
    ScanTouch(); //Возвращает координату и признак нажатия
		GUI_Update_NU();
  }
}
//////////////////////////////////////////////////////////


//short TS_Press;
//short TS_Releases;
//short TS_X, TS_Y;
//////////////////////////////////////////////////////////
//Получение текущего сотояния нажатия
//////////////////////////////////////////////////////////
void ScanTouch(void)
{
				YA_Output;YB_Output;
	      YA_1;YB_0;
				XA_Input;XB_Input;
	      
			  osDelay(10);	// Выдержка на переходный процесс.
	
			  HAL_ADC_Start(&hadc2); //XA
	  	  HAL_ADC_PollForConversion(&hadc2, 100);
        coord_Y = HAL_ADC_GetValue(&hadc2);
			  HAL_ADC_Stop(&hadc2);
	      //GPIOB->PUPDR &= ~10; //Pull-Down
	      YA_0;YB_0;
	
		  	XA_Output;XB_Output;
		  	XA_1;XB_0;
	      YA_Input;YB_Input;
				
	      osDelay(10);	// Выдержка на переходный процесс.
	      // ХА на Vcc  XB на GND остальные в HiZ нюхают АЦП	
			  HAL_ADC_Start(&hadc3); //YA
	  	  HAL_ADC_PollForConversion(&hadc3, 100);
        coord_X = HAL_ADC_GetValue(&hadc3);
			  HAL_ADC_Stop(&hadc3);
				//GPIOE->PUPDR &= ~ ((1<<25) | (1<<23));
				XA_0;XB_0;
				
				//uint16_t temp;
				float r;
				r = (TS_MAXY-TS_MINY)/240.0;
				if ((coord_Y>TS_MINY) && (TS.Press==0))
				{
				  r =(coord_Y-TS_MINY)/r;
				  TS.Y = (unsigned short)r;
					if (TS.Y>240) TS.Y = 240;
					if (TS.Y<0) TS.Y = 0;
				  r = (TS_MAXX-TS_MINX)/320.0;
				  r =(coord_X-TS_MINX)/r;
				  TS.X= 320-(unsigned short)r;
					if (TS.X>320)TS.X = 320;
					if (TS.X<0) TS.X = 0;
				  TS.Press = 1;
				}	
				if ((coord_Y<TS_MINY) && TS.Press) TS.Press=0;
				
}
//////////////////////////////////////////////////////////

extern uint16_t PAGE; //Номер текущего окна

void GUI_Update_NU(void)
{
//Сканирует кнопки по их кординатам
if (TS.Press) //если произошло нажатие на экран
{
	//проверим по совпадению с координатаи кнопок
	if (PAGE == 0) //Если выбранн экран 0
	{		
	 for(uint16_t i=0;i<2;i++)
	 {
		  if ((TS.X>Button_Page_0[i].x1) && (TS.X<Button_Page_0[i].x2)&&(TS.Y>Button_Page_0[i].y1) && (TS.Y<Button_Page_0[i].y2)) //совпадение с кнопкой
		  { //Запуск функций обработки нажатия
			  Button_Page_0[i].EventPress(); //Запуск обрботчика нажатий
		    Button_Page_0[i].NU = 1; //	Перерисовывать необходимо
		  }	
		
	  }	
  }
		
	if (PAGE == 1) //Если выбранн экран 1
	{		
	 for(uint16_t i=0;i<2;i++)
	 {
		  if ((TS.X>Button_Page_1[i].x1) && (TS.X<Button_Page_1[i].x2)&&(TS.Y>Button_Page_1[i].y1) && (TS.Y<Button_Page_1[i].y2)) //совпадение с кнопкой
		  { //Запуск функций обработки нажатия
			  //Button_Page_1[i].EventPress(); //Запуск обрботчика нажатий
		    Button_Page_1[i].NU = 1; //	Перерисовывать необходимо
		  }	
		
	  }	
  }
	
		
		
		
		
		

		
		
		
	}//if (TS.Press) //если произошло нажатие на экран
}



	



