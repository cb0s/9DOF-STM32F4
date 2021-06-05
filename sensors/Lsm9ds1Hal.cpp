/*
 * Lsm9ds1.cpp
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */

#include "Lsm9ds1Hal.h"
#include "i2c/lsm9ds1_i2c.h"

Lsm9ds1Hal::Lsm9ds1Hal(HAL_I2C *device) : Hal(device)
{}

bool Lsm9ds1Hal::initLsm9ds1()
{
	return Hal::init(LSM9DS1_I2C::FREQ);
}

bool Lsm9ds1Hal::setup()
{
	return true;
}

bool Lsm9ds1Hal::setupI2c()
{

	return true;
}


bool Lsm9ds1Hal::readTemp(float &temp)
{
	return false;
}

bool Lsm9ds1Hal::readAcceleration(Vector3D &acc)
{
	return false;
}

bool Lsm9ds1Hal::readRotation(Vector3D &rot)
{
	return false;
}

bool Lsm9ds1Hal::readMagneticField(Vector3D &mag)
{
	return false;
}


bool inline Lsm9ds1Hal::readGxlI2c(Register* reg, uint8_t data[], size_t bytesToRead)
{
	return Hal::readI2c(&LSM9DS1_I2C::GYRO_ACCEL, reg, data, bytesToRead);
}

bool inline Lsm9ds1Hal::writeGxlI2c(Register* reg, uint8_t data[], size_t bytesToWrite)
{
	return Hal::writeI2c(&LSM9DS1_I2C::GYRO_ACCEL, reg, data, bytesToWrite);
}

bool inline Lsm9ds1Hal::readMI2c(Register* reg, uint8_t data[], size_t bytesToRead)
{
	return Hal::readI2c(&LSM9DS1_I2C::MAGNETOMETER, reg, data, bytesToRead);
}

bool inline Lsm9ds1Hal::writeMI2c(Register* reg, uint8_t data[], size_t bytesToWrite)
{
	return Hal::writeI2c(&LSM9DS1_I2C::MAGNETOMETER, reg, data, bytesToWrite);
}
