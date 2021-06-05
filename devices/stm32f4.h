/*
 * stm32f4.h
 *
 *  Created on: 05.06.2021
 *      Author: Cedric
 */

#ifndef DEVICES_STM32F4_H_
#define DEVICES_STM32F4_H_

#include "rodos.h"

/**
 * Contains constants for the STM32F4.
 */
namespace STM32F4 {
	/**
	 * GPIOs of the LEDs of the board.
	 */
	typedef enum {
			LED_GREEN	= GPIO_PIN::GPIO_060,
			LED_ORANGE	= GPIO_PIN::GPIO_061,
			LED_RED		= GPIO_PIN::GPIO_062,
			LED_BLUE	= GPIO_PIN::GPIO_063
	} BORD_LEDS;
};

#endif /* DEVICES_STM32F4_H_ */
