/*
 * Hal.cpp
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */

#include "Hal.h"

Hal::Hal()
{
	init();
}

bool Hal::checkRegister(const Register *reg, const REGISTER_ACCESS requestedAccess, const Register **allowedRegs, const size_t allowedRegsSize)
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

bool Hal::checkI2cAccess(I2cDevice *device, Register *reg, uint8_t accessCount, REGISTER_ACCESS access)
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

	if (regToTest == nullptr || lastAddr + accessCount > device->AVAILABLE_REGS_COUNT)
	{
		return false;
	}

	size_t i = 1;
	do {
		bool result = Hal::checkRegister(regToTest, access, device->AVAILABLE_REGS, device->AVAILABLE_REGS_COUNT);

		regToTest = device->AVAILABLE_REGS[addrIndex + i];
		if (!result || (lastAddr - regToTest->ADDRESS) < -1)
		{
			return false;
		}
		lastAddr = regToTest->ADDRESS;
	} while(i++ < accessCount);

	return true;
}

bool Hal::readI2c(I2cDevice *device, Register *reg, uint8_t dataBuffer[], size_t bufferSize)
{
	return Hal::checkI2cAccess(device, reg, bufferSize, REGISTER_ACCESS::READ_ONLY);
}

bool Hal::writeI2c(I2cDevice *device, Register *reg, uint8_t dataBuffer[], size_t bufferSize)
{
	return Hal::checkI2cAccess(device, reg, bufferSize, REGISTER_ACCESS::WRITE_ONLY);
}
