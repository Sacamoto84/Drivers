#ifndef ARDUINOPIN_H_
#define ARDUINOPIN_H_

#include "gpio.h"

#define pinMode(x, y) {}

#define PINMAX 16

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
} arduinopin_t;

//Сюда заносим в ардуино стиле порты и используем гдето в программе
static inline arduinopin_t arduinoPins[PINMAX];

static inline void setArduinoPin(uint16_t index, GPIO_TypeDef * port, uint16_t pin)
{
  arduinopin_t t = { port, pin };
  arduinoPins[index] = t;
}

static inline void digitalWrite(uint16_t pin, bool state) {

	if (state != false) {
		arduinoPins[pin].port->BSRR = arduinoPins[pin].pin;
	} else {
		arduinoPins[pin].port->BSRR = (uint32_t) arduinoPins[pin].pin << 16u;
	}
}

//#define CREATE_SPRITE16(name, x , y) u16 name##_buffer16[x * y + 4];\
//TFT_LCD_t name={x , y, SPRITE_RAM, 16, NULL , NULL, 0, &name##_buffer16[0],};





/*
#define STEP 0

setArduinoPin(STEP, STEP_GPIO_Port, STEP_Pin);

digitalWrite(STEP, 0);
digitalWrite(STEP, 1);

*/

#endif /* ARDUINOPIN_H_ */
