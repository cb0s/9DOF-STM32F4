/*
 * lsm9ds1_i2c.cpp
 *
 *  Created on: 04.06.2021
 *      Author: Cedric Boes
 */
#include "lsm9ds1_i2c.h"
#include "../registers/lsm9ds1_reg.h"

I2cDevice LSM9DS1_I2C::GYRO_ACCEL(0x6B, I2C_DEVICE_TYPE::SLAVE,
		const_cast<const Register**>(LSM9DS1_GXL_REGS::USED_GXL), LSM9DS1_GXL_REGS::SIZE_GXL);
I2cDevice LSM9DS1_I2C::MAGNETOMETER(0x1E, I2C_DEVICE_TYPE::SLAVE,
		const_cast<const Register**>(LSM9DS1_M_REGS::USED_M), LSM9DS1_M_REGS::SIZE_M);
