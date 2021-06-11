/*
 * ContinousThread.cpp
 *
 *  Created on: 05.06.2021
 *      Author: Cedric Boes
 */

#include "ContinuousThread.h"


ContinuousThread::ContinuousThread(uint64_t delay, RODOS::HAL_GPIO *LED, const char* name)
	: DELAY(delay), LED(LED), Thread(name)
{}

ContinuousThread::~ContinuousThread()
{}

void ContinuousThread::init()
{
	LED->init(true, 1, 1);
	prepare();
}

void ContinuousThread::run()
{
	uint64_t currentTime = RODOS::NOW();
	while(onLoop(currentTime))
	{
		currentTime += DELAY;
		AT(currentTime);
	}

	cleanUp();
}

void ContinuousThread::toggleLed()
{
	LED->setPins(~LED->readPins());
}
