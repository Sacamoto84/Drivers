/*
 * tm1637.h
 *
 *  Created on: 4 февр. 2021 г.
 *      Author: Ivan
 */

#ifndef TM1637_H_
#define TM1637_H_

#include "main.h"

#include <stdlib.h>
#include <stdio.h> /* for printf */
#include <string.h> /* for printf */

#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC

#define TM1637_DIO_LOW()   TM1637_DIO_GPIO_Port->BSRR = (uint32_t)TM1637_DIO_Pin << 16U;
#define TM1637_DIO_HIGH()  TM1637_DIO_GPIO_Port->BSRR = TM1637_DIO_Pin

#define TM1637_CLK_LOW()   TM1637_CLK_GPIO_Port->BSRR = (uint32_t)TM1637_CLK_Pin << 16U;
#define TM1637_CLK_HIGH()  TM1637_CLK_GPIO_Port->BSRR = TM1637_CLK_Pin

//#define TM1637_DIO_LOW()   HAL_GPIO_WritePin(TM1637_DIO_GPIO_Port, TM1637_DIO_Pin, GPIO_PIN_RESET)
//#define TM1637_DIO_HIGH()  HAL_GPIO_WritePin(TM1637_DIO_GPIO_Port, TM1637_DIO_Pin, GPIO_PIN_SET)

#define TM1637_DIO_READ()  HAL_GPIO_ReadPin(TM1637_DIO_GPIO_Port, TM1637_DIO_Pin)

//#define TM1637_CLK_LOW()   HAL_GPIO_WritePin(TM1637_CLK_GPIO_Port, TM1637_CLK_Pin, GPIO_PIN_RESET)
//#define TM1637_CLK_HIGH()  HAL_GPIO_WritePin(TM1637_CLK_GPIO_Port, TM1637_CLK_Pin, GPIO_PIN_SET)

#define TM1637_PIN_DELAY() delay_micros(1);

// Commands
#define TM1637_CMD_DATA                 0x40 //!< Display data command
#define TM1637_CMD_CTRL                 0x80 //!< Display control command
#define TM1637_CMD_ADDR                 0xc0 //!< Display address command

// Data command bits
#define TM1637_DATA_WRITE               0x00 //!< Write data
#define TM1637_DATA_READ_KEYS           0x02 //!< Read keys
#define TM1637_DATA_AUTO_INC_ADDR       0x00 //!< Auto increment address
#define TM1637_DATA_FIXED_ADDR          0x04 //!< Fixed address

// Control command bits
#define TM1637_CTRL_PULSE_1_16          0x00 //!< Pulse width 1/16
#define TM1637_CTRL_PULSE_2_16          0x01 //!< Pulse width 2/16
#define TM1637_CTRL_PULSE_4_16          0x02 //!< Pulse width 4/16
#define TM1637_CTRL_PULSE_10_16         0x03 //!< Pulse width 10/16
#define TM1637_CTRL_PULSE_11_16         0x04 //!< Pulse width 11/16
#define TM1637_CTRL_PULSE_12_16         0x05 //!< Pulse width 12/16
#define TM1637_CTRL_PULSE_13_16         0x06 //!< Pulse width 13/16
#define TM1637_CTRL_PULSE_14_16         0x07 //!< Pulse width 14/16
#define TM1637_CTRL_DISPLAY_OFF         0x00 //!< Display off
#define TM1637_CTRL_DISPLAY_ON          0x08 //!< Display on

#define TM1637_NUM_GRIDS                4    //!< Number of grid registers

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

static const uint8_t minusSegments = 0b01000000;

static const int8_t tm1637_symbols[] = {
                // XGFEDCBA
        0x3f, // 0b00111111,    // 0
        0x06, // 0b00000110,    // 1
        0x5b, // 0b01011011,    // 2
        0x4f, // 0b01001111,    // 3
        0x66, // 0b01100110,    // 4
        0x6d, // 0b01101101,    // 5
        0x7d, // 0b01111101,    // 6
        0x07, // 0b00000111,    // 7
        0x7f, // 0b01111111,    // 8
        0x6f, // 0b01101111,    // 9
        0x77, // 0b01110111,    // A
        0x7c, // 0b01111100,    // b
        0x39, // 0b00111001,    // C
        0x5e, // 0b01011110,    // d
        0x79, // 0b01111001,    // E
        0x71, // 0b01110001     // F
        0x40, // 0b01000000     // minus sign
};

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

// Create an array that sets individual segments per digit to display the word "dOnE"
const uint8_t done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

// Create °C symbol
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,	// °
  SEG_A | SEG_D | SEG_E | SEG_F		// C
};



class TM1637 {
public:
	uint8_t buf[4] = {0, 0, 0, 0};

	TM1637();
	~TM1637();

	void start();
	void stop();

	void writeData(uint8_t address, uint8_t data);
	void writeData(uint8_t address, const uint8_t *buf, uint8_t len);
	void writeCommand(uint8_t cmd);
	void writeByte(uint8_t data);
	void writeDisplayControl();
	uint8_t getKeys();
	uint8_t readByte();

