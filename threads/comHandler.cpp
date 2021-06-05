/*
 * comHandler.cpp
 *
 *  Created on: 28.05.2021
 *      Author: Cedric
 */

#include "rodos.h"
#include "hal.h"
#include "comHandler.h"
#include "../utils.h"
#include "../sensors/registers/lsm9ds1_reg.h"


class TelecommandListener //: ContinuousThread
{
public:
	TelecommandListener() //: ContinuousThread(10*MILLISECONDS, RODOS::HAL_GPIO(LEDS::LED_ORANGE), "ConListener")
	{

	}

	bool onLoop(unsigned long time)
	{
		return true;
	}
};


class TelemetryPublisher //: ContinuousThread
{
public:
	TelemetryPublisher() //: ContinuousThread(10*MILLISECONDS, RODOS::HAL_GPIO(LEDS::LED_GREEN), "ConPublisher")
	{

	}

	bool onLoop(unsigned long time)
	{
		return true;
	}
};
