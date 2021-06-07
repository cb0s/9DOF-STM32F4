/*
 * TelecommandThread.h
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#ifndef THREADS_TELECOMMANDTHREAD_H_
#define THREADS_TELECOMMANDTHREAD_H_

#include "ContinuousThread.h"

class TelecommandThread: public ContinuousThread {
public:
	TelecommandThread(uint64_t delay, RODOS::HAL_GPIO *LED, const char* name = "Anonymous-TelecommandThread");
	virtual ~TelecommandThread();

	bool onLoop(uint64_t time) override;
	void cleanUp() override;
};

#endif /* THREADS_TELECOMMANDTHREAD_H_ */