	void begin();
	void end();

	void displayOn();
	void displayOff();

	void setBrightness(uint8_t brightness);
	void clear();

	void delay_micros(uint32_t us)
	{
	    uint32_t us_count_tic =  us * (SystemCoreClock / 1000000)/2; // получаем кол-во тактов за 1 мкс и умножаем на наше значение
	    DWT->CYCCNT = 0U; // обнуляем счётчик
	    while(DWT->CYCCNT < us_count_tic);
	}

    void refresh(void);   //Обновить buf
    void dot(uint8_t on); //Показать дот

    void    showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero, uint8_t length, uint8_t pos);



    bool    _displayOn;  //!< Display on and off status for display control register
    uint8_t _brightness; //!< Display brightness for display control register



    //tm1637.showNumberDec(0, false); // Expect: ___0
    //tm1637.showNumberDec(1, false); // Expect: ___1
    //tm1637.showNumberDec(1, true);  // Expect: 0001
    //tm1637.showNumberDec(14, false, 2, 1); // Expect: _14_
    //tm1637.showNumberDec(14, false, 2, 1); // Expect: _14_
    //tm1637.showNumberDec(-1, false);  // Expect: __-1
    //tm1637.showNumberDec(-1, true);  // Expect: 00-1
    //tm1637.showNumberDec(-999);       // Expect: -999
    //tm1637.showNumberDec(-12);        // Expect: _-12
    //tm1637.showNumberDec(-5, false, 3, 0); // Expect: _-5_
    //! Отображение десятичного числа
    //!
    //! @param num Число, которое будет показано
    //! @param leading_zero Если истина, отображаются в переди нули. Иначе пустота.
    //! ПРИМЕЧАНИЕ: ведущий ноль не поддерживается с отрицательными числами.
    //! @param length Количество цифр для установки. Пользователь должен убедиться, что отображаемое число
    //! соответствует количеству запрошенных цифр (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos Положение старшей цифры (0 - крайний левый, 3 - крайний правый)
    void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    //! Display a decimal number, with dot control
    //!
    //! Display the given argument as a decimal number. The dots between the digits (or colon)
    //! can be individually controlled.
    //!
    //! @param num The number to be shown
    //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
    //!        between the digits (or colon mark, as implemented by each module). i.e.
    //!        For displays with dots between each digit:
    //!        * 0.000 (0b10000000)
    //!        * 00.00 (0b01000000)
    //!        * 000.0 (0b00100000)
    //!        * 0.0.0.0 (0b11100000)
    //!        For displays with just a colon:
    //!        * 00:00 (0b01000000)
    //!        For displays with dots and colons colon:
    //!        * 0.0:0.0 (0b11100000)
    //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
    //!        blank. NOTE: leading zero is not supported with negative numbers.
    //! @param length The number of digits to set. The user must ensure that the number to be shown
    //!        fits to the number of digits requested (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
    void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);


    //tm1637.showNumberHexEx(0xf1af);        // Expect: f1Af
    //tm1637.showNumberHexEx(0x2c);          // Expect: __2C
    //tm1637.showNumberHexEx(0xd1, 0, true); // Expect: 00d1

    //! Display a hexadecimal number, with dot control
    //!
    //! Display the given argument as a hexadecimal number. The dots between the digits (or colon)
    //! can be individually controlled.
    //!
    //! @param num The number to be shown
    //! @param dots Dot/Colon enable. The argument is a bitmask, with each bit corresponding to a dot
    //!        between the digits (or colon mark, as implemented by each module). i.e.
    //!        For displays with dots between each digit:
    //!        * 0.000 (0b10000000)
    //!        * 00.00 (0b01000000)
    //!        * 000.0 (0b00100000)
    //!        * 0.0.0.0 (0b11100000)
    //!        For displays with just a colon:
    //!        * 00:00 (0b01000000)
    //!        For displays with dots and colons colon:
    //!        * 0.0:0.0 (0b11100000)
    //! @param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
    //!        blank
    //! @param length The number of digits to set. The user must ensure that the number to be shown
    //!        fits to the number of digits requested (for example, if two digits are to be displayed,
    //!        the number must be between 0 to 99)
    //! @param pos The position of the most significant digit (0 - leftmost, 3 - rightmost)
    void showNumberHexEx(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

    //! Translate a single digit into 7 segment code
    //!
    //! The method accepts a number between 0 - 15 and converts it to the
    //! code required to display the number on a 7 segment display.
    //! Numbers between 10-15 are converted to hexadecimal digits (A-F)
    //!
    //! @param digit A number between 0 to 15
    //! @return A code representing the 7 segment image of the digit (LSB - segment A;
    //!         bit 6 - segment G; bit 7 - always zero)
    uint8_t encodeDigit(uint8_t digit);
};


#endif /* TM1637_H_ */
