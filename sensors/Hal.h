/*
 * Hal.h
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */
#ifndef SENSORS_HAL_H_
#define SENSORS_HAL_H_

#include "rodos.h"
#include "i2c/i2c.h"
#include "registers/registers.h"

class Hal {
public:
	/**
	 * Must be called from the extending class.<br>
	 * This basically calls the {@link Hal#init()}.
	 */
	Hal();
	virtual ~Hal();

	/**
	 * Exists to set up a sensor.<br>
	 * <br>
	 * Gets called from the constructor.
	 */
	virtual void init();

	/**
	 * Reads the data at the given address from the I2C-Device to the given buffer.<br>
	 * Basically wraps around the writeRead() function which passes the requested register
	 * to the designated device and retrieves its data from there which will be put into the given buffer.<br>
	 *
	 * @return whether the specified Register was valid and readable
	 */
	bool readI2c(I2cDevice *device, Register *reg, uint8_t dataBuffer[], size_t bufferSize);

	/**
	 * Writes the data from buffer to the I2C-Device at the given address.
	 */
	bool writeI2c(I2cDevice *device, Register *reg, uint8_t buffer[], size_t bufferSize);

protected:
	/**
	 * Returns whether the register can be accessed in the requested way.
	 *
	 * @param reg				Pointer to register to check
	 * @param requestedAccess	Desired access to the register
	 * @param allowedRegs		Pointer of array of allowed registers in general
	 * @param allowedRegsSize	Size of the array allowedRegs
	 */
	bool checkRegister(const Register *reg, const REGISTER_ACCESS requestedAccess, const Register **allowedRegs, const size_t allowedRegsSize);

private:
	bool checkI2cAccess(I2cDevice *device, Register *reg, uint8_t accessCount, REGISTER_ACCESS access);
};

#endif /* SENSORS_HAL_H_ */
