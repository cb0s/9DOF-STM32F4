/*
 * HalSpi.h
 *
 *  Created on: 08.07.2021
 *      Author: Cedric Boes
 */

#ifndef SENSORS_HALSPI_H_
#define SENSORS_HALSPI_H_

#include "rodos.h"
#include "hal.h"
#include "../utils.h"
#include "spi/spi.h"
#include "registers/registers.h"

/**
 * Buffer sizes for the HalSpi-Classes.
 */
#define SPI_WRITE_BUFFER_LENGTH 2

class HalSpi {
public:
	/**
	 * Must be called from the extending class.<br>
	 * This basically calls the {@link HalSpi#init()}.
	 */
	HalSpi(HAL_SPI *spiDevice);
	virtual ~HalSpi();

	/**
	 * Sets up a sensor.<br>
	 *
	 * @return whether setup was successful
	 */
	virtual bool setup() = 0;

	/**
	 * Sets up the SPI-Interface for the hall-sensor.
	 */
	virtual bool setupSpi() = 0;

	/**
	 * Initializes the HalSpi-Class in general. This means all init() methods will be called<br>
	 * After that #setup() is called to complete the initialize process.<br>
	 * <br>
	 * Breaks if only one method fails.
	 *
	 * @return if all init()-calls and the #setup()-call afterwards were successful
	 */
	bool init(uint32_t i2cFreq);

	/**
	 * Initializes the SPI-device with a frequency.<br>
	 * Calls #setupSpi() afterwards.<br>
	 * <br>
	 * Breaks if only one method fails.
	 *
	 * @return if the setup of the frequency and the #setupI2c()-call were successful
	 */
	bool initSpi(uint32_t freq);

	/**
	 * Reads the data at the given address from the given sensor of the given SPI-Device to the given buffer.<br>
	 * First of all the requested register is written to the SPI-Device. After that the data is read.<br>
	 * This is not done simultaneously (no writeRead()) because of inconsistencies.
	 *
	 * @return whether the specified Register was valid and readable
	 */
	bool readSpi(HAL_GPIO *sensor, SpiDevice *device, Register *reg,
			uint8_t *dataBuffer, size_t bytesToRead,
			uint8_t specialFlags=0x0, bool orOperator=true);

	/**
	 * Writes the data from buffer to the SPI-Device at the given address of the given sensor.<br>
	 * Currently this just uses the first byte to write. The other bytes will be ignored
	 */
	bool writeSpi(HAL_GPIO *sensor, SpiDevice *device, Register *reg,
			uint8_t *buffer, size_t bytesToWrite,
			uint8_t specialFlags=0x0, bool orOperator=true);

private:
	/**
	 * Checks if the requested registers on the given SpiDevice can be accessed in the way specified by the given REGISTER_ACCESS.<br>
	 * All registers from reg->ADDRESS to reg->ADDRESS + addressCount will be checked.
	 *
	 * @return if all checked registers can be accessed in the specified way
	 */
	bool checkSpiAccess(SpiDevice *device, Register *reg, uint8_t accessCount, REGISTER_ACCESS access);

	uint8_t writeBuffer[SPI_WRITE_BUFFER_LENGTH];
	HAL_SPI *SPI_DEVICE;
};

#endif /* SENSORS_HALSPI_H_ */
