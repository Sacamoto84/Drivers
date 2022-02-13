/**
  ******************************************************************************
  * @file    wavprocess.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    14-January-2013
  * @brief   This file includes WAV Processing layer headers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WAVPROCESS_H_
#define __WAVPROCESS_H_

#ifdef __cplusplus
 extern "C" {
#endif

///////* Includes ------------------------------------------------------------------*/
//////#include "stm32f4xx_hal.h"  
////////#include "fatfs.h"
//////#include <stdlib.h>

///////* Error Identification structure */
//////#define VALID_WAVE_FILE                   0
//////#define INVALID_RIFF_ID                   1
//////#define INVALID_WAVE_FORMAT               2
//////#define INVALID_FORMAT_CHUNK_ID           3
//////#define UNSUPPORTED_FORMAT_TAG            4
//////#define UNSUPPORTED_NUM_CHANNELS          5
//////#define UNSUPPORTED_SAMPLE_RATE           6
//////#define UNSUPPORTED_BITS_PER_SAMPLE       7
//////#define INVALID_DATA_CHUNK_ID             8
//////#define UNSUPPORTED_EXTRA_FORMAT_BYTES    9
//////#define INVALID_FACT_CHUNK_ID            10

//////#define LITTLE_ENDIAN                     0
//////#define BIG_ENDIAN                        1

///////* Audio Parsing Constants */
//////#define  CHUNK_ID             0x52494646  /* correspond to the letters 'RIFF' */
//////#define  FILE_FORMAT          0x57415645  /* correspond to the letters 'WAVE' */
//////#define  FORMAT_ID            0x666D7420  /* correspond to the letters 'fmt ' */
//////#define  DATA_ID              0x64617461  /* correspond to the letters 'data' */
//////#define  FACT_ID              0x66616374  /* correspond to the letters 'fact' */
//////#define  WAVE_FORMAT_PCM      0x01
//////#define  FORMAT_CHUNK_SIZE    0x10
//////#define  BITS_PER_SAMPLE_8    8
//////#define  BITS_PER_SAMPLE_16   16
////// 
///////* Audio file information structure */
//////typedef struct
//////{
//////  uint32_t  SampleRate;        /* Audio sampling frequency */
//////  uint32_t  NumChannels;       /* Number of channels: 1:Mono or 2:Stereo */
//////  uint32_t  BitsPerSample;     /* Number of bits per sample (16, 24 or 32) */
//////  uint32_t  AudioLength;       /* Total length of useful audio data (payload) */
//////  uint32_t  AudioStartAddr;     /* Relative start address of audio payload */
//////  uint32_t  RIFFchunksize;     /* The file header chunk size */
//////  uint16_t  FormatTag;         /* Audio file format: PCM = 1 */
//////  uint32_t  ByteRate;          /* Number of bytes per second  (sample rate * block align)  */
//////  uint16_t  BlockAlign;        /* channels * bits/sample / 8 */
//////  uint32_t  DataSize;          /* Audio data size */
//////}WAVE_FormatTypeDef;   
//////   
//////typedef struct
//////{
//////  uint8_t  Channels;          //количество каналов                                             /
//////  uint16_t SamplesPerSec;     //частота дискретизации аудиосигнала, Гц
//////  uint8_t  BitsPerSample;     //разрядность данных (8,16)
//////  uint32_t Size;              //количество полезных данных, байт  
//////  uint16_t AvgBytesPerSec;    //количество байт в секунду (поток данных)
//////} wp_format;

///////*                значения, которые могут возвращать функции                                 */
//////typedef enum
//////{
//////  WP_OK = 0,                  //файл успешно воспроизведен
//////  WP_ERROR_FILE,              //файл не открыт (не найден, не верный формат и т.п.)
//////  WP_ERROR_READ_FILE,         //возникла ошибка чтения при воспроизведении открытого файла
//////  WP_STOP,                    //воспроизведение было остановлено программно, файл закрыт
//////  WP_PAUSE,                   //пауза воспроизведения
//////  WP_PLAY,                    //возобновить воспроизведение
//////  WP_NONE,                    //
//////  WP_ERROR                    //просто ошибка
//////}WPRESULT;

//////extern WAVE_FormatTypeDef WaveFormatStruct;
//////extern wp_format WAV_format;
//////extern FIL   WavFile;
//////extern WPRESULT WAV_res; //Результат открытия файла
///////** @defgroup WavProcess_Exported_Functions
//////  * @{
//////  */
//////   
//////WPRESULT wp_open( FIL *file,  const char *FileName, wp_format *format );

////////uint32_t WavProcess_DecodeData(__IO int16_t* OutPutBuffer,uint32_t NbSamples,void * none);
////////uint32_t WavProcess_GetStreamLenght(uint32_t fLength);
////////uint32_t WavProcess_GetElapsedTime(uint32_t CurPos);
////////uint32_t WavProcess_StreamForward(void);
////////uint32_t WavProcess_StreamBackward(void);
////////uint32_t WavProcess_StopMode(void);
////////uint32_t WavProcess_GetSamplingRate(void);

////////uint32_t WavProcess_EncoderInit(uint32_t Freq, uint8_t* pHeader);
////////uint32_t WavProcess_EncoderDeInit(void);
////////uint32_t WavProcess_EncodeData(int8_t *pBufIn, int8_t *pBufOut, uint32_t* nEnc, void* pOpt);
////////uint32_t WavProcess_EncGetSampleRate (void);
////////uint32_t WavProcess_EncGetStreamLength (void);
////////uint32_t WavProcess_EncGetElapsedTime (void);

//////extern volatile int16_t req_slow_buffer_left [256];
//////extern volatile int16_t req_slow_buffer_right[256];

//////extern volatile int16_t req_fast_buffer_left [1024];
//////extern volatile int16_t req_fast_buffer_right[1024];

//////extern volatile int16_t req_fast_buffer_left_export [256];
//////extern volatile int16_t req_fast_buffer_right_export[256];

//////extern uint16_t req_fast;
//////extern uint16_t req_fast_ok;
#ifdef __cplusplus
}
#endif

#endif /* __WAVPROCESS_H_ */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
