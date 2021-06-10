/*
 * telemetry.cpp
 *
 *  Created on: 10.06.2021
 *      Author: Cedric Boes
 */

#include "telemetry.h"

bool TELEMETRY::operator == (TELEMETRY::TELEMETRY_MSG &msg1, TELEMETRY::TELEMETRY_MSG &msg2)
{
	return msg1.INTERNAL_TIME == msg2.INTERNAL_TIME && msg2.MSG_ID == msg2.MSG_ID && msg1.STATE == msg2.STATE;
}


