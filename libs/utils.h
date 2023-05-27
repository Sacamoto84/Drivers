/*
 * utils.h
 *
 *  Created on: May 27, 2023
 *      Author: Ivan
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "Arduino.h"

char* mUtoa(uint32_t value, char *buffer, bool clear = 1);
char* mLtoa(int32_t value, char *buffer, bool clear = 1);
char* mFtoa(double value, int8_t decimals, char *buffer);

#endif /* UTILS_H_ */
