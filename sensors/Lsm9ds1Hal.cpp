/*
 * Lsm9ds1.cpp
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */

#include "Lsm9ds1Hal.h"

// Must be <255 otherwise it is treated like infinity
#define FAILED_RECOGNITION_ATTEMPTS 3

Lsm9ds1Hal::Lsm9ds1Hal(HAL_I2C *device)
	: Hal(device)
{
	// Standard norm values for this exercise
	setNormalizationValues(LSM9DS1_GENERAL_CONSTANTS::ACCEL_LIN_SENS_2G,
			LSM9DS1_GENERAL_CONSTANTS::ANGULAR_RATE_SENS_2000DPS, LSM9DS1_GENERAL_CONSTANTS::MAGNETIC_SENS_4GAUSS);
}

Lsm9ds1Hal::~Lsm9ds1Hal()
{}

bool Lsm9ds1Hal::initLsm9ds1()
{
	return this->init(LSM9DS1_I2C::FREQ);
}

bool Lsm9ds1Hal::setup()
{
	UTILS::clearBuffer(dataBuffer, 6);
	return true;
}

bool Lsm9ds1Hal::setupI2c()
{
	// Check that both I2C-Devices are recognized -> fail after FAILED_RECOGNITION_ATTEMPTS unsuccessful attempts
	if (!(this->detectSensor(&LSM9DS1_I2C::GYRO_ACCEL, &LSM9DS1_GXL_REGS::WHO_AM_I_GXL, LSM9DS1_I2C::GYRO_ACCEL.ADDRESS) ||
			this->detectSensor(&LSM9DS1_I2C::MAGNETOMETER, &LSM9DS1_M_REGS::WHO_AM_I_M, LSM9DS1_I2C::MAGNETOMETER.ADDRESS)))
	{
		return false;
	}

	dataBuffer[0] = 0x0;

	// Following listing like this to ensure readability and maintainability
	// Gyro/Accel
	// ODR=238Hz, Cutoff=14Hz, Full-scale=2000
	dataBuffer[0] |= 0b1 << 7;	// ODR 2
	dataBuffer[0] |= 0b0 << 6;	// ODR 1
	dataBuffer[0] |= 0b0 << 5;	// ODR 0
	dataBuffer[0] |= 0b1 << 4;	// FS 1
	dataBuffer[0] |= 0b1 << 3;	// FS 0
	dataBuffer[0] |= 0b0 << 2;	// Blank (must always be 0)
	dataBuffer[0] |= 0b0 << 1;	// BW 1
	dataBuffer[0] |= 0b0 << 0;	// BW 0

	if (!this->writeGxlI2c(&LSM9DS1_GXL_REGS::CTRL_REG1_G, dataBuffer, 1))
	{
		return false;
	}

	dataBuffer[0] = 0x0;

	// ODR=238Hz, Acceleration Full-Scale=(+/-)2g, BW=auto, Acceleration-Anti-Alias-Filter-Bandwidth=50Hz
	dataBuffer[0] |= 0b1 << 7;	// ODR 2
	dataBuffer[0] |= 0b0 << 6;	// ODR 1
	dataBuffer[0] |= 0b0 << 5;	// ODR 0
	dataBuffer[0] |= 0b0 << 4;	// FS 1
	dataBuffer[0] |= 0b0 << 3;	// FS 0
	dataBuffer[0] |= 0b0 << 2;	// BW_SELECT
	dataBuffer[0] |= 0b0 << 1;	// BW 1
	dataBuffer[0] |= 0b0 << 0;	// BW 0

	if (!this->writeGxlI2c(&LSM9DS1_GXL_REGS::CTRL_REG6_XL, dataBuffer, 1))
	{
		return false;
	}

	dataBuffer[0] = 0x0;

	// Magnetometer
	// ODR=80Hz, low-power-mode, no temperature compensation, no fast ODR, self-test disabled
	dataBuffer[0] |= 0b0 << 7;	// TEMP_COMP
	dataBuffer[0] |= 0b0 << 6;	// OM 1
	dataBuffer[0] |= 0b0 << 5;	// OM 0
	dataBuffer[0] |= 0b1 << 4;	// DO 2
	dataBuffer[0] |= 0b1 << 3;	// DO 1
	dataBuffer[0] |= 0b1 << 2;	// DO 0
	dataBuffer[0] |= 0b0 << 1;	// FAST_ODR
	dataBuffer[0] |= 0b0 << 0;	// ST

	if (!this->writeMI2c(&LSM9DS1_M_REGS::CTRL_REG1_M, dataBuffer, 1))
	{
		return false;
	}

	dataBuffer[0] = 0x0;

	// Magnetic-Full-Scale=(+/-)4gauss
	dataBuffer[0] |= 0b0 << 7;	// Blank (must always be 0)
	dataBuffer[0] |= 0b0 << 6;	// FS 1
	dataBuffer[0] |= 0b0 << 5;	// FS 0
	dataBuffer[0] |= 0b0 << 4;	// Blank (must always be 0)
	dataBuffer[0] |= 0b0 << 3;	// REBOOT
	dataBuffer[0] |= 0b0 << 2;	// SOFT_RST
	dataBuffer[0] |= 0b0 << 1;	// Blank (must always be 0)
	dataBuffer[0] |= 0b0 << 0;	// Blank (must always be 0)

	if (!this->writeMI2c(&LSM9DS1_M_REGS::CTRL_REG2_M, dataBuffer, 1))
	{
		return false;
	}

	dataBuffer[0] = 0x0;

	// Continuous Conversion Mode
	dataBuffer[0] |= 0b0 << 7;	// I2C-Disable
	dataBuffer[0] |= 0b0 << 6;	// Blank (must always be 0)
	dataBuffer[0] |= 0b0 << 5;	// LP
	dataBuffer[0] |= 0b0 << 4;	// Blank (must always be 0)
	dataBuffer[0] |= 0b0 << 3;	// Blank (must always be 0)
	dataBuffer[0] |= 0b0 << 2;	// SIM
	dataBuffer[0] |= 0b0 << 1;	// MD 1
	dataBuffer[0] |= 0b0 << 0;	// MD 0

	if (!this->writeMI2c(&LSM9DS1_M_REGS::CTRL_REG3_M, dataBuffer, 1))
	{
		return false;
	}

	return true;
}


