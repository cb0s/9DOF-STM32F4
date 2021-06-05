/*
 * Hal.h
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */
#ifndef SENSORS_HAL_H_
#define SENSORS_HAL_H_

#include "rodos.h"
#include "hal.h"
#include "../utils.h"
#include "i2c/i2c.h"
#include "registers/registers.h"

/**
 * Write-buffer size for the Hal-Classes.
 */
#define WRITE_BUFFER_LENGTH 8

class Hal {
public:
	/**
	 * Must be called from the extending class.<br>
	 * This basically calls the {@link Hal#init()}.
	 */
	Hal(HAL_I2C *device);
	virtual ~Hal();

	/**
	 * Sets up a sensor.<br>
	 *
	 * @return whether setup was successful
	 */
	virtual bool setup();

	/**
	 * Sets up the I2C-Interface for the hall-sensor.
	 */
	virtual bool setupI2c();

	/**
	 * Initializes the Hal-Class in general. This means all init() methods will be called<br>
	 * After that #setup() is called to complete the initialize process.<br>
	 * <br>
	 * Breaks if only one method fails.
	 *
	 * @return if all init()-calls and the #setup()-call afterwards were successful
	 */
	bool init(uint32_t i2cFreq);

	/**
	 * Initializes the I2C-device with a frequency.<br>
	 * Calls #setupI2c() afterwards.<br>
	 * <br>
	 * Breaks if only one method fails.
	 *
	 * @return if the setup of the frequency and the #setupI2c()-call were successful
	 */
	bool initI2c(uint32_t freq);

	/**
	 * Reads the data at the given address from the I2C-Device to the given buffer.<br>
	 * Basically wraps around the writeRead() function which passes the requested register
	 * to the designated device and retrieves its data from there which will be put into the given buffer.<br>
	 *
	 * @return whether the specified Register was valid and readable
	 */
	bool readI2c(I2cDevice *device, Register *reg, uint8_t dataBuffer[], size_t bytesToRead);

	/**
	 * Writes the data from buffer to the I2C-Device at the given address.
	 */
	bool writeI2c(I2cDevice *device, Register *reg, uint8_t buffer[], size_t bytesToWrite);

protected:
	/**
	 * Returns whether the register can be accessed in the requested way.
	 *
	 * @param reg				Pointer to register to check
	 * @param requestedAccess	Desired access to the register
	 * @param allowedRegs		Pointer of array of allowed registers in general
	 * @param allowedRegsSize	Size of the array allowedRegs
	 *
	 * @return if all checked registers can be accessed in the specified way
	 */
	bool checkRegister(const Register *reg, const REGISTER_ACCESS requestedAccess, const Register **allowedRegs,
			const size_t allowedRegsSize);

private:
	/**
	 * Checks if the requested registers on the given I2cDevice can be accessed in the way specified by the given REGISTER_ACCESS.<br>
	 * All registers from reg->ADDRESS to reg->ADDRESS + addressCount will be checked.
	 *
	 * @return if all checked registers can be accessed in the specified way
	 */
	bool checkI2cAccess(I2cDevice *device, Register *reg, uint8_t accessCount, REGISTER_ACCESS access);

	uint8_t writeBuffer[WRITE_BUFFER_LENGTH];

	HAL_I2C *I2C_DEVICE;
};

#endif /* SENSORS_HAL_H_ */
