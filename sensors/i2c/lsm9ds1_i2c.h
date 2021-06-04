/*
 * lsm9ds1_i2c.h
 *
 *  Created on: 04.06.2021
 *      Author: Cedric Boes
 */

#ifndef SENSORS_I2C_LSM9DS1_I2C_H_
#define SENSORS_I2C_LSM9DS1_I2C_H_

#include "i2c.h"

namespace LSM9DS1_I2C {
	extern I2cDevice GYRO_ACCEL;
	extern I2cDevice MAGNETOMETER;
}

#endif /* SENSORS_I2C_LSM9DS1_I2C_H_ */
