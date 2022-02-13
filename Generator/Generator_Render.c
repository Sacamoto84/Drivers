#include "main.h"
#include "mysignal.h"

#include <math.h>

#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_Int.h"
#include "SEGGER_RTT.h"

void create_carriaer(uint8_t index, uint8_t channel);
void create_am_modulation(uint8_t index, uint8_t channel);
void create_fm_modulation(uint16_t channel);	

uint32_t req_to_creaete_fm_ch1;
uint32_t req_to_creaete_fm_ch2;

//Функция рендерит carrier по номеру типа волны
//10-Sin
//20-Triagle
//30-Rectagle
void create_carriaer(uint8_t index, uint8_t channel)
{
	uint16_t i;
	if (channel == 0)
	{
	//10-Sin
	if (index == 10){
	  //SEGGER_SYSVIEW_Print("Sinus"); 
	  for(i=0;i<1024;i++)   
	    buffer_carrier[i] = SINE_12b_1024[i];
	}
	 
	//1-Triagle
	if (index == 20)   {
		//SEGGER_SYSVIEW_Print("Triagle"); 
		for(i=0;i<1024;i++)   
		  buffer_carrier[i] = TRIAGLE_12b_1024[i];
	}

	//2-Rectagle	
	if (index == 30) 	{
		//SEGGER_SYSVIEW_Print("Rectagle");
	  for(i=0;i<256;i++)		 buffer_carrier[i] = 0;
		for(i=256;i<768;i++)	 buffer_carrier[i] = 4095;
	  for(i=768;i<1024;i++)  buffer_carrier[i] = 0; 
	}

	}
	else
	{
	 //10-Sin
   if (index == 10) 	 for(i=0;i<1024;i++)   buffer_carrier2[i] = SINE_12b_1024[i]; 
	 
	  //1-Triagle
	 if (index == 20)    for(i=0;i<1024;i++)   buffer_carrier2[i] = TRIAGLE_12b_1024[i];

	  //2-Rectagle	
	 if (index == 30) 	{
	  for(i=0;i<256;i++)		 buffer_carrier2[i] = 0;
		for(i=256;i<768;i++)	 buffer_carrier2[i] = 4095;
	  for(i=768;i<1024;i++)  buffer_carrier2[i] = 0; }
	}
}

void create_am_modulation(uint8_t index, uint8_t channel)
{
	uint16_t i;
	//SEGGER_SYSVIEW_Print("create_am_modulation");
  if (channel==0)
	{
		if (index == 10) { for(i=0;i<SAMPLES;i++) buffer_am[i] = CHIRP_1024[i]; }
		if (index == 20) { for(i=0;i<SAMPLES;i++) buffer_am[i] = TENS2_1024[i]; }
		if (index == 30) { for(i=0;i<SAMPLES;i++)	buffer_am[i] = TENS1_1024[i]; }
		if (index == 40) { for(i=0;i<SAMPLES;i++) buffer_am[i] = MY_1024[i];	  }
		if (index == 50) { for(i=0;i<SAMPLES;i++) buffer_am[i] = AmpALT_1024[i];}
		if (index == 60) { for(i=0;i<SAMPLES;i++) buffer_am[i] = AttALT_1024[i];}
		if (index == 70) { for(i=0;i<SAMPLES;i++) buffer_am[i] = Dnramp_1024[i];}
		if (index == 80) { for(i=0;i<SAMPLES;i++) buffer_am[i] = Ramp_1024[i];	}
		if (index == 90) { for(i=0;i<SAMPLES;i++) buffer_am[i] = Sine_1024[i];	}
  }
  else
	{
		if (index == 10) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = CHIRP_1024[i]; }
		if (index == 20) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = TENS2_1024[i]; }
		if (index == 30) { for(i=0;i<SAMPLES;i++)	buffer_am2[i] = TENS1_1024[i]; }
		if (index == 40) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = MY_1024[i];	   }
		if (index == 50) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = AmpALT_1024[i];}
		if (index == 60) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = AttALT_1024[i];}
		if (index == 70) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = Dnramp_1024[i];}
		if (index == 80) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = Ramp_1024[i];	 }
		if (index == 90) { for(i=0;i<SAMPLES;i++) buffer_am2[i] = Sine_1024[i];	 }
  }
}

void create_fm_modulation( uint16_t channel )
{
  int i;
	uint16_t x, y;
	const uint16_t *p;
	
	//SEGGER_SYSVIEW_Print("create_fm_modulation");
	if (channel==0)
	{
    x = FM_Base - FM1_Deviation;	
    y = FM1_Deviation*2;
    	
		switch (FM_Modulation_CH1)
    {
    	case 10 : p = &CHIRP_1024[0];  	break;	
			case 20 : p = &TENS2_1024[0];	  break;
			case 30 : p = &TENS1_1024[0]; 	break;
			case 40 : p = &MY_1024[0];    	break;
			case 50 : p = &AmpALT_1024[0];	break;
			case 60 : p = &AttALT_1024[0];	break;
			case 70 : p = &Dnramp_1024[0];	break;
			case 80 : p = &Ramp_1024[0];  	break;
			case 90 : p = &Sine_1024[0];	  break;	
    	default:	break;
    }

		for(i=0;i<512;i++)
    { 
	    buffer_fm[i] = x + (y	* *p/4095.0F);
	    p+=2;
		}	
		
  }
	else
	{
	  for(i=0;i<512;i++)
    { 
	    buffer_fm2[i] = FM2_Base + (FM2_Deviation* sin(6.28F/512.0F * i));
	  }	
	}
	
}
