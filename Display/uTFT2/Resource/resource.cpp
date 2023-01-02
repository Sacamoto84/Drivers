#include "resource.h"

//Для ресурсов
static uint32_t resurce_count;
static uint32_t resurce_start_adress; //Начало ресурсов
//Bitmap   resurce_bmp;
static uint32_t *resurce_p;

void setResStartAdress(uint32_t i) {
  resurce_start_adress = i;
  resurce_p = (uint32_t *)resurce_start_adress;
  resurce_count = *resurce_p;       //Получить количество записей
}

//Получить начало ресурсов
uint8_t * getResAdressFontID(uint32_t id) {
	uint32_t * offset;
	offset = (uint32_t *)(resurce_start_adress + 4 + (16*id) + 12);
	uint8_t * adress;
	adress = (uint8_t *)(resurce_start_adress + *offset);
	    return adress;
}

//Получить дескриптов картинки из ресурсов 16 и 32 бит
Bitmap getResBitmapID(int8_t id) {
    Bitmap bmp = {0};
    if (id == -1)
    {
      return bmp;
    }
    uint32_t *p;
    p = (uint32_t *)(resurce_start_adress + 4 + (16*id));
    bmp.W = (uint16_t)*p++;
    bmp.H = (uint16_t)*p++;
    bmp.bit =*p++;
    uint32_t * offset;
    offset = (uint32_t *)(resurce_start_adress + 4 + (16*id) + 12);
    bmp.data = (uint32_t *)(resurce_start_adress + *offset);
  	return bmp;
}