bool Lsm9ds1Hal::readTemp(float &temp)
{
	if (!this->readGxlI2c(&LSM9DS1_GXL_REGS::OUT_TEMP_L, dataBuffer, 2))
	{
		return false;
	}

	int16_t tempBits = (((int16_t) dataBuffer[0]) << 8 | dataBuffer[1]) & 0b0000111111111111;

	// For signed numbers
	// 0x0800 = 0b0000 1000 0000 0000
	if (tempBits & 0x0800)
	{
		// 0xF000 = 0b1111 0000 0000 0000
		tempBits |= 0xF000;
	}

	temp = (float) tempBits / LSM9DS1_GENERAL_CONSTANTS::TEMP_NORM_FACTOR + LSM9DS1_GENERAL_CONSTANTS::NORM_TEMP;

	UTILS::clearBuffer(dataBuffer, 2);
	return true;
}

bool Lsm9ds1Hal::readAcceleration(Vector3D &acc)
{
	if (!this->	readGxlI2c(&LSM9DS1_GXL_REGS::OUT_X_XL_L, dataBuffer, 6))
	{
		return false;
	}

	PRINTF("\n\nACCEL - %c%c%c%c%c%c\n\n", dataBuffer[0], dataBuffer[1], dataBuffer[2], dataBuffer[3], dataBuffer[4], dataBuffer[5]);

	// All values must be converted from mg's to g's
	acc.x = 	(float) static_cast<int16_t>((dataBuffer[1] << 8) | dataBuffer[0]) * this->linearAccelSens / 1000.0;
	acc.y = 	(float) static_cast<int16_t>((dataBuffer[3] << 8) | dataBuffer[2]) * this->linearAccelSens / 1000.0;
	acc.z = -1*	(float) static_cast<int16_t>((dataBuffer[5] << 8) | dataBuffer[4]) * this->linearAccelSens / 1000.0;

	PRINTF("\nACCEL2 - %f %f %f\n\n", acc.x, acc.y, acc.z);

	UTILS::clearBuffer(dataBuffer, 6);
	return true;
}

