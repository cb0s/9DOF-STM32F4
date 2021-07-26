/*
 * TelemetryThread.cpp
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#include "TelemetryThread.h"
//#include "stdio.h"

TelemetryThread::TelemetryThread(uint64_t delay,
			RODOS::HAL_GPIO *led,
			RODOS::CommBuffer<uint64_t> *telemetryIntervalBuffer,
			RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> *telemetryMsgBuffer,
			RODOS::CommBuffer<INTERNAL_MSG::MEASUREMENT> *measurementBuffer,
			RODOS::CommBuffer<INTERNAL_MSG::CALIBRATION> *calibrationBuffer,
			const char* name)
	: ContinuousThread(delay, led, name),
		telemetryIntervalBuffer(telemetryIntervalBuffer),
		telemetryMsgBuffer(telemetryMsgBuffer),
		calibrationBuffer(calibrationBuffer),
		measurementBuffer(measurementBuffer),
		msgCount(0)
{}

TelemetryThread::~TelemetryThread() {
	// TODO Auto-generated destructor stub
}

void TelemetryThread::prepare()
{
	// Currently not used
}

bool TelemetryThread::onLoop(uint64_t time)
{
	TELEMETRY::TELEMETRY_MSG telMsg(0, 0, BOARD_STATE::RADIO_SILENCE);		// "Empty" msg

	if (!telemetryMsgBuffer->getOnlyIfNewData(telMsg) && telMsg.STATE == BOARD_STATE::RADIO_SILENCE)
	{
		// Nothing to print out
		return true;
	}

//	PRINTF("\n\nTelemetry - time %llu msg-id %c state %c\n\n", telMsg.INTERNAL_TIME, telMsg.MSG_ID, telMsg.STATE);

//	uint8_t length = snprintf(NULL, 0, "{}");	// Is promising but I am not sure whether this would work in rodos
//	length += length % 10;

	uint8_t length = 0;

	if (msgCount == MESSAGES::MAGIC_BYTE || msgCount == MESSAGES::MAGIC_BYTE_END)
	{
		msgCount = MESSAGES::MAGIC_BYTE_END + 1;
	}

	if (telMsg.MSG_ID == TELEMETRY::CALIBRATION_DATA::MSG_ID)
	{
		static INTERNAL_MSG::CALIBRATION cal;
		this->calibrationBuffer->getOnlyIfNewData(cal);
		PRINTF("%c%c%c%c"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%llu;%c"
				"%c",
				MESSAGES::MAGIC_BYTE, length, msgCount, telMsg.MSG_ID,
				cal.ACCEL_OFFSET.x, cal.ACCEL_OFFSET.y, cal.ACCEL_OFFSET.z,
				cal.GYRO_OFFSET.x, cal.GYRO_OFFSET.y, cal.GYRO_OFFSET.z,
				cal.MAGNET_OFFSET_MIN.x, cal.MAGNET_OFFSET_MIN.y, cal.MAGNET_OFFSET_MIN.z,
				cal.MAGNET_OFFSET_MAX.x, cal.MAGNET_OFFSET_MAX.y, cal.MAGNET_OFFSET_MAX.z,
				telMsg.INTERNAL_TIME, telMsg.STATE,
				MESSAGES::MAGIC_BYTE_END);
	}
	else if (telMsg.MSG_ID == TELEMETRY::SYSTEM_T::MSG_ID)
	{
		static INTERNAL_MSG::MEASUREMENT dat;
		this->measurementBuffer->getOnlyIfNewData(dat);
		PRINTF("%c%c%c%c"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;"
				"%llu;%c"
				"%c",
				MESSAGES::MAGIC_BYTE, length, msgCount, telMsg.MSG_ID,
				dat.accel.x, dat.accel.y, dat.accel.z,
				dat.gyro.x, dat.gyro.y, dat.gyro.z,
				dat.GYRO_SPEED.x, dat.GYRO_SPEED.y, dat.GYRO_SPEED.z,
				dat.GYRO_GAUSS.x, dat.GYRO_SPEED.y, dat.GYRO_SPEED.z,
				dat.magnet.x, dat.magnet.y, dat.magnet.z,
				dat.rotMatrix.r[0][0], dat.rotMatrix.r[0][1], dat.rotMatrix.r[0][2],
				dat.rotMatrix.r[1][0], dat.rotMatrix.r[1][1], dat.rotMatrix.r[1][2],
				dat.rotMatrix.r[2][0], dat.rotMatrix.r[2][1], dat.rotMatrix.r[2][2],
				dat.temp,
				telMsg.INTERNAL_TIME, telMsg.STATE,
				MESSAGES::MAGIC_BYTE_END);
	}
	else
	{
		PRINTF("%c%c%c%c"
				"%llu;%c"
				"%c",
				MESSAGES::MAGIC_BYTE, length, msgCount, telMsg.MSG_ID,
				telMsg.INTERNAL_TIME, telMsg.STATE,
				MESSAGES::MAGIC_BYTE_END);
	}

	msgCount++;

	telemetryIntervalBuffer->getOnlyIfNewData(DELAY);
	return true;
}

void TelemetryThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
