/******************************************************************************
* TI INA219 hi-side i2c current/power monitor Library
*
* http://www.ti.com/product/ina219
*
* 6 May 2012 by John De Cristofaro
*
*
* Tested at standard i2c 100kbps signaling rate.
*
* This library does not handle triggered conversion modes. It uses the INA219
* in continuous conversion mode. All reads are from continous conversions.
*
* A note about the gain (PGA) setting:
*	The gain of the ADC pre-amplifier is programmable in the INA219, and can
*	be set between 1/8x (default) and unity. This allows a shunt voltage 
*	range of +/-320mV to +/-40mV respectively. Something to keep in mind,
*	however, is that this change in gain DOES NOT affect the resolution
*	of the ADC, which is fixed at 1uV. What it does do is increase noise
*	immunity by exploiting the integrative nature of the delta-sigma ADC.
*	For the best possible reading, you should set the gain to the range
*	of voltages that you expect to see in your particular circuit. See
*	page 15 in the datasheet for more info about the PGA.
*
* Known bugs:
*     * may return unreliable values if not connected to a bus or at
*	bus currents below 10uA.
*
* Arduino 1.0 compatible as of 6/6/2012
*
* Dependencies:
*    * Arduino Wire library
*
* MIT license
******************************************************************************/

#include "INA219.h"

INA219::INA219() {
}


void INA219::begin(uint8_t addr)
{
  //Wire.begin();
  i2c_address = addr;
  gain = D_GAIN;
}


// calibration of equations and device
// shunt_val 		= value of shunt in Ohms
// v_shunt_max 		= maximum value of voltage across shunt
// v_bus_max 		= maximum voltage of bus
// i_max_expected 	= maximum current draw of bus + shunt
// default values are for a 0.25 Ohm shunt on a 5V bus with max current of 1A
void INA219::calibrate(float shunt_val, float v_shunt_max, float v_bus_max, float i_max_expected)
{
  uint16_t cal;
  //float i_max_possible, min_lsb, max_lsb, swap;
  float min_lsb, swap;

  r_shunt = shunt_val;

  //i_max_possible = v_shunt_max / r_shunt;
  min_lsb = i_max_expected / 32767;
  //max_lsb = i_max_expected / 4096;

  current_lsb = (uint16_t)(min_lsb * 100000000) + 1;
  current_lsb /= 100000000;
  swap = (0.04096)/(current_lsb*r_shunt);
  cal = (uint16_t)swap;
  power_lsb = current_lsb * 20;

#if (INA219_DEBUG == 1)
  Serial.print("v_bus_max:	"); Serial.println(v_bus_max, 8);
  Serial.print("v_shunt_max:	"); Serial.println(v_shunt_max, 8);
  Serial.print("i_max_possible:	"); Serial.println(i_max_possible, 8);
  Serial.print("i_max_expected: "); Serial.println(i_max_expected, 8);
  Serial.print("min_lsb:	"); Serial.println(min_lsb, 12);
  Serial.print("max_lsb:	"); Serial.println(max_lsb, 12);
  Serial.print("current_lsb:	"); Serial.println(current_lsb, 12);
  Serial.print("power_lsb:	"); Serial.println(power_lsb, 8);
  Serial.println("  ");
  Serial.print("cal:		"); Serial.println(cal);
  Serial.print("r_shunt:	"); Serial.println(r_shunt);
#endif

  write16(CAL_R, cal);

}


// config values (range, gain, bus adc, shunt adc, mode) can be derived from pp26-27 in the datasheet
// defaults are:
// range = 1 (0-32V bus voltage range)
// gain = 3 (1/8 gain - 320mV range)
// bus adc = 3 (12-bit, single sample, 532uS conversion time)
// shunt adc = 3 (12-bit, single sample, 532uS conversion time)
// mode = 7 (continuous conversion)
void INA219::configure(uint8_t range, uint8_t gain, uint8_t bus_adc, uint8_t shunt_adc, uint8_t mode)
{
  config = 0;

  config |= (range << BRNG | gain << PG0 | bus_adc << BADC1 | shunt_adc << SADC1 | mode);

  write16(CONFIG_R, config);		
}

// resets the INA219
void INA219::reset()
{
  write16(CONFIG_R, INA_RESET);
  HAL_Delay(5);
}

// returns the raw binary value of the shunt voltage
int16_t INA219::shuntVoltageRaw()
{
  return read16(V_SHUNT_R);
}

// returns the shunt voltage in volts.
float INA219::shuntVoltage()
{
  float temp;
  temp = read16(V_SHUNT_R);
  return (temp / 100);
}

// returns raw bus voltage binary value
int16_t INA219::busVoltageRaw()
{
  return read16(V_BUS_R);
}

// returns the bus voltage in volts
float INA219::busVoltage()
{
  int16_t temp;
  temp = read16(V_BUS_R);
  temp >>= 3;
  return (temp * 0.004);
}

// returns the shunt current in amps
float INA219::shuntCurrent()
{
  return (read16(I_SHUNT_R) * current_lsb);
}

// returns the bus power in watts
float INA219::busPower()
{
  return (read16(P_BUS_R) * power_lsb);
}


/**********************************************************************
* 			INTERNAL I2C FUNCTIONS			      *
**********************************************************************/

// writes a 16-bit word (d) to register pointer (a)
// when selecting a register pointer to read from, (d) = 0
void INA219::write16(uint8_t a, uint16_t d) {

	uint8_t dt[2];
	dt[0] = d>>8;
	dt[1] = d & 0xFF;
    HAL_I2C_Mem_Write(&hi2c2, 0x80, a, 1, dt, 2, 1000);

    HAL_Delay(1);
}


int16_t INA219::read16(uint8_t a) {
  uint8_t dt[2];
  HAL_I2C_Mem_Read(&hi2c2, 0x80, a, 1, &dt[0], 2, 1000);

   uint16_t value = dt[0] << 8;
   value += dt[1];
   return value;
}

