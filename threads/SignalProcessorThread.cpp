/*
 * SignalProcessorThread.cpp
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#include "SignalProcessorThread.h"

SignalProcessorThread::SignalProcessorThread(Lsm9ds1Hal *sensor, uint64_t delay, RODOS::HAL_GPIO *LED, const char *name)
	: ContinuousThread(delay, LED, name), SENSOR(sensor)
{}

SignalProcessorThread::~SignalProcessorThread()
{}

bool SignalProcessorThread::onLoop(uint64_t time)
{
	float temp = 0.0f;

	Vector3D accel;
	Vector3D gyro;
	Vector3D magn;

	SENSOR->readTemp(temp);
	SENSOR->readAcceleration(accel);
	SENSOR->readRotation(gyro);
	SENSOR->readMagneticField(magn);

	TELEMETRY::SYSTEM_T msg(accel, gyro, magn, temp);

	return false;
}

void SignalProcessorThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
