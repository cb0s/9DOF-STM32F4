/*
 * messages.cpp
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#include "messages.h"

uint8_t MESSAGES::MAGIC_BYTE = 0x42;
uint8_t MESSAGES::HEADER_LENGTH = 4;

uint8_t MESSAGES::USED_MSG_IDS[] = {
		0x4		// Telemetry
};

MESSAGES::MESSAGE MESSAGES::INVALID_MSG(MESSAGES::HEADER(255, 255, 255), nullptr);

MESSAGES::MESSAGE MESSAGES::parseRaw(uint8_t *rawMessage, size_t bufferLength)
{
	if (bufferLength < HEADER_LENGTH || rawMessage[0] != MESSAGES::MAGIC_BYTE)
	{
		// Unsuccessful because there is not even a complete header and at minimum a complete header is required
		return MESSAGES::INVALID_MSG;
	}

	return MESSAGES::MESSAGE(MESSAGES::HEADER(rawMessage[1], rawMessage[2], rawMessage[3]),
			rawMessage+HEADER_LENGTH);
}

inline uint8_t MESSAGES::messagesMissing(MESSAGES::MESSAGE *m1, MESSAGES::MESSAGE *m2)
{
	uint8_t val = m2->HEAD.ID-m1->HEAD.ID;
	return val == 255 ? 0 : val-1;
}
