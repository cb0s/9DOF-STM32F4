/*
 * imuReader.cpp
 *
 *  Created on: 28.05.2021
 *      Author: Cedric
 */
#include "rodos.h"
#include "hal.h"
#include "comHandler.h"
#include "utils.h"


class ImuReader : RODOS::Thread
{
public:
	ImuReader(RODOS::HAL_GPIO LED) : Thread("ImuReader"), LED(LED)
	{
		LED.init(true, 1, 1);
	}

	void run() override
	{
		LED.setPins(~LED.readPins());
	}
private:
	RODOS::HAL_GPIO LED;
};
