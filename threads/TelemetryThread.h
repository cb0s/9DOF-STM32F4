/*
 * TelemetryThread.h
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#ifndef THREADS_TELEMETRYTHREAD_H_
#define THREADS_TELEMETRYTHREAD_H_

#include "ContinuousThread.h"

class TelemetryThread: public ContinuousThread {
public:
	TelemetryThread(uint64_t delay, RODOS::HAL_GPIO *LED, const char* name = "Anonymous-TelemetryThread");
	virtual ~TelemetryThread();

	bool onLoop(uint64_t time) override;
	void cleanUp() override;
};

#endif /* THREADS_TELEMETRYTHREAD_H_ */
