/*
 * lsm9ds1_spi.cpp
 *
 *  Created on: 08.07.2021
 *      Author: Cedric Boes
 */

#include "lsm9ds1_spi.h"
#include "../registers/lsm9ds1_reg.h"

SpiDevice LSM9DS1_SPI::GYRO_ACCEL(SPI_DEVICE_TYPE::SLAVE,
		const_cast<const Register**>(LSM9DS1_GXL_REGS::USED_GXL), LSM9DS1_GXL_REGS::SIZE_GXL);
SpiDevice LSM9DS1_SPI::MAGNETOMETER(SPI_DEVICE_TYPE::SLAVE,
		const_cast<const Register**>(LSM9DS1_M_REGS::USED_M), LSM9DS1_M_REGS::SIZE_M);

uint32_t LSM9DS1_SPI::FREQ = 1000000; // 1_000_000

