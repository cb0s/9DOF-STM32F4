/*
 * spi.h
 *
 *  Created on: 08.07.2021
 *      Author: Cedric Boes
 */

#ifndef SENSORS_SPI_SPI_H_
#define SENSORS_SPI_SPI_H_

#include "stdint.h"
#include "../registers/registers.h"

enum class SPI_DEVICE_TYPE : uint8_t {
	MASTER,
	SLAVE
};

struct SpiDevice {
	SpiDevice(SPI_DEVICE_TYPE type, const Register **availableRegs, size_t availableRegsCount)
		: TYPE(type), AVAILABLE_REGS(availableRegs), AVAILABLE_REGS_COUNT(availableRegsCount)
	{}

	const SPI_DEVICE_TYPE TYPE;
	const Register **AVAILABLE_REGS;
	size_t AVAILABLE_REGS_COUNT;
};

#endif /* SENSORS_SPI_SPI_H_ */
