#ifndef GENERATOR_H
#define GENERATOR_H

#include "main.h"

#include "tim.h"

#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_Int.h"
#include "SEGGER_RTT.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "dac.h"

#include "IniFile.h"

extern DMA_HandleTypeDef hdma_dac1;

#define SAMPLES        1024
#define AM_MODSAMPLES  1024
#define FM_MODSAMPLES  512
#define OUTPUTSAMPLES  2048

#include "fatfs.h"

extern uint8_t retSD; /* Return value for SD */
extern FRESULT fr;

extern uint32_t OUTPUT[OUTPUTSAMPLES] RAM_16;

#pragma pack(push,1)
typedef struct {

	uint8_t CH;               //Номер канала

	uint8_t CH_EN;
	uint8_t AM_EN;
	uint8_t FM_EN;

	uint16_t Carrier_fr;       //Частота нечущей
	char Carrier_mod[20];     //Имя файла

	float AM_fr;              //Часта модуляции
	char AM_mod[20];          //Имя файла

	uint16_t FM_Base;
	float    FM_Dev;
	char FM_mod[20];       //Имя файла
	float FM_mod_fr;        //Часта модуляции

	//Буфферы
	uint16_t buffer_carrier[SAMPLES];
	uint16_t buffer_am[AM_MODSAMPLES];
	float    buffer_fm[FM_MODSAMPLES];

	uint32_t rC;		     //Частота несущей 1
	uint32_t angle_cr;

	uint32_t rAM;
	uint32_t angle_m;

	uint32_t rFM;
	uint32_t angle_fm;

	float Volume; //Громкость 0..1

	//Аккумуляторы
	uint32_t phase_accumulator_carrier;
	uint32_t phase_accumulator_mod;
	uint32_t phase_accumulator_fm;

} _structure_ch;
#pragma pack(pop)

class GENERATOR {
public:

	float convertHzToR(float hz) {
		hz = hz * 16384.0F / 3.798F * 2.0F;
		return hz;
	}
	float convertHzToR_FM(float hz) {
		hz = hz * 16384.0F;
		return hz;
	}
	float convertRToHz(float hz) {
		hz = hz / 16384.0F * 3.798F / 2.0F;
		return hz;
	}

    //dma_dac_it.c

    //Generator.c
	void Create_Carrier(_structure_ch *_CH);
	void Create_AM_Modulation(_structure_ch *_CH);
	void Create_FM_Modulation(_structure_ch *_CH);
	void RAW_to_Structure(_structure_ch *_CH);
	void Structure_to_RAW(_structure_ch *_CH);
	void read_save_file_to_structure_ch(void); //Чтение файла сохранения и запись его в структуру

	void Init(void);
	void syg_update(int Buffer_Current);


	void read_save_ini_to_structure_ch(void);

	void pause(void){
		HAL_TIM_Base_Stop(&htim6);
	}

	void start(void){
	  HAL_TIM_Base_Start(&htim6);
	}

    //Сброс фазы и аккумулятора
	void syg_reset_phase(void) {
		CH1.phase_accumulator_carrier = 0;
		CH2.phase_accumulator_carrier = 0;
		CH1.phase_accumulator_mod = 0;
		CH2.phase_accumulator_mod = 0;
		CH1.phase_accumulator_fm = 0;
		CH2.phase_accumulator_fm = 0;
	}





	_structure_ch CH1, CH2; //Используются для чтения настроек из файла save.dat

	UINT buffer_temp[1024+16]; //Сохраняются данные файлов

	int32_t DMA_Buffer_Current;

};

#endif
