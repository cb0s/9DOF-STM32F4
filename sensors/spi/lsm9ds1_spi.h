/*
 * lsm9ds1_spi.h
 *
 *  Created on: 08.07.2021
 *      Author: Cedric Boes
 */

#ifndef SENSORS_SPI_LSM9DS1_SPI_H_
#define SENSORS_SPI_LSM9DS1_SPI_H_

#include "spi.h"

namespace LSM9DS1_SPI {
	extern SpiDevice GYRO_ACCEL;
	extern SpiDevice MAGNETOMETER;

	extern uint32_t FREQ;
}

#endif /* SENSORS_SPI_LSM9DS1_SPI_H_ */
