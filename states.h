/*
 * states.h
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#ifndef STATES_H_
#define STATES_H_

#include "rodos.h"

enum BOARD_STATE : uint8_t {
	NORMAL = 0,

	CALIBRATE_ACCEL = 10,
	CALIBRATE_ACCEL_X = 11,
	CALIBRATE_ACCEL_Y = 12,
	CALIBRATE_ACCEL_Z = 13,

	CALIBRATE_GYRO = 20,

	CALIBRATE_MAGN = 30,
	CALIBRATE_MAGN_X = 31,
	CALIBRATE_MAGN_Y = 32,
	CALIBRATE_MAGN_Z = 33,

	CALIBRATE_WARN = 40,
	CALIBRATE_FINAL = 41,

	RADIO_SILENCE = 50,
};


#endif /* STATES_H_ */
