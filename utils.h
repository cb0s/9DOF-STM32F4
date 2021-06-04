/*
 * utils.h
 *
 *  Created on: 28.05.2021
 *      Author: Cedric
 */
#pragma once

#ifndef UTILS_H_
#define UTILS_H_

#include "rodos.h"
#include "hal.h"

namespace UTILS {
	/**
	 * GPIOs of the LEDs of the board (TODO: Exclude to designated namespace).
	 */
	typedef enum {
			LED_GREEN	= GPIO_060,
			LED_ORANGE	= GPIO_061,
			LED_RED		= GPIO_062,
			LED_BLUE	= GPIO_063
	} LEDS;

	/**
	 * Prints out the length of an array with a stopper-value of NULL.<br>
	 * If no NULL-value is present, 0 is returned.
	 */
	template<typename A> size_t arrayLength(A *array)
	{
		for (size_t i = 0; i < SIZE_MAX; i++) {
			if (array[i] == NULL) {
				return i;
			}
		}
		return 0;	// invalid array
	}
};

class ContinuousThread : Thread
{
public:
	ContinuousThread(unsigned long delay, RODOS::HAL_GPIO LED, const char* name = "Anonymous-ContinousThread");

	virtual bool onLoop(unsigned long currentTime);
	virtual void cleanUp();

	void run() override;

private:
	const unsigned long m_DELAY;
	RODOS::HAL_GPIO m_LED;

	void toggleLed();
};

#endif /* UTILS_H_ */
