/*
 * topics.cpp
 *
 *  Created on: 09.06.2021
 *      Author: Cedric Boes
 */

#include "topics.h"

Topic<BOARD_STATE> TOPICS::SYSTEM_STATE_TOPIC(20, "SYSTEM_STATE_T");

Topic<uint64_t> TOPICS::SIGNAL_INTERVAL_TOPIC(30, "SIGNAL_INTERVAL_T");
Topic<uint64_t> TOPICS::TELEMETRY_INTERVAL_TOPIC(31, "TELEMETRY_INTERVAL_T");

Topic<TELEMETRY::TELEMETRY_MSG> TOPICS::TELEMETRY_TOPIC(40, "TELEMETRY_MSG_T");
Topic<TELECOMMAND::TELECOMMAND_MSG> TOPICS::TELECOMMAND_TOPIC(41, "TELECOMMAND_MSG_T");
