/*
 * SignalProcessorThread.cpp
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#include "SignalProcessorThread.h"

SignalProcessorThread::SignalProcessorThread(
		Lsm9ds1Hal *sensor,
		uint32_t i2cFreq,

		INTERNAL_MSG::MEASUREMENT *dat,
		INTERNAL_MSG::CALIBRATION *cal,
		TELEMETRY::TELEMETRY_MSG *msg,
		RODOS::CommBuffer<BOARD_STATE> *stateBuffer,
		RODOS::CommBuffer<uint64_t> *signalIntervalBuffer,

		RODOS::HAL_GPIO *calibrationLed,

		uint64_t DELAY,
		RODOS::HAL_GPIO *LED,
		const char *name)
	: ContinuousThread(DELAY, LED, name),
		sensor(sensor),
		i2cFreq(i2cFreq),
		msg(msg),
		dat(dat),
		cal(cal),
		stateBuffer(stateBuffer),
		signalIntervalBuffer(signalIntervalBuffer),
		lastRead(0),
		radioSilenceSent(false),
		state(BOARD_STATE::NORMAL),
		calibrationLed(calibrationLed),
		calibrationProcess(0),
		calBlinky(0)
{}

SignalProcessorThread::~SignalProcessorThread()
{}

void SignalProcessorThread::prepare()
{
	sensor->init(i2cFreq);
	msg->STATE = BOARD_STATE::NORMAL;
}

bool SignalProcessorThread::onLoop(uint64_t time)
{
	if (this->stateBuffer->getOnlyIfNewData(state))
	{
		msg->STATE = state;
		radioSilenceSent = false;
	}

	this->signalIntervalBuffer->getOnlyIfNewData(DELAY);

	bool error = false;
	msg->INTERNAL_TIME = time;
	msg->MSG_ID = TELEMETRY::ALIVE_SIGNAL::MSG_ID;

	switch(state)
	{
	case BOARD_STATE::RADIO_SILENCE:		// That the sender knows RADIO-Silence was received
		if (!radioSilenceSent)
		{
			radioSilenceSent = true;
			TOPICS::TELEMETRY_TOPIC.publish(*msg);
		}
		return true;
	case BOARD_STATE::REQUEST_CAL_INFO:
		TOPICS::TELEMETRY_TOPIC.publish(*msg);
		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::NORMAL);
		return true;
	case BOARD_STATE::NORMAL:
		if (!measure(time))
		{
			msg->MSG_ID = TELEMETRY::READING_ERROR::MSG_ID;
			TOPICS::TELEMETRY_TOPIC.publish(*msg);
		}
		else
		{
			msg->MSG_ID = TELEMETRY::SYSTEM_T::MSG_ID;
			TOPICS::CURRENT_DAT_TOPIC.publish(*dat);
			TOPICS::TELEMETRY_TOPIC.publish(*msg);
		}
		return true;
	case BOARD_STATE::CALIBRATE_ACCEL:
		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_ACCEL_X);
		break;
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
		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_MAGN_X);
		break;
	case BOARD_STATE::CALIBRATE_MAGN_X:
	case BOARD_STATE::CALIBRATE_MAGN_Y:
	case BOARD_STATE::CALIBRATE_MAGN_Z:
		error = !calibrateMagn(time);
		break;
	case BOARD_STATE::CALIBRATE_WARN:
		calBlinky++;
		calibrationLed->setPins(~calibrationLed->readPins());
		if (calBlinky % CALIBRATION_BLINKS == 0)
		{
			calBlinky = 0;
			calibrationLed->setPins(false);

			TOPICS::SYSTEM_STATE_TOPIC.publish(calState);
		}
		break;
	case BOARD_STATE::CALIBRATE_FINAL:
		this->calibrationProcess = 0;
		this->msg->INTERNAL_TIME = time;
		for (uint8_t i = 0; i < 3; i++)
		{
			this->calVals[i].x = 0;
			this->calVals[i].y = 0;
			this->calVals[i].z = 0;
		}

		TOPICS::CURRENT_CAL_TOPIC.publish(*cal);
		msg->MSG_ID = TELEMETRY::CALIBRATION_DATA::MSG_ID;
		TOPICS::TELEMETRY_TOPIC.publish(*msg);

		LED->setPins(true);
		return true;
	default:
		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::NORMAL);
	}

	// Being here means being in the calibration process
	if (error)
	{
		msg->MSG_ID = TELEMETRY::READING_ERROR::MSG_ID;
		TOPICS::TELEMETRY_TOPIC.publish(*msg);
	}
	else
	{
		TOPICS::TELEMETRY_TOPIC.publish(*msg);
	}

	return true;
}

bool SignalProcessorThread::measure(uint64_t time)
{
	Vector3D accel;
	Vector3D rot;
	Vector3D flux;

	float temp = 0.0f;

	if (!(sensor->readTemp(temp) && sensor->readAcceleration(accel) && sensor->readRotation(rot) &&	sensor->readMagneticField(flux)))
	{
		this->msg->INTERNAL_TIME = time;
		return false;
	}

	accel = accel - this->cal->ACCEL_OFFSET;
	rot = rot - this->cal->GYRO_OFFSET;

	flux.x = (flux.x-this->cal->MAGNET_OFFSET_MIN.x) /
			(this->cal->MAGNET_OFFSET_MAX.x-this->cal->MAGNET_OFFSET_MIN.x) * 2 - 1;
	flux.y = (flux.y-this->cal->MAGNET_OFFSET_MIN.y) /
			(this->cal->MAGNET_OFFSET_MAX.y-this->cal->MAGNET_OFFSET_MIN.y) * 2 - 1;
	flux.z = (flux.z-this->cal->MAGNET_OFFSET_MIN.z) /
			(this->cal->MAGNET_OFFSET_MAX.z-this->cal->MAGNET_OFFSET_MIN.z) * 2 - 1;

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
	RPY finalRot = this->dat->GYRO_GAUSS + dtRotEul * deltaT;

	// Tilt compensation
	Vector3D correctedFlux;
	correctedFlux.x = flux.x * cosPitch + flux.z * sinPitch;
	correctedFlux.y = flux.x * sinRoll * sinPitch + flux.y * cosRoll - flux.z * sinRoll * cosPitch;
	correctedFlux.z = flux.z;

	this->dat->accel = accel;
	this->dat->gyro = rot;
	this->dat->GYRO_SPEED = dtRotEul;
	this->dat->GYRO_GAUSS = finalRot;
	this->dat->rotMatrix = rotMatrix;
	this->dat->magnet = correctedFlux;
	this->dat->temp = temp;

	this->dat->internalTime = time;

	return true;
}

bool SignalProcessorThread::calibrateAccel(uint64_t time)
{
	toggleLed();

	Vector3D vec;
	if (!sensor->readAcceleration(vec))
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
		this->cal->ACCEL_OFFSET.x = (this->calVals[0].x + this->calVals[2].x) / (CALIBRATION_SAMPLES * 2);
		this->cal->ACCEL_OFFSET.y = (this->calVals[0].y + this->calVals[1].y) / (CALIBRATION_SAMPLES * 2);
		this->cal->ACCEL_OFFSET.z = (this->calVals[1].z + this->calVals[2].z) / (CALIBRATION_SAMPLES * 2);

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_FINAL);
	}

	return true;
}

bool SignalProcessorThread::calibrateGyro(uint64_t time)
{
	toggleLed();

	Vector3D vec;
	if (!sensor->readRotation(vec))
	{
		return false;
	}

	this->calVals[0] = this->calVals[0] + vec;

	calibrationProcess++;
	if (calibrationProcess == CALIBRATION_SAMPLES)
	{
		this->cal->GYRO_OFFSET = this->calVals[0] / CALIBRATION_SAMPLES;
	}

	return true;
}

bool SignalProcessorThread::calibrateMagn(uint64_t time)
{
	toggleLed();

	Vector3D vec;
	if (!sensor->readMagneticField(vec))
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
		cal->MAGNET_OFFSET_MIN.x = calVals[0].x;
		cal->MAGNET_OFFSET_MIN.y = calVals[0].y;
		cal->MAGNET_OFFSET_MIN.z = calVals[0].z;

		cal->MAGNET_OFFSET_MAX.x = calVals[1].x;
		cal->MAGNET_OFFSET_MAX.y = calVals[1].y;
		cal->MAGNET_OFFSET_MAX.z = calVals[1].z;

		TOPICS::SYSTEM_STATE_TOPIC.publishConst(BOARD_STATE::CALIBRATE_FINAL);
	}

	return true;
}

void SignalProcessorThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
