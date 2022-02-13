#include "wavprocess.h"
#include "stm32f4xx_hal.h"  
#include "fatfs.h"
#include <stdlib.h>
#include "global_define.h"
#include "Generator_define.h"

#include "cmsis_os.h" 
#include "math.h" 

#include "ff.h"			/* Declarations of FatFs API */
#include "diskio.h"		/* Declarations of device I/O functions */
extern osMutexId sd_mutex_id; 

/* .WAV file format : ----------------------------------------------------------

 Endian      Offset      Length      Contents
  big         0           4 bytes     'RIFF'             -- 0x52494646
  little      4           4 bytes     <file length - 8>
  big         8           4 bytes     'WAVE'             -- 0x57415645

Next, the fmt chunk describes the sample format:

  big         12          4 bytes     'fmt '             -- 0x666D7420
  little      16          4 bytes     0x00000010         -- Length of the fmt data (16 bytes)
  little      20          2 bytes     0x0001             -- Format tag: 1 = PCM
  little      22          2 bytes     <channels>         -- Channels: 1 = mono, 2 = stereo
  little      24          4 bytes     <sample rate>      -- Samples per second: e.g., 22050
  little      28          4 bytes     <bytes/second>     -- sample rate * block align
  little      32          2 bytes     <block align>      -- channels * bits/sample / 8
  little      34          2 bytes     <bits/sample>      -- 8 or 16

Finally, the data chunk contains the sample data:

  big         36          4 bytes     'data'             -- 0x64617461
  little      40          4 bytes     <length of the data block>
  little      44          *           <sample data>
--------------------------------------------------------------------------------
*/

wp_format WAV_format;
FIL   WavFile;
WPRESULT WAV_res;

volatile int16_t req_slow_buffer_left [256];
volatile int16_t req_slow_buffer_right[256];

volatile int16_t req_fast_buffer_left [1024];
volatile int16_t req_fast_buffer_right[1024];

volatile int16_t req_fast_buffer_left_export [256];
volatile int16_t req_fast_buffer_right_export[256];

int16_t find_trigger(int16_t* data);//Поиск начала сигнала

uint16_t req_fast;     //Запрос на создание ексорта быстрого буфера
uint16_t req_fast_ok;  //Запрос на создание ексорта быстрого буфера выполнен
//********************************************************************************************
//function открывает файл и заполняет структуру формата данных format                       //
//argument указатель на файловую структуру, имя файла, указатель на структуру формата данных//
//result   файл открыт нормально, ошибка открытия                                           //
//********************************************************************************************
WPRESULT wp_open( FIL *file,  const char *FileName, wp_format *format )
{
  unsigned char res;                          //для хранения результата
  unsigned char buff[64];                     //буфер для заголовка файла
  unsigned int cnt;                          //количество прочитанных байт
  unsigned long i;                            //вспом. переменная
 
  res = f_open ( file, FileName, FA_READ );   //открыть файл для чтения
  
	if(res) return WP_ERROR_FILE;               //ошибка открытия

  res = f_read ( file, &buff[0], 64, &cnt );  //загрузить первые 64 байт
 
	if(res || cnt!=64) return WP_ERROR_FILE;    //ошибка чтения
 
  //байты 0..3 - должен быть маркер "RIFF"
  if(buff[0]!='R' || buff[1]!='I' || buff[2]!='F' || buff[3]!='F')     res = 2;
 
  //байты 8..11 - должен быть маркер "WAVE"
  if(buff[8]!='W' || buff[9]!='A' || buff[10]!='V' || buff[11]!='E')   res = 2;
 
  //байты 12..15 - должен быть маркер "fmt "
  if(buff[12]!='f' || buff[13]!='m' || buff[14]!='t' || buff[15]!=' ') res = 2;
 
  //байты 20,21  - 1 и 0
  if(buff[20]!=1 || buff[21]!=0) res = 2;
 
  //вычислить размер чанка "fmt "
  i = buff[16] | (buff[17]<<8) | (buff[18]<<16) | (buff[19]<<24);
 
  //после чанка "fmt " должен следовать чанк "data"
  if(buff[20+i]!='d' || buff[21+i]!='a' || buff[22+i]!='t' || buff[23+i]!='a') res = 2;
 
  if(res==2)
  {
    f_close(file);                          //закрыть файл
    return WP_ERROR_FILE;                   //ошибка формата файла
  }
 
  /*                    Загрузить формат данных                        */
  format->Channels       = buff[22];
  format->SamplesPerSec  = buff[24] | (buff[25]<<8) | (buff[26]<<16) | (buff[27]<<24);
  format->AvgBytesPerSec = buff[28] | (buff[29]<<8) | (buff[30]<<16) | (buff[31]<<24);
  format->BitsPerSample  = buff[34] | (buff[35]<<8);
  format->Size           = buff[24+i]|(buff[25+i]<<8)|(buff[26+i]<<16)|(buff[27+i]<<24);
 
  //файловый указатель на начало данных
  f_lseek(file,i+28);
 
	if (format->SamplesPerSec==22050)
   {
     TIM6->ARR = 1142;
   }
  else
   {
     TIM6->ARR = 1142/2; //44K
   }
	
  return WP_OK;
}



