/*
 * Hal.cpp
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */

#include "Hal.h"

Hal::Hal(HAL_I2C *i2cDevice) : I2C_DEVICE(i2cDevice)
{}

bool inline Hal::init(uint32_t i2cFreq)
{
	UTILS::clearBuffer(this->writeBuffer, WRITE_BUFFER_LENGTH);
	return initI2c(i2cFreq) && setup();
}

bool inline Hal::initI2c(uint32_t freq)
{
	if (this->I2C_DEVICE == NULL || this->I2C_DEVICE == nullptr)
	{
		return false;
	}
	I2C_DEVICE->reset();
	return (I2C_DEVICE->init(freq) == 0) && setupI2c();
}

bool Hal::checkRegister(const Register *reg, const REGISTER_ACCESS requestedAccess, const Register **allowedRegs,
		const size_t allowedRegsSize)
{
	for (int i = 0; i < allowedRegsSize; i++)
	{
		if (reg->ADDRESS == allowedRegs[i]->ADDRESS)
		{
			return (requestedAccess & REGISTER_ACCESS::READ_ONLY) <= (reg->ACCESS & REGISTER_ACCESS::READ_ONLY) &&
					(requestedAccess & REGISTER_ACCESS::WRITE_ONLY) <= (reg->ACCESS & REGISTER_ACCESS::WRITE_ONLY);
		}
	}

	return false;
}

bool Hal::checkI2cAccess(I2cDevice *counterPart, Register *reg, uint8_t accessCount, REGISTER_ACCESS access)
{
	uint8_t lastAddr = 0x0;
	size_t addrIndex = SIZE_MAX;
	const Register *regToTest = nullptr;

	for (size_t i = 0; i < counterPart->AVAILABLE_REGS_COUNT; i++)
	{
		regToTest = counterPart->AVAILABLE_REGS[i];
		if (reg == regToTest)
		{
			lastAddr = regToTest->ADDRESS;
			addrIndex = i;
			break;
		}
	}

	if (regToTest == nullptr || lastAddr + accessCount > counterPart->AVAILABLE_REGS_COUNT)
	{
		return false;
	}

	size_t i = 1;
	do {
		bool result = this->checkRegister(regToTest, access, counterPart->AVAILABLE_REGS, counterPart->AVAILABLE_REGS_COUNT);

		regToTest = counterPart->AVAILABLE_REGS[addrIndex + i];
		if (!result || (lastAddr - regToTest->ADDRESS) < -1)
		{
			return false;
		}
		lastAddr = regToTest->ADDRESS;
	} while(i++ < accessCount);

	return true;
}

bool Hal::readI2c(I2cDevice *device, Register *reg, uint8_t dataBuffer[], size_t bytesToRead)
{
	if (!this->checkI2cAccess(device, reg, bytesToRead, REGISTER_ACCESS::READ_ONLY))
	{
		return false;
	}

	writeBuffer[0] = reg->ADDRESS;

	if (I2C_DEVICE->writeRead(device->ADDRESS, writeBuffer, 1, dataBuffer, bytesToRead) != bytesToRead)
	{
		return false;
	}

	return true;
}

bool Hal::writeI2c(I2cDevice *device, Register *reg, uint8_t dataBuffer[], size_t bytesToWrite)
{
	if (!this->checkI2cAccess(device, reg, bytesToWrite, REGISTER_ACCESS::WRITE_ONLY))
	{
		return false;
	}

	uint8_t lastAddress = reg->ADDRESS;

	for (uint8_t i = 0; i < bytesToWrite / (WRITE_BUFFER_LENGTH-1); i++)
	{
		writeBuffer[0] = lastAddress++;
		for (uint8_t j = 1; j < WRITE_BUFFER_LENGTH; j++)
		{
			writeBuffer[j] = dataBuffer[i * (WRITE_BUFFER_LENGTH-1) + j-1];
		}

		if (!I2C_DEVICE->write(device->ADDRESS, writeBuffer, WRITE_BUFFER_LENGTH) == WRITE_BUFFER_LENGTH - 1)
		{
			return false;
		}
	}

	if (bytesToWrite % WRITE_BUFFER_LENGTH != 0)
	{
		writeBuffer[0] = lastAddress;
		uint8_t mod = bytesToWrite % (WRITE_BUFFER_LENGTH - 1);

		for (uint8_t i = 1; i < mod; i++)
		{
			writeBuffer[i] = dataBuffer[bytesToWrite - (mod-i+1)];
		}

		if (!I2C_DEVICE->write(device->ADDRESS, writeBuffer, WRITE_BUFFER_LENGTH) == WRITE_BUFFER_LENGTH - 1)
		{
			return false;
		}
	}

	return true;
}
