/*
 * states.h
 *
 *  Created on: 07.06.2021
 *      Author: Cedric
 */

#ifndef STATES_H_
#define STATES_H_

#include "rodos.h"

enum BOARD_STATE : uint8_t {
	NORMAL = 0,
	CALIBRATE_ACCEL = 10,
	CALIBRATE_GYRO = 11,
	CALIBRATE_MAGN = 12,
	CALIBRATE_FINAL = 13,
	RADIO_SILENCE = 20,
};


#endif /* STATES_H_ */
