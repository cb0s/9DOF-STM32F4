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
	CALIBRATE_ACCEL = 1,
	CALIBRATE_GYRO = 2,
	CALIBRATE_MAGN = 3,
	RADIO_SILENCE = 4,
};


#endif /* STATES_H_ */
