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

class Lsm9ds1Hal : public Hal {
public:
	Lsm9ds1Hal(HAL_I2C *device);
	virtual ~Lsm9ds1Hal();

	bool initLsm9ds1();

	bool setup() override;
	bool setupI2c() override;

	bool readTemp(float &temp);
	bool readAcceleration(Vector3D &acc);
	bool readRotation(Vector3D &rot);
	bool readMagneticField(Vector3D &mag);

private:
	bool inline readGxlI2c(Register* reg, uint8_t data[], size_t bytesToRead);
	bool inline writeGxlI2c(Register* reg, uint8_t data[], size_t bytesToWrite);
	bool inline readMI2c(Register* reg, uint8_t data[], size_t bytesToRead);
	bool inline writeMI2c(Register* reg, uint8_t data[], size_t bytesToWrite);
};

#endif /* SENSORS_LSM9DS1HAL_H_ */
