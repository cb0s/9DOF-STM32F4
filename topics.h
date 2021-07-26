/*
 * topics.h
 *
 *  Created on: 07.06.2021
 *      Author: Cedric Boes
 */

#ifndef TOPICS_H_
#define TOPICS_H_

#include "rodos.h"
#include "messages/telemetry.h"
#include "messages/telecommands.h"
#include "messages/internal.h"
#include "states.h"

namespace TOPICS {
	// application state
	extern Topic<uint8_t> SYSTEM_STATE_TOPIC;
	extern Topic<uint64_t> SIGNAL_INTERVAL_TOPIC;
	extern Topic<uint64_t> TELEMETRY_INTERVAL_TOPIC;

	extern Topic<INTERNAL_MSG::CALIBRATION> CURRENT_CAL_TOPIC;
	extern Topic<INTERNAL_MSG::MEASUREMENT> CURRENT_DAT_TOPIC;

	extern Topic<TELEMETRY::TELEMETRY_MSG> TELEMETRY_TOPIC;
	extern Topic<TELECOMMAND::TELECOMMAND_MSG> TELECOMMAND_TOPIC;
};


#endif /* TOPICS_H_ */
