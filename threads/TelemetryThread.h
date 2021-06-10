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

class TelemetryThread: public ContinuousThread {
public:
	TelemetryThread(uint64_t delay,
			RODOS::HAL_GPIO *led,
			RODOS::CommBuffer<uint64_t> *telemetryIntervalBuffer,
			RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> *telemetryMsgBuffer,
			const char* name = "Anonymous-TelemetryThread");
	virtual ~TelemetryThread();

	void prepare() override;
	bool onLoop(uint64_t time) override;
	void cleanUp() override;

private:
	RODOS::CommBuffer<uint64_t> *telemetryIntervalBuffer;
	RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> *telemetryMsgBuffer;

	uint8_t msgCount;
};

#endif /* THREADS_TELEMETRYTHREAD_H_ */
