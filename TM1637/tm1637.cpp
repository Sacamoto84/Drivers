/*
 * tm1637.cpp
 *
 *  Created on: 4 февр. 2021 г.
 *      Author: Ivan
 */

#include "tm1637.h"


TM1637::TM1637()
{

}

TM1637::~TM1637()
{

}


void TM1637::start()
{
    TM1637_DIO_LOW();
    TM1637_PIN_DELAY();
    TM1637_CLK_LOW();
    TM1637_PIN_DELAY();
}

void TM1637::stop()
{
    TM1637_DIO_LOW();
    TM1637_PIN_DELAY();
    TM1637_CLK_HIGH();
    TM1637_PIN_DELAY();
    TM1637_DIO_HIGH();
    TM1637_PIN_DELAY();
}

/*!
 * \brief Write display register
 * \param address Display address 0x00..0x05
 * \param data Value 0x00..0xFF
 */
void TM1637::writeData(uint8_t address, uint8_t data)
{
    if (address < TM1637_NUM_GRIDS) {
        start();
        writeByte((uint8_t)(TM1637_CMD_ADDR | address));
        writeByte(data);
        stop();
    }
}

/*!
 * \brief Write buffer to multiple display registers
 * \details
 *    Write buffer to TM1638 with auto address increment
 * \param address
 *    Display address 0x00..0x05
 * \param buf
 *    Buffer
 * \param len
 *    Buffer length
 */
void TM1637::writeData(uint8_t address, const uint8_t *buf, uint8_t len)
{
    //if ((address + len) <= TM1637_NUM_GRIDS)
   {
        // Write buffer to display registers
        start();
        writeByte((uint8_t)(TM1637_CMD_ADDR | address));
        for (uint8_t i = 0; i < len; i++) {
            writeByte(buf[i]);
        }
        stop();
    }
}

/*!
 * \brief Write command to TM1637.
 */
void TM1637::writeCommand(uint8_t cmd)
{
    start();
    writeByte(cmd);
    stop();
}

/*!
 * \brief Write byte to TM1637.
 * \param data 8-bit value.
 */
void TM1637::writeByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) {
        TM1637_PIN_DELAY();
        TM1637_CLK_LOW();

        if (data & 0x01) {
            TM1637_DIO_HIGH();
        } else {
            TM1637_DIO_LOW();
        }
        data >>= 1;

        TM1637_PIN_DELAY();
        TM1637_CLK_HIGH();
    }

    TM1637_PIN_DELAY();
    TM1637_CLK_LOW();

    // Set DIO pin to input with pull-up to read ACK
    TM1637_DIO_HIGH();
    //TM1637_DIO_INPUT();
    TM1637_PIN_DELAY();

    TM1637_CLK_HIGH();
    TM1637_PIN_DELAY();
    // Skip reading DIO pin: ack = _dioPin;
    TM1637_CLK_LOW();
    TM1637_PIN_DELAY();

    // Set DIO pin to output
    //TM1637_DIO_OUTPUT();
    TM1637_PIN_DELAY();
}

// -------------------------------------------------------------------------------------------------
/*!
 * \brief Write display control register.
 */
void TM1637::writeDisplayControl()
{
    // Write to display control register
    start();
    writeByte((uint8_t)(TM1637_CMD_CTRL |
                        (_displayOn ? TM1637_CTRL_DISPLAY_ON : TM1637_CTRL_DISPLAY_OFF) |
                         _brightness));
    stop();
}

void TM1637::begin()
{
	SCB_DEMCR   |= CoreDebug_DEMCR_TRCENA_Msk; // разрешаем использовать счётчик
	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk;   // запускаем счётчик

    // Make DIO and CLK pins high
    TM1637_DIO_HIGH();
    TM1637_CLK_HIGH();

    // Write _displayOn and _brightness to display control register
    writeDisplayControl();

    // Data write with auto address increment
    writeCommand(TM1637_CMD_DATA | TM1637_DATA_WRITE | TM1637_DATA_AUTO_INC_ADDR);
}


/*!
 * \brief Turn Display on.
 */
void TM1637::displayOn()
{
    _displayOn = true;

    writeDisplayControl();
}

/*!
 * \brief Turn display off.
 */
void TM1637::displayOff()
{
    _displayOn = false;

    writeDisplayControl();
}

/*!
 * \brief Set brightness LED's.
 * \param brightness
 *    Display brightness value 0..7
0 1/16
1 2/16
2 4/16
3 10/16
4 11/16
5 12/16
6 13/16
7 14/16

*/
void TM1637::setBrightness(uint8_t brightness)
{
    if (brightness <= 7) {
        _brightness = brightness;

        writeDisplayControl();
    }
}

/*!
 * \brief Turn all LED's off.
 */
void TM1637::clear()
{
    start();
    writeByte((uint8_t)(TM1637_CMD_ADDR | 0x00));
    for (uint8_t i = 0; i < TM1637_NUM_GRIDS; i++) {
        writeByte(0x00);
    }
    stop();
}

void TM1637::dot(uint8_t on) //Показать дот
{
if (on)
 buf[1] |= 0x80;
else
 buf[1]	&= ~ 0x80;
}

//Записать буффер в экран
void TM1637::refresh(void)
{
  writeData (0x00, buf, sizeof (buf));
}

void TM1637::showNumberDec(int num, bool leading_zero, uint8_t length, uint8_t pos)
{
	  showNumberDecEx(num, 0, leading_zero, length, pos);
}

void TM1637::showNumberDecEx(int num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void TM1637::showNumberHexEx(uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void TM1637::showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
    bool negative = false;
	if (base < 0) {
	    base = -base;
		negative = true;
	}

    uint8_t digits[4]={0,0,0,0};

	if (num == 0 && !leading_zero) {
		// Singular case - take care separately
		for(uint8_t i = 0; i < (length-1); i++)
			digits[i] = 0;
		digits[length-1] = encodeDigit(0);
	}
	else {
		//uint8_t i = length-1;
		//if (negative) {
		//	// Negative number, show the minus sign
		//    digits[i] = minusSegments;
		//	i--;
		//}

		for(int i = length-1; i >= 0; --i)
		{
		    uint8_t digit = num % base;

			if (digit == 0 && num == 0 && leading_zero == false)
			    // Leading zero is blank
				digits[i] = 0;
			else
			    digits[i] = encodeDigit(digit);

			if (digit == 0 && num == 0 && negative) {
			    digits[i] = minusSegments;
				negative = false;
			}

			num /= base;
		}
    }



	//if(dots != 0)
	//{
	////	showDots(dots, digits);
	//}
	writeData (pos, digits, length);
    //setSegments(digits, length, pos);
}

uint8_t TM1637::encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}
