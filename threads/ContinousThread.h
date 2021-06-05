/*
 * ContinousThread.h
 *
 *  Created on: 05.06.2021
 *      Author: Cedric Boes
 */

#ifndef THREADS_CONTINOUSTHREAD_H_
#define THREADS_CONTINOUSTHREAD_H_

#include "rodos.h"

class ContinuousThread : public Thread
{
public:
	ContinuousThread(unsigned long delay, RODOS::HAL_GPIO LED, const char* name = "Anonymous-ContinousThread");

	virtual bool onLoop(unsigned long currentTime);
	virtual void cleanUp();

	void run() override;

private:
	const unsigned long m_DELAY;
	RODOS::HAL_GPIO m_LED;

	void toggleLed();
};

#endif /* THREADS_CONTINOUSTHREAD_H_ */
