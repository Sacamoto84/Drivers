#include "Wire.h"

/** TwoWire object used when in slave interrupt
 */
TwoWire *slaveTwoWire[4];

TwoWire::TwoWire(I2C_TypeDef *instance) {
	handle.Instance = instance;
}

void TwoWire::begin(void) {

	rxBufferIndex = 0;
	rxBufferLength = 0;

	txBufferIndex = 0;
	txBufferLength = 0;

	isMaster = 1;
}

void TwoWire::begin(uint8_t address) {
	isMaster = 0;
	this->address = address << 1;
}

void TwoWire::begin(int address) {
	begin((uint8_t) address);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity,
		uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
	if (isMaster == true) {
		if (isize > 0) {
			// send internal address; this mode allows sending a repeated start to access
			// some devices' internal registers. This function is executed by the hardware
			// TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

			beginTransmission(address);

			// the maximum size of internal address is 3 bytes
			if (isize > 3) {
				isize = 3;
			}

			// write internal register address - most significant byte first
			while (isize-- > 0)
				write((uint8_t) (iaddress >> (isize * 8)));
			endTransmission(false);
		}

		// clamp to buffer length
		if (quantity > BUFFER_LENGTH) {
			quantity = BUFFER_LENGTH;
		}

		// perform blocking read into buffer
		//uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
		uint8_t read = 0;

		if (HAL_I2C_Master_Receive(&handle, address << 1, rxBuffer, quantity,
				1000) == HAL_OK) {
			read = quantity;
		}

		// set rx buffer iterator vars
		rxBufferIndex = 0;
		rxBufferLength = read;

		return read;
	}

	return 0;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity,
		uint8_t sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint32_t) 0,
			(uint8_t) 0, (uint8_t) sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
	return requestFrom((uint8_t) address, (uint8_t) quantity, (uint8_t) true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
	return requestFrom((uint8_t) address, (uint8_t) quantity,
			(uint8_t) sendStop);
}

void TwoWire::beginTransmission(uint8_t address) {
	// set address of targeted slave
	txAddress = address << 1;
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
	beginTransmission((uint8_t) address);
}

//
//  Originally, 'endTransmission' was an f(void) function.
//  It has been modified to take one parameter indicating
//  whether or not a STOP should be performed on the bus.
//  Calling endTransmission(false) allows a sketch to
//  perform a repeated start.
//
//  WARNING: Nothing in the library keeps track of whether
//  the bus tenure has been properly ended with a STOP. It
//  is very possible to leave the bus in a hung state if
//  no call to endTransmission(true) is made. Some I2C
//  devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop) {
	int8_t ret = 4;

	if (isMaster == true) {
		HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&handle, txAddress,
				txBuffer, txBufferLength, HAL_MAX_DELAY);
		switch (status) {
		case HAL_OK:
			ret = 0;
			break;
		case HAL_TIMEOUT:
			ret = 1;
			break;
		default:
			ret = 4;
			break;
		}

		txBufferIndex = 0;
		txBufferLength = 0;
	}

	return ret;
}

//  This provides backwards compatibility with the original
//  definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void) {
	return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data) {

	// in master transmitter mode
	// don't bother if buffer is full
	if (txBufferLength >= BUFFER_LENGTH) {
		return 0;
	}
	// put byte in tx buffer
	txBuffer[txBufferIndex] = data;
	++txBufferIndex;
	// update amount in buffer
	txBufferLength = txBufferIndex;

	return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity) {
	// in master transmitter mode
	for (size_t i = 0; i < quantity; ++i) {
		write(data[i]);
	}
	return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void) {
	return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void) {
	int value = -1;

	// get each successive byte on each call
	if (rxBufferIndex < rxBufferLength) {
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}

	return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void) {
	int value = -1;

	if (rxBufferIndex < rxBufferLength) {
		value = rxBuffer[rxBufferIndex];
	}

	return value;
}

void TwoWire::flush(void) {
	rxBufferIndex = 0;
	rxBufferLength = 0;
	txBufferIndex = 0;
	txBufferLength = 0;

}

TwoWire *interruptWire;

TwoWire Wire = TwoWire(I2C2);

