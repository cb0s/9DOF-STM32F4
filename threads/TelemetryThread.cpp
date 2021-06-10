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
			const char* name)
	: ContinuousThread(delay, led, name),
	  telemetryIntervalBuffer(telemetryIntervalBuffer),
	  telemetryMsgBuffer(telemetryMsgBuffer),
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

	if (!telemetryMsgBuffer->getOnlyIfNewData(telMsg))
	{	// Nothing to print out
		return true;
	}

//	uint8_t length = snprintf(NULL, 0, "{}");	// Is promising but I am not sure whether this would work in rodos
//	length += length % 10;

	uint8_t length = 0;

	if (telMsg.MSG_ID == TELEMETRY::ALIVE_SIGNAL::MSG_ID)
	{
		TELEMETRY::ALIVE_SIGNAL *msg = (TELEMETRY::ALIVE_SIGNAL*) &telMsg;
		PRINTF("%c%c%c"
				"%llu;%c;"
				"%c",
				MESSAGES::MAGIC_BYTE, msgCount++, telMsg.MSG_ID,
				msg->INTERNAL_TIME, msg->STATE,
				MESSAGES::MAGIC_BYTE_END);
	}
	else if (telMsg.MSG_ID == TELEMETRY::CALIBRATION_DATA::MSG_ID)
	{
		TELEMETRY::CALIBRATION_DATA msg = *((TELEMETRY::CALIBRATION_DATA*) &telMsg);
		PRINTF("%c%c%c"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%llu;%c;"
				"%c",
				MESSAGES::MAGIC_BYTE, msgCount++, telMsg.MSG_ID,
				msg.ACCEL_OFFSET.x, msg.ACCEL_OFFSET.y, msg.ACCEL_OFFSET.z,
				msg.GYRO_OFFSET.x, msg.GYRO_OFFSET.y, msg.GYRO_OFFSET.z,
				msg.MAGNET_OFFSET_MIN.x, msg.MAGNET_OFFSET_MIN.y, msg.MAGNET_OFFSET_MIN.z,
				msg.MAGNET_OFFSET_MAX.x, msg.MAGNET_OFFSET_MAX.y, msg.MAGNET_OFFSET_MAX.z,
				msg.INTERNAL_TIME, msg.STATE,
				MESSAGES::MAGIC_BYTE_END);
	}
	else if (telMsg.MSG_ID == TELEMETRY::READING_ERROR::MSG_ID)
	{
		TELEMETRY::READING_ERROR msg = *((TELEMETRY::READING_ERROR*) &telMsg);
		PRINTF("%c%c%c"
				"%llu;%c;"
				"%c",
				MESSAGES::MAGIC_BYTE, msgCount++, telMsg.MSG_ID,
				msg.INTERNAL_TIME, msg.STATE,
				MESSAGES::MAGIC_BYTE_END);
	}
	else if (telMsg.MSG_ID == TELEMETRY::SYSTEM_T::MSG_ID)
	{
		TELEMETRY::SYSTEM_T msg = *((TELEMETRY::SYSTEM_T*) &telMsg);
		PRINTF("%c%c%c"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;"
				"%f;%f;%f;%f;%f;%f;%f;%f;%f;"
				"%f;"
				"%llu;%c;"
				"%c",
				MESSAGES::MAGIC_BYTE, msgCount++, telMsg.MSG_ID,
				msg.ACCEL.x, msg.ACCEL.y, msg.ACCEL.z,
				msg.GYRO.x, msg.GYRO.y, msg.GYRO.z,
				msg.GYRO_SPEED.x, msg.GYRO_SPEED.y, msg.GYRO_SPEED.z,
				msg.GYRO_GAUSS.x, msg.GYRO_SPEED.y, msg.GYRO_SPEED.z,
				msg.MAGNET.x, msg.MAGNET.y, msg.MAGNET.z,
				msg.ROT_MATRIX.r[0][0], msg.ROT_MATRIX.r[0][1], msg.ROT_MATRIX.r[0][2],
				msg.ROT_MATRIX.r[1][0], msg.ROT_MATRIX.r[1][1], msg.ROT_MATRIX.r[1][2],
				msg.ROT_MATRIX.r[2][0], msg.ROT_MATRIX.r[2][1], msg.ROT_MATRIX.r[2][2],
				MESSAGES::MAGIC_BYTE_END);
	}

	telemetryIntervalBuffer->getOnlyIfNewData(DELAY);
	return true;
}

void TelemetryThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
