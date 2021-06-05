/*
 * utils.cpp
 *
 *  Created on: 28.05.2021
 *      Author: Cedric Boes
 */
#include "utils.h"


inline void UTILS::clearBuffer(uint8_t *buffer, size_t length)
{
	for (size_t i = 0 ; i < length; i++)
	{
		buffer[i] = 0x0;
	}
}
