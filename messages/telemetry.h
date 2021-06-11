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
#include"../states.h"

namespace TELEMETRY {
	// Object Oriented programming in rodos goes BRRRRRRR :D
	struct TELEMETRY_MSG {
		TELEMETRY_MSG() : TELEMETRY_MSG(255, 255, BOARD_STATE::RADIO_SILENCE)
		{}

		TELEMETRY_MSG(uint8_t msgId, uint64_t internalTime, BOARD_STATE state)
			: MSG_ID(msgId), INTERNAL_TIME(internalTime), STATE(state)
		{}

		uint8_t MSG_ID;
		uint64_t INTERNAL_TIME;
		BOARD_STATE STATE;
	};

	struct SYSTEM_T : TELEMETRY_MSG {
		SYSTEM_T(Vector3D &accel,
				Vector3D &gyro,
				Vector3D &GYRO_SPEED,
				Vector3D &GYRO_GAUSS,
				Vector3D &magnet,
				Matrix3D &rotMatrix,
				float &temp,
				uint64_t &internalTime)
			: TELEMETRY_MSG(MSG_ID, internalTime, BOARD_STATE::NORMAL),
			  ACCEL(accel),
			  GYRO(gyro),
			  GYRO_SPEED(GYRO_SPEED),
			  GYRO_GAUSS(GYRO_GAUSS),
			  MAGNET(magnet),
			  ROT_MATRIX(rotMatrix),
			  TEMP(temp)
		{}

		Vector3D ACCEL;
		Vector3D GYRO;
		Vector3D GYRO_SPEED;
		Vector3D GYRO_GAUSS;
		Vector3D MAGNET;
		Matrix3D ROT_MATRIX;
		float TEMP;

		static const uint8_t MSG_ID = 0x10;
	};

	struct CALIBRATION_DATA : TELEMETRY_MSG {
		CALIBRATION_DATA(const Vector3D &accelOffset,
				const Vector3D &gyroOffset,
				const Vector3D &magnetOffsetMin,
				const Vector3D &magnetOffsetMax,
				const uint64_t &internalTime)
			: TELEMETRY_MSG(MSG_ID, internalTime, BOARD_STATE::CALIBRATE_FINAL),
			  ACCEL_OFFSET(accelOffset),
			  GYRO_OFFSET(gyroOffset),
			  MAGNET_OFFSET_MIN(magnetOffsetMin),
			  MAGNET_OFFSET_MAX(magnetOffsetMax)
		{}

		Vector3D ACCEL_OFFSET;
		Vector3D GYRO_OFFSET;
		Vector3D MAGNET_OFFSET_MIN;
		Vector3D MAGNET_OFFSET_MAX;

		static const uint8_t MSG_ID = 0x11;
	};

	struct READING_ERROR : TELEMETRY_MSG {
		READING_ERROR(uint64_t &measureTime)
			: TELEMETRY_MSG(MSG_ID, measureTime, BOARD_STATE::NORMAL)
		{}

		static const uint8_t MSG_ID = 0x12;
	};

	struct ALIVE_SIGNAL : TELEMETRY_MSG {
		ALIVE_SIGNAL(uint64_t &measureTime)
			: TELEMETRY_MSG(MSG_ID, measureTime, BOARD_STATE::NORMAL)
		{}

		static const uint8_t MSG_ID = 0x13;
	};

	extern bool operator == (TELEMETRY_MSG &msg1, TELEMETRY_MSG &msg2);
};



#endif /* MESSAGES_TELEMETRY_H_ */
