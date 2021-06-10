/*
 * TelecommandThread.h
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#ifndef THREADS_TELECOMMANDTHREAD_H_
#define THREADS_TELECOMMANDTHREAD_H_

#include "rodos.h"
#include "ContinuousThread.h"

#include "../messages/messages.h"
#include "../messages/telecommands.h"
#include "../topics.h"
#include "../utils.h"
#include "../states.h"

#define BUFFER_LENGTH 13	// (HEADER_LENGTH = 5) + 8 (MAX_TC_SIZE)

class TelecommandThread: public ContinuousThread {
public:
	TelecommandThread(uint64_t delay,
			RODOS::HAL_GPIO *LED,
			RODOS::HAL_UART *UART,
			uint32_t baudRate,
			uint64_t timeOut,
			RODOS::CommBuffer<BOARD_STATE> *stateBuffer,
			const char* name = "Anonymous-TelecommandThread");
	virtual ~TelecommandThread();

	void prepare() override;
	bool onLoop(uint64_t time) override;
	void cleanUp() override;

private:
	RODOS::HAL_UART *uart;
	const uint32_t baudRate;
	RODOS::CommBuffer<BOARD_STATE> *stateBuffer;
	uint64_t lastMsgBegin;
	const uint64_t timeOut;
	uint8_t messageBuffer[BUFFER_LENGTH];
	uint8_t currentMsgSize;

	void reset();
	void resetError();
	bool handleMsg();
};

#endif /* THREADS_TELECOMMANDTHREAD_H_ */
