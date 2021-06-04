/*
 * Lsm9ds1.h
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */
#ifndef SENSORS_LSM9DS1HAL_H_
#define SENSORS_LSM9DS1HAL_H_

#include "Hal.h"
#include "matlib.h"

class Lsm9ds1Hal : Hal {
public:
	Lsm9ds1Hal();
	virtual ~Lsm9ds1Hal();

	void readTemp(float &temp);
	void readAcceleration(Vector3D &acc);
	void readRotation(Vector3D &rot);
	void readMagneticField(Vector3D &mag);
};

#endif /* SENSORS_LSM9DS1HAL_H_ */
