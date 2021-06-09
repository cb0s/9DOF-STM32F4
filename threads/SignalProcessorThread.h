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
#include "../topics.h"
#include "../messages/telemetry.h"
#include "../sensors/Lsm9ds1Hal.h"

#define CALIRBATION_SAMPLES 100

class SignalProcessorThread: public ContinuousThread {
public:
	SignalProcessorThread(
			Lsm9ds1Hal *sensor,
			TELEMETRY::SYSTEM_T *signalData,
			TELEMETRY::CALIBRATION_DATA *calibrationData,
			TELEMETRY::READING_ERROR *readingErrorData,
			TELEMETRY::ALIVE_SIGNAL *heartBeat,
			RODOS::CommBuffer<BOARD_STATE> *stateBuffer,
			RODOS::CommBuffer<uint64_t> *signalIntervalBuffer,

			uint64_t DELAY,
			RODOS::HAL_GPIO *LED,
			const char *name = "Anonymous-SignalProcessorThread");
	virtual ~SignalProcessorThread();

	bool onLoop(uint64_t time) override;
	void cleanUp() override;

private:
	bool measure(uint64_t time);

	bool radioSilenceSent;
	BOARD_STATE state;

	uint64_t lastRead;

	Lsm9ds1Hal *SENSOR;
	TELEMETRY::SYSTEM_T *signalData;
	TELEMETRY::CALIBRATION_DATA *calibrationData;
	TELEMETRY::READING_ERROR *readingErrorData;
	TELEMETRY::ALIVE_SIGNAL *heartBeat;
	RODOS::CommBuffer<BOARD_STATE> *stateBuffer;
	RODOS::CommBuffer<uint64_t> *signalIntervalBuffer;

	RODOS::HAL_GPIO calibrationLed;
};

#endif /* THREADS_SIGNALPROCESSORTHREAD_H_ */
