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

namespace TOPICS {
	Topic<TELEMETRY::SYSTEM_T> SYSTEM_T_TOPIC;
	Topic<TELEMETRY::TELEMETRY_MSG> TELEMETRY_TOPIC;
	Topic<TELEMETRY::CALIBRATION_DATA> CALIBRATION_TOPIC;

	Topic<TELECOMMAND::CHANGE_STATE> CHANGE_STATE_TOPIC;
	Topic<TELECOMMAND::UPDATE_TELEMETRY_DELAY> UPDATE_TELEMETRY_DELAY_TOPIC;
	Topic<TELECOMMAND::UPDATE_SIGNAL_UPDATE_DELAY> UPDATE_SIGNAL_UPDATE_DELAY_TOPIC;
};


#endif /* TOPICS_H_ */
