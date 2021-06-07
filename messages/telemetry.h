/*
 * telemetry.h
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#ifndef MESSAGES_TELEMETRY_H_
#define MESSAGES_TELEMETRY_H_

#include "rodos.h"
#include "matlib.h"
#include "messages.h"

namespace TELEMETRY {
	// Object Oriented programming in rodos goes BRRRRRRR :D
	struct TELEMETRY_MSG {
		TELEMETRY_MSG(const uint8_t msgId) : MSG_ID(msgId)
		{}

		const uint8_t MSG_ID;
	};

	struct SYSTEM_T : TELEMETRY_MSG {
		SYSTEM_T(const Vector3D &accel, const Vector3D &gyro, const Vector3D &magnet, float temp)
			: TELEMETRY_MSG(MSG_ID), ACCEL(accel), GYRO(gyro), MAGNET(magnet), TEMP(temp)
		{}

		const Vector3D ACCEL;
		const Vector3D GYRO;
		const Vector3D MAGNET;
		const float TEMP;

		static const uint8_t MSG_ID = 0x4;
	};

	struct CALIBRATION_DATA : TELEMETRY_MSG {
		CALIBRATION_DATA(const Vector3D &accelOffset, const Vector3D &gyroOffset, const Vector3D &magnetOffset)
			: TELEMETRY_MSG(MSG_ID), ACCEL_OFFSET(accelOffset), GYRO_OFFSET(gyroOffset), MAGNET_OFFSET(magnetOffset)
		{}

		const Vector3D ACCEL_OFFSET;
		const Vector3D GYRO_OFFSET;
		const Vector3D MAGNET_OFFSET;

		static const uint8_t MSG_ID = 0x5;
	};
};



#endif /* MESSAGES_TELEMETRY_H_ */
