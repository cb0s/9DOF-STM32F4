/*
 * internal.h
 *
 *  Created on: 11.06.2021
 *      Author: Cedric Boes
 */

#ifndef MESSAGES_INTERNAL_H_
#define MESSAGES_INTERNAL_H_

#include "rodos.h"
#include "matlib.h"

namespace INTERNAL_MSG {
	struct MEASUREMENT {
		MEASUREMENT()
			: accel(Vector3D(0, 0, 0)),
			  gyro(Vector3D(0, 0, 0)), GYRO_SPEED(Vector3D(0, 0, 0)), GYRO_GAUSS(Vector3D(0, 0, 0)),
			  magnet({0, 0, 0}),
			  rotMatrix(Matrix3D({0, 0, 0}, {0, 0, 0}, {0, 0, 0})), temp(0.0f), internalTime(0)
		{}

		MEASUREMENT(Vector3D &accel,
				Vector3D &gyro,
				Vector3D &GYRO_SPEED,
				Vector3D &GYRO_GAUSS,
				Vector3D &magnet,
				Matrix3D &rotMatrix,
				float &temp,
				uint64_t &internalTime)
			: accel(accel),
			  gyro(gyro),
			  GYRO_SPEED(GYRO_SPEED),
			  GYRO_GAUSS(GYRO_GAUSS),
			  magnet(magnet),
			  rotMatrix(rotMatrix),
			  temp(temp),
			  internalTime(internalTime)
		{}

		Vector3D accel;
		Vector3D gyro;
		Vector3D GYRO_SPEED;
		Vector3D GYRO_GAUSS;
		Vector3D magnet;
		Matrix3D rotMatrix;
		float temp;
		uint64_t internalTime;
	};

	struct CALIBRATION {
		CALIBRATION()
			: ACCEL_OFFSET(Vector3D(0, 0, 0)),
			  GYRO_OFFSET(Vector3D(0, 0, 0)),
			  MAGNET_OFFSET_MIN(Vector3D(0, 0, 0)), MAGNET_OFFSET_MAX(Vector3D(0, 0, 0))
		{}

		CALIBRATION(const Vector3D &accelOffset,
				const Vector3D &gyroOffset,
				const Vector3D &magnetOffsetMin,
				const Vector3D &magnetOffsetMax)
			: ACCEL_OFFSET(accelOffset),
			  GYRO_OFFSET(gyroOffset),
			  MAGNET_OFFSET_MIN(magnetOffsetMin),
			  MAGNET_OFFSET_MAX(magnetOffsetMax)
		{}

		Vector3D ACCEL_OFFSET;
		Vector3D GYRO_OFFSET;
		Vector3D MAGNET_OFFSET_MIN;
		Vector3D MAGNET_OFFSET_MAX;
	};
};



#endif /* MESSAGES_INTERNAL_H_ */
