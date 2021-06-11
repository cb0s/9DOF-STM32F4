/*
 * telecommands.h
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#ifndef MESSAGES_TELECOMMANDS_H_
#define MESSAGES_TELECOMMANDS_H_

#include "rodos.h"
#include "../states.h"

namespace TELECOMMAND {
	// Object Oriented programming in rodos goes BRRRRRRR :D
	struct TELECOMMAND_MSG {
		TELECOMMAND_MSG() : TELECOMMAND_MSG(255)
		{}

		TELECOMMAND_MSG(uint8_t msgId) : MSG_ID(msgId)
		{}

		uint8_t MSG_ID;
	};

	struct CHANGE_STATE : TELECOMMAND_MSG {
		CHANGE_STATE(const BOARD_STATE newState) : TELECOMMAND_MSG(MSG_ID), STATE(newState)
		{}

		const BOARD_STATE STATE;
		static const uint8_t MSG_ID = 0x1;
	};

	struct UPDATE_TELEMETRY_DELAY : TELECOMMAND_MSG {
		UPDATE_TELEMETRY_DELAY(const uint64_t delay) : TELECOMMAND_MSG(MSG_ID), DELAY(delay)
		{}

		const uint64_t DELAY;
		static const uint8_t MSG_ID = 0x2;
	};

	struct UPDATE_SIGNAL_DELAY : TELECOMMAND_MSG {
		UPDATE_SIGNAL_DELAY(const uint64_t delay) : TELECOMMAND_MSG(MSG_ID), DELAY(delay)
		{}

		const uint64_t DELAY;
		static const uint8_t MSG_ID = 0x3;
	};
};


#endif /* MESSAGES_TELECOMMANDS_H_ */
