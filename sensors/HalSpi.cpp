/*
 * HalSpi.cpp
 *
 *  Created on: 08.07.2021
 *      Author: Cedric Boes
 */

#include "HalSpi.h"

HalSpi::HalSpi(HAL_SPI *spiDevice) : SPI_DEVICE(spiDevice)
{}

HalSpi::~HalSpi()
{}

bool HalSpi::init(uint32_t spiFreq)
{
	UTILS::clearBuffer(this->writeBuffer, SPI_WRITE_BUFFER_LENGTH);
	return initSpi(spiFreq) && setup();
}

bool HalSpi::initSpi(uint32_t freq)
{
	if (this->SPI_DEVICE == NULL || this->SPI_DEVICE == nullptr)
	{
		return false;
	}
	SPI_DEVICE->reset();
	return (SPI_DEVICE->init(freq) == 0) && setupSpi();
}

bool HalSpi::checkSpiAccess(SpiDevice *device, Register *reg, uint8_t accessCount, REGISTER_ACCESS access)
{
	uint8_t lastAddr = 0x0;
	size_t addrIndex = SIZE_MAX;
	const Register *regToTest = nullptr;

	for (size_t i = 0; i < device->AVAILABLE_REGS_COUNT; i++)
	{
		regToTest = device->AVAILABLE_REGS[i];
		if (reg == regToTest)
		{
			lastAddr = regToTest->ADDRESS;
			addrIndex = i;
			break;
		}
	}

	if (regToTest == nullptr || (addrIndex + accessCount) > device->AVAILABLE_REGS_COUNT)
	{
		return false;
	}

	for (size_t i = 0; i < accessCount; i++)
	{
		regToTest = device->AVAILABLE_REGS[addrIndex + i];
		bool result = REG_HELPERS::checkRegister(regToTest, access, device->AVAILABLE_REGS, device->AVAILABLE_REGS_COUNT);

		if (!result || (lastAddr - regToTest->ADDRESS) < -1)
		{
			return false;
		}
		lastAddr = regToTest->ADDRESS;
	}

	return true;
}

bool HalSpi::readSpi(HAL_GPIO *sensor, SpiDevice *device, Register *reg, uint8_t *dataBuffer,
		size_t bytesToRead, uint8_t specialFlags, bool orOperator)
{
	if (!this->checkSpiAccess(device, reg, bytesToRead, REGISTER_ACCESS::READ_ONLY))
	{
		return false;
	}

	writeBuffer[0] = reg->ADDRESS;
	writeBuffer[0] |= 0x80;	// | 10000000 -> read
	if (orOperator)
	{
		writeBuffer[0] |= specialFlags;
	}
	else
	{
		writeBuffer[0] &= specialFlags;
	}

	bool retVal = true;

	sensor->setPins(0);
	if (SPI_DEVICE->write(writeBuffer, 1) < 0) retVal = false;
	if (SPI_DEVICE->read(dataBuffer, bytesToRead) < 0) retVal = false;
	sensor->setPins(1);

	return retVal;
}

bool HalSpi::writeSpi(HAL_GPIO *sensor, SpiDevice *device, Register *reg, uint8_t *dataBuffer,
		size_t bytesToWrite, uint8_t specialFlags, bool orOperator)
{
	if (!this->checkSpiAccess(device, reg, bytesToWrite, REGISTER_ACCESS::WRITE_ONLY))
	{
		return false;
	}

	writeBuffer[0] = reg->ADDRESS;
	writeBuffer[0] &= 0x7F;	// & 01111111 -> write

	writeBuffer[1] = dataBuffer[0];

	if (orOperator)
	{
		writeBuffer[0] |= specialFlags;
	}
	else
	{
		writeBuffer[0] &= specialFlags;
	}

	bool retVal = true;

	// Currently this just writes 1 byte to the given register. The others will be ignored
	sensor->setPins(0);	// enable
	if (SPI_DEVICE->write(writeBuffer, 2) < 0) retVal = false;
	sensor->setPins(1);	// disable

	return retVal;
}
