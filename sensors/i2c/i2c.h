/*
 * i2c.h
 *
 *  Created on: 04.06.2021
 *      Author: Cedric Boes
 */

#ifndef SENSORS_I2C_I2C_H_
#define SENSORS_I2C_I2C_H_

#include "rodos.h"
#include "../registers/registers.h"

enum class I2C_DEVICE_TYPE {
	MASTER,
	SLAVE
};

struct I2cDevice {
	I2cDevice(uint8_t address, I2C_DEVICE_TYPE type, const Register **availableRegs, size_t availableRegsCount)
		: ADDRESS(address), TYPE(type), AVAILABLE_REGS(availableRegs), AVAILABLE_REGS_COUNT(availableRegsCount)
	{}

	const uint8_t ADDRESS;
	const I2C_DEVICE_TYPE TYPE;
	const Register **AVAILABLE_REGS;
	const size_t AVAILABLE_REGS_COUNT;
};



#endif /* SENSORS_I2C_I2C_H_ */
