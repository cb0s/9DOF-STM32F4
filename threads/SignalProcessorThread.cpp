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
	  state(BOARD_STATE::NORMAL),
	  calibrationProcess(0)
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

	bool error = false;
	heartBeat->INTERNAL_TIME = time;
	readingErrorData->INTERNAL_TIME = time;

	switch(state)
	{
	case BOARD_STATE::RADIO_SILENCE:		// That the sender knows RADIO-Silence was received
		if (!radioSilenceSent)
		{
			radioSilenceSent = false;
			TOPICS::TELEMETRY_TOPIC.publish(*heartBeat);
		}
		return true;
	case BOARD_STATE::NORMAL:
		if (measure(time))
		{
			TOPICS::TELEMETRY_TOPIC.publish(*readingErrorData);
		}
		else
		{
			TOPICS::TELEMETRY_TOPIC.publish(*signalData);
		}
		return true;
	case BOARD_STATE::CALIBRATE_ACCEL:
	case BOARD_STATE::CALIBRATE_ACCEL_X:
	case BOARD_STATE::CALIBRATE_ACCEL_Y:
	case BOARD_STATE::CALIBRATE_ACCEL_Z:
		error = !calibrateAccel(time);
		break;
	case BOARD_STATE::CALIBRATE_GYRO:
		error = !calibrateGyro(time);
		break;
	case BOARD_STATE::CALIBRATE_MAGN:
		this->calVals[0].x = 0b01111111100000000000000000000000;	// = Inf
		this->calVals[0].y = 0b01111111100000000000000000000000;	// = Inf
		this->calVals[0].z = 0b01111111100000000000000000000000;	// = Inf
		// Fall through intentional
	case BOARD_STATE::CALIBRATE_MAGN_X:
	case BOARD_STATE::CALIBRATE_MAGN_Y:
	case BOARD_STATE::CALIBRATE_MAGN_Z:
		error = !calibrateMagn(time);
		break;
	case BOARD_STATE::CALIBRATE_WARN:
		calibrationProcess++;
		calibrationLed.setPins(~calibrationLed.readPins());
		if (calibrationProcess % CALIBRATION_SAMPLES == CALIBRATION_BLINKS)
		{
			calibrationProcess -= CALIBRATION_BLINKS;
			calibrationLed.setPins(false);

			TOPICS::SYSTEM_STATE_TOPIC.publish(calState);
		}
		break;
	case BOARD_STATE::CALIBRATE_FINAL:
		this->calibrationProcess = 0;
		this->calibrationData->INTERNAL_TIME = time;
		for (uint8_t i = 0; i < 3; i++)
		{
			this->calVals[i].x = 0;
			this->calVals[i].y = 0;
			this->calVals[i].z = 0;
		}
		TOPICS::TELEMETRY_TOPIC.publish(*calibrationData);

		LED->setPins(true);
		return true;
	}

	// Being here means being in the calibration process
	if (error)
	{
		TOPICS::TELEMETRY_TOPIC.publish(*readingErrorData);
	}
	else
	{
		TOPICS::TELEMETRY_TOPIC.publish(*heartBeat);
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
	float sinPitch = sin(rotEul.getPitch());
	float cosPitch = cos(rotEul.getPitch());
	float tanPitch = sinPitch / cosPitch;

	Matrix3D gyroMatrix({0, 1, 0},
			{cosRoll, sinRoll * tanPitch, sinRoll / cosPitch},
			{-sinRoll, cosRoll * tanPitch, cosRoll / cosPitch});

	RPY dtRotEul = gyroMatrix * rot;
	RPY finalRot = this->signalData->GYRO_GAUSS + dtRotEul * deltaT;

	// Tilt compensation
	Vector3D correctedFlux;
	correctedFlux.x = flux.x * cosPitch + flux.z * sinPitch;
	correctedFlux.y = flux.x * sinRoll * sinPitch + flux.y * cosRoll - flux.z * sinRoll * cosPitch;
	correctedFlux.z = flux.z;

	this->signalData->ACCEL = accel;
	this->signalData->GYRO = rot;
	this->signalData->GYRO_SPEED = dtRotEul;
	this->signalData->GYRO_GAUSS = finalRot;
	this->signalData->ROT_MATRIX = rotMatrix;
	this->signalData->MAGNET = correctedFlux;
	this->signalData->TEMP = temp;

	this->signalData->INTERNAL_TIME = time;

	return true;
}

