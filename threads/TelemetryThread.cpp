/*
 * TelemetryThread.cpp
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#include "TelemetryThread.h"

TelemetryThread::TelemetryThread(uint64_t delay, RODOS::HAL_GPIO *LED, const char* name)
	: ContinuousThread(delay, LED, name)
{}

TelemetryThread::~TelemetryThread() {
	// TODO Auto-generated destructor stub
}

bool TelemetryThread::onLoop(uint64_t time)
{
	return false;
}

void TelemetryThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
