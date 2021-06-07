/*
 * SignalProcessorThread.h
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#ifndef THREADS_SIGNALPROCESSORTHREAD_H_
#define THREADS_SIGNALPROCESSORTHREAD_H_

#include "rodos.h"
#include "ContinuousThread.h"
#include "../messages/telemetry.h"
#include "../sensors/Lsm9ds1Hal.h"

class SignalProcessorThread: public ContinuousThread {
public:
	SignalProcessorThread(Lsm9ds1Hal *sensor, uint64_t delay,
			RODOS::HAL_GPIO *LED, const char *name = "Anonymous-SignalProcessorThread");
	virtual ~SignalProcessorThread();

	bool onLoop(uint64_t time) override;
	void cleanUp() override;

private:
	Lsm9ds1Hal *SENSOR;
	TELEMETRY::CALIBRATION_DATA *data = nullptr;
};

#endif /* THREADS_SIGNALPROCESSORTHREAD_H_ */
