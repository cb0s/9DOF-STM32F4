/*
 * messages.cpp
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#include "messages.h"

uint8_t MESSAGES::MAGIC_BYTE = 0x42;
uint8_t MESSAGES::MAGIC_BYTE_END = 0x43;
uint8_t MESSAGES::HEADER_LENGTH = 5;	// Magic byte (0x42 == 66) + 3 header bytes + Magic end byte (0x43 == 67)

uint8_t MESSAGES::USED_MSG_IDS[] = {
		0x1,		// CHANGE_STATE (TC)
		0x2,		// UPDATE_TELEMETRY_DELAY (TC)
		0x3,		// UPDATE_SIGNAL_UPDATE_DELAY (TC)

		0x10,		// SYSTEM_T (TEL)
		0x11,		// CALIBRATION_DATA (TEL)
		0x12,		// READING_ERROR (TEL)
		0x13,		// ALIVE_SIGNAL (TEL)
};

uint8_t MESSAGES::USED_MSG_COUNT = 7;

MESSAGES::MESSAGE MESSAGES::INVALID_MSG(MESSAGES::HEADER(255, 255, 255), nullptr);

MESSAGES::MESSAGE MESSAGES::parseRaw(uint8_t *rawMessage, size_t bufferLength)
{
	if (bufferLength < HEADER_LENGTH || rawMessage[0] != MESSAGES::MAGIC_BYTE || rawMessage[bufferLength] != MESSAGES::MAGIC_BYTE_END)
	{
		// Unsuccessful because there is not even a complete header and at minimum a complete header is required
		return MESSAGES::INVALID_MSG;
	}

	return MESSAGES::MESSAGE(MESSAGES::HEADER(rawMessage[1], rawMessage[2], rawMessage[3]),
			rawMessage+HEADER_LENGTH-1);
}

bool MESSAGES::operator ==(MESSAGES::HEADER &h1, MESSAGES::HEADER &h2)
{
	return h1.ID == h2.ID && h1.LENGTH == h2.LENGTH && h1.MSG_ID == h2.MSG_ID;
}

bool MESSAGES::operator ==(MESSAGES::MESSAGE &m1, MESSAGES::MESSAGE &m2)
{
	bool state = m1.HEAD == m2.HEAD;
	for (uint8_t i = 0; i < m1.HEAD.LENGTH && state; i++)
	{
		state = m1.CONTENT[i] == m2.CONTENT[i];
	}

	return state;
}

inline uint8_t MESSAGES::messagesMissing(MESSAGES::MESSAGE *m1, MESSAGES::MESSAGE *m2)
{
	uint8_t val = m2->HEAD.ID-m1->HEAD.ID;
	return val == 255 ? 0 : val-1;
}
