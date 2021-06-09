/*
 * SignalProcessorThread.cpp
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#include "SignalProcessorThread.h"

SignalProcessorThread::SignalProcessorThread(
		Lsm9ds1Hal *sensor,
		TELEMETRY::SYSTEM_T *signalData,
		TELEMETRY::CALIBRATION_DATA *calibrationData,
		TELEMETRY::READING_ERROR *readingErrorData,
		TELEMETRY::ALIVE_SIGNAL *heartBeat,
		RODOS::CommBuffer<BOARD_STATE> *stateBuffer,
		RODOS::CommBuffer<uint64_t> *signalIntervalBuffer,

		uint64_t DELAY,
		RODOS::HAL_GPIO *LED,
		const char *name)
	: ContinuousThread(DELAY, LED, name),
	  SENSOR(sensor),
	  signalData(signalData),
	  calibrationData(calibrationData),
	  readingErrorData(readingErrorData),
	  heartBeat(heartBeat),
	  stateBuffer(stateBuffer),
	  signalIntervalBuffer(signalIntervalBuffer),
	  lastRead(0),
	  radioSilenceSent(false),
	  state(BOARD_STATE::NORMAL)
{}

SignalProcessorThread::~SignalProcessorThread()
{}

bool SignalProcessorThread::onLoop(uint64_t time)
{
	if (this->stateBuffer->getOnlyIfNewData(state))
	{
		signalData->STATE = state;
		calibrationData->STATE = state;
		readingErrorData->STATE = state;
		heartBeat->STATE = state;
	}

	switch(state)
	{
	case BOARD_STATE::RADIO_SILENCE:
		if (!radioSilenceSent)
		{
			radioSilenceSent = false;
			TOPICS::TELEMETRY_TOPIC.publish(*heartBeat);
		}
		break;
	case BOARD_STATE::NORMAL:
		if (measure(time))
		{
			TOPICS::TELEMETRY_TOPIC.publish(*readingErrorData);
		}
		else
		{
			TOPICS::TELEMETRY_TOPIC.publish(*signalData);
		}
		break;
	case BOARD_STATE::CALIBRATE_ACCEL:
		break;
	case BOARD_STATE::CALIBRATE_GYRO:
		break;
	case BOARD_STATE::CALIBRATE_MAGN:
		break;
	case BOARD_STATE::CALIBRATE_FINAL:
		TOPICS::TELEMETRY_TOPIC.publish(*calibrationData);
		break;
	}

	return true;
}

bool SignalProcessorThread::measure(uint64_t time)
{
	Vector3D accel;
	Vector3D rot;
	Vector3D flux;

	float temp = 0.0f;

	if (!(SENSOR->readTemp(temp) && SENSOR->readAcceleration(accel) && SENSOR->readRotation(rot) &&	SENSOR->readMagneticField(flux)))
	{
		this->readingErrorData->INTERNAL_TIME = time;
		return false;
	}

	accel = accel - this->calibrationData->ACCEL_OFFSET;
	rot = rot - this->calibrationData->GYRO_OFFSET;

	flux.x = (flux.x-this->calibrationData->MAGNET_OFFSET_MIN.x) /
			(this->calibrationData->MAGNET_OFFSET_MAX.x-this->calibrationData->MAGNET_OFFSET_MIN.x) * 2 - 1;
	flux.y = (flux.y-this->calibrationData->MAGNET_OFFSET_MIN.y) /
			(this->calibrationData->MAGNET_OFFSET_MAX.y-this->calibrationData->MAGNET_OFFSET_MIN.y) * 2 - 1;
	flux.z = (flux.z-this->calibrationData->MAGNET_OFFSET_MIN.z) /
			(this->calibrationData->MAGNET_OFFSET_MAX.z-this->calibrationData->MAGNET_OFFSET_MIN.z) * 2 - 1;

	uint64_t deltaT = lastRead == 0 ? 0 : lastRead - time;

	// Coordinate System
	Vector3D z = accel.normalize() * -1.0f;
	Vector3D y = z.cross(flux.normalize());
	Vector3D x = y.cross(z);

	Matrix3D rotMatrix(x, y, z);
	RPY rotEul = rotMatrix.toRPY();

	float sinRoll = sin(rotEul.getRoll());
	float cosRoll = cos(rotEul.getRoll());
	float tanPitch = tan(rotEul.getPitch());
	float cosPitch = cos(rotEul.getPitch());

	Matrix3D gyroMatrix({0, 1, 0},
			{cosRoll, sinRoll * tanPitch, sinRoll / cosPitch},
			{-sinRoll, cosRoll * tanPitch, cosRoll / cosPitch});

	RPY dtRotEul = gyroMatrix * rot;

	this->signalData->ACCEL = accel;
	this->signalData->GYRO = rot;
	this->signalData->GYRO_SPEED = dtRotEul;
	this->signalData->GYRO_GAUSS = this->signalData->GYRO + dtRotEul * deltaT;
	this->signalData->ROT_MATRIX = rotMatrix;
	this->signalData->MAGNET = flux;
	this->signalData->TEMP = temp;

	this->signalData->INTERNAL_TIME = time;

	return true;
}

void SignalProcessorThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
