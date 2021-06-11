/*
 * ContinousThread.h
 *
 *  Created on: 05.06.2021
 *      Author: Cedric Boes
 */

#ifndef THREADS_CONTINUOUSTHREAD_H_
#define THREADS_CONTINUOUSTHREAD_H_

#include "rodos.h"

class ContinuousThread : public Thread
{
public:
	ContinuousThread(uint64_t delay, RODOS::HAL_GPIO *LED, const char* name = "Anonymous-ContinousThread");
	virtual ~ContinuousThread();

	virtual void prepare() = 0;
	virtual bool onLoop(uint64_t currentTime) = 0;
	virtual void cleanUp() = 0;

	void init() override;
	void run() override;

	void toggleLed();
protected:
	uint64_t DELAY;
	RODOS::HAL_GPIO *LED;
};

#endif /* THREADS_CONTINUOUSTHREAD_H_ */
