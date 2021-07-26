/*
 * TelecommandThread.cpp
 *
 *  Created on: 06.06.2021
 *      Author: Cedric Boes
 */

#include "TelecommandThread.h"

TelecommandThread::TelecommandThread(uint64_t delay,
		RODOS::HAL_GPIO *led,
		RODOS::HAL_UART *uart,
		uint32_t baudRate,
		uint64_t timeOut,
		RODOS::CommBuffer<uint8_t> *stateBuffer,
		const char* name)
	: ContinuousThread(delay, led, name),
	  uart(uart),
	  baudRate(baudRate),
	  timeOut(timeOut),
	  stateBuffer(stateBuffer),
	  lastMsgBegin(0),
	  currentMsgSize(0)
{}

TelecommandThread::~TelecommandThread()
{}

void TelecommandThread::prepare()
{
	UTILS::clearBuffer(messageBuffer, BUFFER_LENGTH);

	// Might not be working according to friends
//	this->uart->init(baudRate);

//	this->uart->config(RODOS::UART_PARAMETER_ENABLE_DMA, 1);
//	this->uart->config(RODOS::UART_PARAMETER_BAUDRATE, baudRate);
}

bool TelecommandThread::onLoop(uint64_t time)
{
	PRINTF("WAITING FOR TCs\n");

	this->uart->suspendUntilDataReady();

	uint64_t wait = this->lastMsgBegin + this->timeOut;
	if (this->lastMsgBegin != 0 && (wait <= timeOut ? END_OF_TIME : wait) <= RODOS::NOW())
	{
		resetError();
		return true;
	}

	toggleLed();	// Blinks if handling TC
	PRINTF("\n\nTELECOMMAND\n\n");

	char tempBuff[1];
	if (!this->uart->read(tempBuff, 1))
	{
		resetError();
	}

	if (tempBuff[0] == MESSAGES::MAGIC_BYTE)
	{
		if (lastMsgBegin != 0)
		{
			resetError();
		}

		lastMsgBegin = RODOS::NOW();
	}

	// Must be non blocking because to occur there must be an incomplete msg before
	// -> if you warn the user constantly this will block the thread for 13*0.5s = 6.5s
	if (lastMsgBegin == 0)
	{
		reset();
		return true;
	}

	messageBuffer[currentMsgSize++] = tempBuff[0];

	if (tempBuff[0] == MESSAGES::MAGIC_BYTE_END)
	{
		PRINTF("\nHandling TC-End\n");

		if (currentMsgSize < 2)
		{
			resetError();
			return true;
		}

		if (handleMsg())
		{
			PRINTF("\nSUCCESS\n");
			reset();
			return true;
		}

		PRINTF("\nUnknown TC\n");
		resetError();
		return true;
	}

	if (currentMsgSize >= BUFFER_LENGTH)
	{
		resetError();
		return true;
	}

	return true;
}

void TelecommandThread::reset()
{
	lastMsgBegin = 0;
	currentMsgSize = 0;
	UTILS::clearBuffer(this->messageBuffer, BUFFER_LENGTH);

	LED->setPins(true);
}

void TelecommandThread::resetError()
{
	// Blocks TC reading for 500 ms!
	for(uint8_t i; i < 5; i++)
	{
		toggleLed();
		AT(100 * MILLISECONDS);
	}
	reset();
}

bool TelecommandThread::handleMsg()
{
	for (int i = 0; i < currentMsgSize; i++) {
		PRINTF("%c", messageBuffer[i]);
	}
	PRINTF("\n");

	MESSAGES::MESSAGE msg = MESSAGES::parseRaw(messageBuffer, currentMsgSize);

	PRINTF("\nTelecommand Handling\n");

	if (msg == MESSAGES::INVALID_MSG)
	{
		PRINTF("\nINVALID\n");
		return false;
	}

	switch(msg.HEAD.MSG_ID)
	{
	case TELECOMMAND::CHANGE_STATE::MSG_ID:
		PRINTF("\nChange State\n");

		if (msg.HEAD.LENGTH != 1)
		{
			PRINTF("LENGHT\n");
			return false;
		}

		// Block because of state which would be leaked to other cases
		{
			BOARD_STATE state = (BOARD_STATE) msg.CONTENT[0];
			uint8_t currentStateRaw = state;
			this->stateBuffer->get(currentStateRaw);
			BOARD_STATE currentState = static_cast<BOARD_STATE>(currentStateRaw);

			PRINTF("\nstateBuffer %d\n", currentState);

			if ((currentState != BOARD_STATE::NORMAL &&
					currentState != BOARD_STATE::RADIO_SILENCE &&
					currentState != BOARD_STATE::REQUEST_CAL_INFO) ||
					state == currentState ||
					(state != BOARD_STATE::NORMAL &&
					state != BOARD_STATE::RADIO_SILENCE &&
					state != BOARD_STATE::REQUEST_CAL_INFO &&
					state != BOARD_STATE::CALIBRATE_ACCEL &&
					state != BOARD_STATE::CALIBRATE_GYRO &&
					state != BOARD_STATE::CALIBRATE_MAGN))
			{
				PRINTF("INVALID_STATE\n");
				return false;
			}

			PRINTF("CHANGED_STATE\n");
			TOPICS::SYSTEM_STATE_TOPIC.publishConst(static_cast<uint8_t>(state));
		}
		return true;
	case TELECOMMAND::UPDATE_SIGNAL_DELAY::MSG_ID:
		PRINTF("\nUpdate Signal Delay\n");

		if (msg.HEAD.LENGTH != 8)
		{
			return false;
		}

		{
			uint64_t val = UTILS::bufferToUInt64T(msg.CONTENT);
			TOPICS::SIGNAL_INTERVAL_TOPIC.publishConst(val);
		}
		return true;
	case TELECOMMAND::UPDATE_TELEMETRY_DELAY::MSG_ID:
		PRINTF("\nUpdate Telemetry Delay\n");

		if (msg.HEAD.LENGTH != 8)
		{
			return false;
		}

		{
			uint64_t val = UTILS::bufferToUInt64T(msg.CONTENT);
			TOPICS::TELEMETRY_INTERVAL_TOPIC.publishConst(val);
		}

		return true;
	default:
		PRINTF("\nDefault Branch\n");

		return false;
	}
	PRINTF("\nNothing Happened\n");
	resetError();
}

void TelecommandThread::cleanUp()
{
	// That's bad! Do something (maybe LED-blinking?)...
}
