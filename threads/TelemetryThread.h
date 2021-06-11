/*
 * TelemetryThread.h
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#ifndef THREADS_TELEMETRYTHREAD_H_
#define THREADS_TELEMETRYTHREAD_H_

#include "rodos.h"
#include "ContinuousThread.h"
#include "../messages/telemetry.h"
#include "../messages/messages.h"
#include "../messages/internal.h"

class TelemetryThread: public ContinuousThread {
public:
	TelemetryThread(uint64_t delay,
			RODOS::HAL_GPIO *led,
			RODOS::CommBuffer<uint64_t> *telemetryIntervalBuffer,
			RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> *telemetryMsgBuffer,
			RODOS::CommBuffer<INTERNAL_MSG::MEASUREMENT> *measurementBuffer,
			RODOS::CommBuffer<INTERNAL_MSG::CALIBRATION> *calibrationBuffer,
			const char* name = "Anonymous-TelemetryThread");
	virtual ~TelemetryThread();

	void prepare() override;
	bool onLoop(uint64_t time) override;
	void cleanUp() override;

private:
	RODOS::CommBuffer<uint64_t> *telemetryIntervalBuffer;
	RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> *telemetryMsgBuffer;
	RODOS::CommBuffer<INTERNAL_MSG::CALIBRATION> *calibrationBuffer;
	RODOS::CommBuffer<INTERNAL_MSG::MEASUREMENT> *measurementBuffer;

	uint8_t msgCount;
};

#endif /* THREADS_TELEMETRYTHREAD_H_ */
