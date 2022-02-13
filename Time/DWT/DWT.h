/*
 * DWT.h
 *
 *  Created on: 4 февр. 2021 г.
 *      Author: Ivan
 */

#ifndef DWT_H_
#define DWT_H_

#include "main.h"

#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC

void delay_micros(uint32_t us);
void DWT_Init(void);

#endif /* DWT_H_ */
