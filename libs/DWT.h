/*
 * DWT.h
 *
 *  Created on: 4 февр. 2021 г.
 *      Author: Ivan
 */

#ifndef DWT_H_
#define DWT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC


static inline void DWT_Init(void)
{
    SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
    DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик
}

static inline void delay_us(uint32_t us)
{
    uint32_t us_count_tic =  us * (SystemCoreClock / 1000000); // получаем кол-во тактов за 1 мкс и умножаем на наше значение
    DWT->CYCCNT = 0U; // обнуляем счётчик
    while(DWT->CYCCNT < us_count_tic);
}


//DWT_CYCCNT = 0;// обнуляем счётчик
// здесь кусок измеряемого участка программы
//count_tic = DWT_CYCCNT; // кол-во тактов

#ifdef __cplusplus
}
#endif

#endif /* DWT_H_ */