void play_wav(char* filename)
{
	int32_t r;
	FRESULT res;
  unsigned int br, btr;
  float left,right;
  uint16_t l1, r1;
	
	left  = 0;
	right = 0;
	
	static uint16_t trigger;
	
	//Открываем Wav файл
	WAV_res = wp_open( &WavFile,  filename , &WAV_format );
	if (WAV_res) //Ошибка WAV файла 
	{
		return;
	}
	for(;;)
	{

	  //Требуется прочесть новую порцию данных
		if (DMA_Buffer_Update)
		{
			left  = 0;
		  right = 0;
			
			l1 = 0;
			r1 = 0;
			
			DMA_Buffer_Update = 0;
			if (DMA_Buffer_Current)
			{	
				//Забираем мутекс
				osMutexWait(sd_mutex_id, osWaitForever);
		    res = f_read(&WavFile, &audio_buffer0[0] , 4096*2, &br);
        //Возвращаем мутекс
				osMutexRelease (sd_mutex_id);			
			}
			else
			{
				//Забираем мутекс
				osMutexWait(sd_mutex_id, osWaitForever);
			  res = f_read(&WavFile, &audio_buffer1[0] , 4096*2, &br);
				//Возвращаем мутекс
				osMutexRelease (sd_mutex_id);	
			}
			
			// Out of data or error or user button... Stop playback!
			if (br < btr || res != FR_OK){//|| BUTTON) {
					//StopAudio();
				
				
          osMutexWait(sd_mutex_id, osWaitForever);
					f_close(&WavFile);
          osMutexRelease (sd_mutex_id);
					// Return to previous function
					return;
				}
			
			if (DMA_Buffer_Current)
			for(int i=0;i < 4096;i++)
			{	
				r = audio_buffer0[i];	
				
				///////////////////////
				//Для медленного Roll    //
				if (i%2)                 //
			  {                        //
					left  +=abs(r);        // 
					if (i<2048) req_fast_buffer_left[l1++]= r;
				}
			  else     
				{            //
				 right +=abs(r);     //
					
				if (i<2048) req_fast_buffer_right[r1++]= r;	
				}
						///////////////////////
				
				if (r >= 0x8000)
					audio_buffer0[i] = 2047 - (0xFFFF - r)/16;
				else
					audio_buffer0[i] = 2047 + r/16;
			}
			else
			for(int i=0;i < 4096;i++)
			{	
				r = audio_buffer1[i];	
				
				///////////////////////
				//Для медленного Roll//
				if (i%2)             //
			   left  +=abs(r);     // 
				else                 //
				 right +=abs(r);     //
				///////////////////////
				
				if (r >= 0x8000)
					audio_buffer1[i] = 2047 - (0xFFFF - r)/16;
				else
					audio_buffer1[i] = 2047 + r/16;
			}	
			///////////////////////////////////////////////////////////////
			//Для медленного Roll                                        //
			for(int i=200;i>=1;i--)                                      //
			{                                                            //
				req_slow_buffer_left[i]=req_slow_buffer_left[i-1];         //
				req_slow_buffer_right[i]=req_slow_buffer_right[i-1];       //
			}                                                            //
			                                                             //
			req_slow_buffer_left[0]  = left /4096/256;                   //
			req_slow_buffer_right[0] = right/4096/256;                   //
			/////////////////////////////////////////////////////////////// 
			//Поиск начала сигнала в fast буффере
			
			if (req_fast)
			{
				req_fast = 0;
				//Ищем триггер на левом канале
				trigger = find_trigger(&req_fast_buffer_left[0]);
				if (trigger >= 0)  //Если триггер найден
				{
					//if(trigger<100)
						for(uint16_t i=0;i<320;i++)
						{
							req_fast_buffer_left_export[i]  = req_fast_buffer_left[i+trigger]/256/4;
							req_fast_buffer_right_export[i] = req_fast_buffer_right[i+trigger]/256/4;
						}
				}	
				else 
				{	
					//ищем триггер на правом канале
					trigger = find_trigger(&req_fast_buffer_right[0]);
					if (trigger >= 0)  //Если триггер найден
          {					
					  //if(trigger<100)
						  for(uint16_t i=0;i<320;i++)
						  {
								req_fast_buffer_left_export[i]  = req_fast_buffer_left[i+trigger]/256/4;
						    req_fast_buffer_right_export[i] = req_fast_buffer_right[i+trigger]/256/4;
				      }
				  }
					else
					{
						//триггера ненайденны, коприруем как есть
						 for(uint16_t i=0;i<320;i++)
						 {
						  req_fast_buffer_right_export[i] = req_fast_buffer_right[i]/256/4;
							req_fast_buffer_left_export[i]  = req_fast_buffer_left[i]/256/4;
						 }
					}
				
				}
				req_fast_ok = 1; //Готово, можно визуализировать
			}	
		}
		else
		  osThreadYield(); //Нет требования переключения буффера
	}	
}

int16_t find_trigger(int16_t* data)
{
	uint16_t result;
	
	int16_t* r;
	int16_t r_last;
	
	r = data;
	r_last = *r;
	for(uint16_t i=0;i<200;i++)
	{	
		r++;
		if ((*r > 0) && ( r_last < 0)) return i;
      			
		r_last = *r;		
	}
	
	
	return -1;
}