bool SignalProcessorThread::calibrateAccel(uint64_t time)
{
	toggleLed();

	Vector3D vec;
	if (!SENSOR->readAcceleration(vec))
	{
		return false;
	}

	uint8_t index = 255;	// Will definitely be changed to be <3 or exited
	if (this->state == BOARD_STATE::CALIBRATE_ACCEL_X)
	{
		index = 0;
	}
	else if (this->state == BOARD_STATE::CALIBRATE_ACCEL_Y)
	{
		index = 1;
	}
	else if (this->state == BOARD_STATE::CALIBRATE_ACCEL_Z)
	{
		index = 2;
	}
	else
	{
		return false;
	}

	this->calVals[index] = this->calVals[index] + vec;

	calibrationProcess++;
	if (calibrationProcess == CALIBRATION_SAMPLES)
	{
		calState = BOARD_STATE::CALIBRATE_ACCEL_Y;

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_WARN);
	}
	else if (calibrationProcess == 2*CALIBRATION_SAMPLES)
	{
		calState = BOARD_STATE::CALIBRATE_ACCEL_Z;

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_WARN);
	}
	else if (calibrationProcess == 3*CALIBRATION_SAMPLES)
	{
		this->calibrationData->ACCEL_OFFSET.x = (this->calVals[0].x + this->calVals[2].x) / (CALIBRATION_SAMPLES * 2);
		this->calibrationData->ACCEL_OFFSET.y = (this->calVals[0].y + this->calVals[1].y) / (CALIBRATION_SAMPLES * 2);
		this->calibrationData->ACCEL_OFFSET.z = (this->calVals[1].z + this->calVals[2].z) / (CALIBRATION_SAMPLES * 2);

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_FINAL);
	}

	return true;
}

bool SignalProcessorThread::calibrateGyro(uint64_t time)
{
	toggleLed();

	Vector3D vec;
	if (!SENSOR->readRotation(vec))
	{
		return false;
	}

	this->calVals[0] = this->calVals[0] + vec;

	calibrationProcess++;
	if (calibrationProcess == CALIBRATION_SAMPLES)
	{
		this->calibrationData->GYRO_OFFSET = this->calVals[0] / CALIBRATION_SAMPLES;
	}

	return true;
}

bool SignalProcessorThread::calibrateMagn(uint64_t time)
{
	toggleLed();

	Vector3D vec;
	if (!SENSOR->readMagneticField(vec))
	{
		return false;
	}

	UTILS::minimizedVector(calVals[0], vec);
	UTILS::maximizedVector(calVals[1], vec);

	calibrationProcess++;

	if (calibrationProcess == CALIBRATION_SAMPLES)
	{
		calState = BOARD_STATE::CALIBRATE_MAGN_Y;

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_WARN);
	}
	else if (calibrationProcess == 2*CALIBRATION_SAMPLES)
	{
		calState = BOARD_STATE::CALIBRATE_MAGN_Z;

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_WARN);
	}
	else if (calibrationProcess == 3*CALIBRATION_SAMPLES)
	{
		calibrationData->MAGNET_OFFSET_MIN.x = calVals[0].x;
		calibrationData->MAGNET_OFFSET_MIN.y = calVals[0].y;
		calibrationData->MAGNET_OFFSET_MIN.z = calVals[0].z;

		calibrationData->MAGNET_OFFSET_MAX.x = calVals[1].x;
		calibrationData->MAGNET_OFFSET_MAX.y = calVals[1].y;
		calibrationData->MAGNET_OFFSET_MAX.z = calVals[1].z;

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_FINAL);
	}

	return true;
}

void SignalProcessorThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
