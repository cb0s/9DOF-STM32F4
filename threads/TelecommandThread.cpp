/*
 * TelecommandThread.cpp
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#include "TelecommandThread.h"

TelecommandThread::TelecommandThread(uint64_t delay, RODOS::HAL_GPIO *LED, const char* name)
	: ContinuousThread(delay, LED, name)
{}

TelecommandThread::~TelecommandThread() {
	// TODO Auto-generated destructor stub
}


bool TelecommandThread::onLoop(uint64_t time)
{
	return false;
}

void TelecommandThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
