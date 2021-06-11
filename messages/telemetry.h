/*
 * telemetry.h
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#ifndef MESSAGES_TELEMETRY_H_
#define MESSAGES_TELEMETRY_H_

#include "rodos.h"
#include "matlib.h"
#include "messages.h"
#include"../states.h"

namespace TELEMETRY {
	struct TELEMETRY_MSG {
		TELEMETRY_MSG() : TELEMETRY_MSG(255, 255, BOARD_STATE::RADIO_SILENCE)
		{}

		TELEMETRY_MSG(uint8_t msgId, uint64_t internalTime, BOARD_STATE state)
			: MSG_ID(msgId), INTERNAL_TIME(internalTime), STATE(state)
		{}

		uint8_t MSG_ID;
		uint64_t INTERNAL_TIME;
		BOARD_STATE STATE;
	};

	struct SYSTEM_T {
		SYSTEM_T()
		{}

		static const uint8_t MSG_ID = 0x10;
	};

	struct CALIBRATION_DATA {
		CALIBRATION_DATA()
		{}

		static const uint8_t MSG_ID = 0x11;
	};

	struct READING_ERROR {
		READING_ERROR()
		{}

		static const uint8_t MSG_ID = 0x12;
	};

	struct ALIVE_SIGNAL {
		ALIVE_SIGNAL()
		{}

		static const uint8_t MSG_ID = 0x13;
	};

	extern bool operator == (TELEMETRY_MSG &msg1, TELEMETRY_MSG &msg2);
};



#endif /* MESSAGES_TELEMETRY_H_ */
