/*
 * lsm9ds1_const.h
 *
 *  Created on: 05.06.2021
 *      Author: Cedric Boes
 */

#ifndef SENSORS_CONSTANTS_LSM9DS1_CONST_H_
#define SENSORS_CONSTANTS_LSM9DS1_CONST_H_

#include "rodos.h"

// Constants from the LSM9DS1 data sheet which can be found here: https://www.st.com/resource/en/datasheet/lsm9ds1.pdf
namespace LSM9DS1_GENERAL_CONSTANTS {
	// Page 14
	extern float NORM_TEMP;
	extern float TEMP_NORM_FACTOR;

	// Page 12
	extern float ACCEL_LIN_SENS_2G;
	extern float ACCEL_LIN_SENS_4G;
	extern float ACCEL_LIN_SENS_8G;
	extern float ACCEL_LIN_SENS_16G;

	extern float ANGULAR_RATE_SENS_245DPS;
	extern float ANGULAR_RATE_SENS_500DPS;
	extern float ANGULAR_RATE_SENS_2000DPS;

	extern float MAGNETIC_SENS_4GAUSS;
	extern float MAGNETIC_SENS_8GAUSS;
	extern float MAGNETIC_SENS_12GAUSS;
	extern float MAGNETIC_SENS_16GAUSS;

	extern uint8_t SLAVE_ADDRESS_GXL;
	extern uint8_t SLAVE_ADDRESS_M;
}

#endif /* SENSORS_CONSTANTS_LSM9DS1_CONST_H_ */
