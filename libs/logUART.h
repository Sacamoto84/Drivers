#ifndef UTILS_LOGUART_H_
#define UTILS_LOGUART_H_

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#define COLOR_WARNING "\33[38;05;11m"
#define COLOR_ERROR   "\33[38;05;9m"
#define COLOR_RESET   "\33[0m"

class classLog {
public:

	void init(UART_HandleTypeDef *_huart) {	huart = _huart; }
	void init(UART_HandleTypeDef *_huart, int *_dma_completed) { huart = _huart; dma_completed = _dma_completed; }

	void clear(void) {	print("\33[1m\n"); }      //Очистка терминала
	void color(int color = 15);                //Задать цвет текста
	void bgcolor(int color = 0);               //Задать цвет фона
	void reset(void)   { print("\33[0m");};
	void resetln(void) { print("\33[0m\n");};

	void print(char const *format);

	void setBold(void);
	void setItalic(void);
	void setUnderline(void);
	void setRevers(void);
	void setFlash(void);

	void warning(char const *const format);
	void error(char const *const format);
	void info(char const *const format);
	void successful(char const *const format);

	template<typename ... Args> void print(char const *const format, Args const &... args) noexcept {
		if (useDMA == false) {
			sprintf(str, format, args ...);
			HAL_UART_Transmit(huart, (const uint8_t*) str, strlen(str), 1000);
		} else {
			waitComplete();
			sprintf(str, format, args ...);
			DMASend();
		}
	}

	template<typename ... Args>
	void println(char const *const format, Args const &... args) noexcept {
		if (useDMA == false) {
			sprintf(str, format, args ...);
			strcat(str, "\n");
			HAL_UART_Transmit(huart, (const uint8_t*) str, strlen(str), 1000);
		} else {
			waitComplete();
			sprintf(str, format, args ...);
			strcat(str, "\n");
			DMASend();
		}
	}

	//Вывести строку с нужным цветом
	void colorString(int color, char const *const format);
	void colorString(int color, int bgcolor, char const *const format);
	void colorStringln(int color, char const *const format);
	void colorStringln(int color, int bgcolor, char const *const format);

	template<typename ... Args>
	void warning(char const *const format, Args const &... args) noexcept {
		if (useDMA) waitComplete();
		sprintf(str, format, args ...);
		colorStringln(11, str);
	}

	template<typename ... Args>
	void error(char const *const format, Args const &... args) noexcept {
		if (useDMA) waitComplete();
		sprintf(str, format, args ...);
		colorStringln(9, str);
	}

	template<typename ... Args>
	void info(char const *const format, Args const &... args) noexcept {
		if (useDMA) waitComplete();
		sprintf(str, format, args ...);
		colorStringln(45, str);
	}

	template<typename ... Args>
	void successful(char const *const format, Args const &... args) noexcept {
		if (useDMA) waitComplete();
		sprintf(str, format, args ...);
		colorStringln(10, str);
	}

	//Ждать окончание передачи, для DMA
	inline void waitComplete(void) {
		int t = HAL_GetTick();
		while (*dma_completed == false) { if ((HAL_GetTick() - t) > 100) break; }
		*dma_completed = 0;
	}

	inline void DMASend(void) {
		int size = strlen(str);
		if (size){
		  //HAL_UART_Transmit_DMA(huart, (const uint8_t*) str, size);
			HAL_UART_Transmit_IT(huart, (const uint8_t*) str, size);
		}
		else
		  *dma_completed = 1;
	}

	int *dma_completed;
	char str[256];
	bool useDMA = false;
	UART_HandleTypeDef *huart;

};

#endif /* UTILS_LOGUART_H_ */
