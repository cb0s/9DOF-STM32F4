/*
 * messages.h
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#ifndef MESSAGES_MESSAGES_H_
#define MESSAGES_MESSAGES_H_

#include <rodos.h>
#include "telecommands.h"

namespace MESSAGES {
	struct HEADER {
		HEADER(uint8_t length, uint8_t id, uint8_t msgId)
			: LENGTH(length), ID(id), MSG_ID(msgId)
		{}

		uint8_t LENGTH;
		uint8_t ID;
		uint8_t MSG_ID;
	};

	struct MESSAGE {
		MESSAGE(MESSAGES::HEADER HEAD, uint8_t *CONTENT) : HEAD(HEAD), CONTENT(CONTENT)
		{}

		MESSAGES::HEADER HEAD;
		uint8_t *CONTENT;
	};

	extern uint8_t HEADER_LENGTH;
	extern uint8_t MAGIC_BYTE;
	extern uint8_t USED_MSG_IDS[];
	extern MESSAGE INVALID_MSG;

	extern uint8_t messagesMissing(MESSAGE *m1, MESSAGE *m2);
	extern MESSAGE parseRaw(uint8_t *rawMessage, size_t bufferLength);
};


#endif /* MESSAGES_MESSAGES_H_ */
