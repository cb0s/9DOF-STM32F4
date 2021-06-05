/*
 * ContinousThread.cpp
 *
 *  Created on: 05.06.2021
 *      Author: Cedric Boes
 */

#include "ContinousThread.h"


ContinuousThread::ContinuousThread(unsigned long delay, RODOS::HAL_GPIO LED, const char* name) : m_DELAY(delay), m_LED(LED), Thread(name)
{
	LED.init(true, 1, 0);
}

void ContinuousThread::run()
{
	unsigned long currentTime = RODOS::NOW();
	while(onLoop(currentTime))
	{
		currentTime += m_DELAY;
		AT(currentTime);
	}

	cleanUp();
}

void ContinuousThread::toggleLed()
{
	m_LED.setPins(~m_LED.readPins());
}
