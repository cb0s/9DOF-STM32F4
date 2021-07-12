/*
 * Lsm9ds1HalSpi.h
 *
 *  Created on: 08.07.2021
 *      Author: Cedric Boes
 */

#ifndef SENSORS_LSM9DS1HALSPI_H_
#define SENSORS_LSM9DS1HALSPI_H_

#include "HalSpi.h"
#include "constants/lsm9ds1_const.h"
#include "spi/lsm9ds1_spi.h"
#include "registers/lsm9ds1_reg.h"

class Lsm9ds1HalSpi : public HalSpi {
public:
	Lsm9ds1HalSpi(HAL_SPI *device, HAL_GPIO *gxl, HAL_GPIO *magn);
	virtual ~Lsm9ds1HalSpi();

	bool initLsm9ds1();

	bool setup() override;
	bool setupSpi() override;

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
	bool detectSensor(HAL_GPIO *sensor, SpiDevice *device, Register *whoAmI, uint8_t expectedValue);

	bool inline readGxlSpi(Register* reg, uint8_t *data, size_t bytesToRead);
	bool inline writeGxlSpi(Register* reg, uint8_t *data, size_t bytesToWrite);
	bool inline readMSpi(Register* reg, uint8_t *data, size_t bytesToRead);
	bool inline writeMSpi(Register* reg, uint8_t *data, size_t bytesToWrite);

	uint8_t dataBuffer[6];

	float linearAccelSens;
	float angularRateSens;
	float magneticSens;

	HAL_GPIO *gxl;
	HAL_GPIO *magn;
};

#endif /* SENSORS_LSM9DS1HALSPI_H_ */