bool Lsm9ds1Hal::readRotation(Vector3D &rot)
{
	if (!this->	readGxlI2c(&LSM9DS1_GXL_REGS::OUT_X_G_L, dataBuffer, 6))
	{
		return false;
	}

	// All values must be converted from mdps to dps
	rot.x = (float) static_cast<int16_t>((dataBuffer[1] << 8) | dataBuffer[0]) * this->angularRateSens / 1000.0;
	rot.y = (float) static_cast<int16_t>((dataBuffer[3] << 8) | dataBuffer[2]) * this->angularRateSens / 1000.0;
	rot.z = (float) static_cast<int16_t>((dataBuffer[5] << 8) | dataBuffer[4]) * this->angularRateSens / 1000.0;

	UTILS::clearBuffer(dataBuffer, 6);
	return true;
}

bool Lsm9ds1Hal::readMagneticField(Vector3D &mag)
{
	if (!this->	readMI2c(&LSM9DS1_M_REGS::OUT_X_M_L, dataBuffer, 6))
	{
		return false;
	}

	// All values must be converted from mgauss to gauss
	mag.x = -1*	(float) static_cast<int16_t>((dataBuffer[1] << 8) | dataBuffer[0]) * this->magneticSens / 1000.0;
	mag.y = 	(float) static_cast<int16_t>((dataBuffer[3] << 8) | dataBuffer[2]) * this->magneticSens / 1000.0;
	mag.z = 	(float) static_cast<int16_t>((dataBuffer[5] << 8) | dataBuffer[4]) * this->magneticSens / 1000.0;

	UTILS::clearBuffer(dataBuffer, 6);
	return true;
}


bool Lsm9ds1Hal::detectSensor(I2cDevice *device, Register *whoAmI, uint8_t expectedValue)
{
	dataBuffer[0] = 0x0;

	for (uint8_t i = 0; i < FAILED_RECOGNITION_ATTEMPTS; i++)
	{
		this->readI2c(device, whoAmI, dataBuffer, 1);

		if (dataBuffer[0] == expectedValue)
		{
			return true;
		}

		// Thread::suspendCallerUntil(RODOS::NOW() + 500 * MILLISECONDS);
		UTILS::blockingDelay(500 * MILLISECONDS);
	}

	return false;
}

void Lsm9ds1Hal::setNormalizationValues(float linearAccelSens, float angularRateSens, float magneticSens)
{
	this->linearAccelSens = linearAccelSens;
	this->angularRateSens = angularRateSens;
	this->magneticSens = magneticSens;
}

float Lsm9ds1Hal::getLinearAccelSens()
{
	return linearAccelSens;
}

float Lsm9ds1Hal::getAngularRateSens()
{
	return angularRateSens;
}

float Lsm9ds1Hal::getMagneticSens()
{
	return magneticSens;
}

bool Lsm9ds1Hal::readGxlI2c(Register *reg, uint8_t data[], size_t bytesToRead)
{
	return this->readI2c(&LSM9DS1_I2C::GYRO_ACCEL, reg, data, bytesToRead);
}

bool Lsm9ds1Hal::writeGxlI2c(Register *reg, uint8_t data[], size_t bytesToWrite)
{
	return this->writeI2c(&LSM9DS1_I2C::GYRO_ACCEL, reg, data, bytesToWrite);
}

bool Lsm9ds1Hal::readMI2c(Register *reg, uint8_t data[], size_t bytesToRead)
{
	return this->readI2c(&LSM9DS1_I2C::MAGNETOMETER, reg, data, bytesToRead);
}

bool Lsm9ds1Hal::writeMI2c(Register *reg, uint8_t data[], size_t bytesToWrite)
{
	return this->writeI2c(&LSM9DS1_I2C::MAGNETOMETER, reg, data, bytesToWrite);
}
