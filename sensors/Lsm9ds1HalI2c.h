/*
 * Lsm9ds1.h
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */
#ifndef SENSORS_LSM9DS1HAL_H_
#define SENSORS_LSM9DS1HAL_H_

#include "rodos.h"
#include "matlib.h"
#include "../utils.h"
#include "HalI2c.h"
#include "constants/lsm9ds1_const.h"
#include "i2c/lsm9ds1_i2c.h"
#include "registers/lsm9ds1_reg.h"

class Lsm9ds1HalI2c : public HalI2c {
public:
	Lsm9ds1HalI2c(HAL_I2C *device);
	virtual ~Lsm9ds1HalI2c();

	bool initLsm9ds1();

	bool setup() override;
	bool setupI2c() override;

	bool readTemp(float &temp);
	bool readAcceleration(Vector3D &acc);
	bool readRotation(Vector3D &rot);
	bool readMagneticField(Vector3D &mag);

	void setNormalizationValues(float linearAccelSens, float angularRateSens, float magneticSens);
	float getLinearAccelSens();
	float getAngularRateSens();
	float getMagneticSens();

private:
	// Blocking operation
	bool detectSensor(I2cDevice *device, Register *whoAmI, uint8_t expectedValue);

	bool inline readGxlI2c(Register* reg, uint8_t *data, size_t bytesToRead);
	bool inline writeGxlI2c(Register* reg, uint8_t *data, size_t bytesToWrite);
	bool inline readMI2c(Register* reg, uint8_t *data, size_t bytesToRead);
	bool inline writeMI2c(Register* reg, uint8_t *data, size_t bytesToWrite);

	uint8_t dataBuffer[6];

	float linearAccelSens;
	float angularRateSens;
	float magneticSens;
};

#endif /* SENSORS_LSM9DS1HAL_H_ */
