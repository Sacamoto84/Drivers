#ifndef CIRCLEBUFFER_H
#define CIRCLEBUFFER_H

#include "main.h"

template <int SIZE_BUF>
class CIRCLE_BUFFER {
public:

	//Очистка буффера
	void FlushBuf(void) {
		tail = 0;
		head = 0;
		count = 0;
	}

	//Взять символ из буффера
	unsigned char GetChar(void) {
		unsigned char sym = 0;
		if (count > 0) {                            //если буфер не пустой
			sym = cycleBuf[head];               //считываем символ из буфера
			count--;                                //уменьшаем счетчик символов
			head++;                        //инкрементируем индекс головы буфера
			if (head == SIZE_BUF)
				head = 0;
		}
		return sym;
	}

	//Положить символ в буффер
	void PutChar(unsigned char sym)
	{
	  if (count < SIZE_BUF){                //если в буфере еще есть место
	      cycleBuf[tail] = sym;             //помещаем в него символ
	      count++;                          //инкрементируем счетчик символов
	      tail++;                           //и индекс хвоста буфера
	      if (tail == SIZE_BUF) tail = 0;
	    }
	}

	//Возвращает количество байт в буффере
	uint16_t available(void)
	{
		return count;
	}

	//Кольцевой входной буффер
	unsigned char cycleBuf[SIZE_BUF]; // __attribute__((section(".my_ccmram"))); //Кольцевой приемный буффер 8K

	uint16_t buffer_size()	{return SIZE_BUF;}

private:
	uint16_t tail = 0;                //Указатель хвоста буффера
	uint16_t head = 0;                //Указатель начала буффера
	uint16_t count = 0;               //Счетчик символов
};

#endif
