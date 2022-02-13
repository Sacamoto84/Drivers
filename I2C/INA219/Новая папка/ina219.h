#ifndef INA219_H
#define INA219_H

/* C++ detection */
#ifdef __cplusplus
extern C {
#endif

#include "i2c.h"
#include "gpio.h"
#include "main.h"
#include <math.h>

void INA219_I2C_Vbus(void);
void INA219_I2C_Ibus(void);

	/* C++ detection */
#ifdef __cplusplus
}
#endif

 
#endif
