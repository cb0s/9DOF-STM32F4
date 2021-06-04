/*
 * registers/registers.h
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */
#ifndef SENSORS_REGISTERS_REGISTERS_H_
#define SENSORS_REGISTERS_REGISTERS_H_

#include "rodos.h"

/**
 * Value which indicates that a sensor register does not have a default value.<br>
 * Usually this means that it is an output-register.
 */
#define NO_DEFAULT_VALUE -1

/**
 * Manages the access to a register. These flags can be added together.
 */
enum REGISTER_ACCESS {
	NO_ACCESS	= 0b00,		// Register cannot be accessed
	READ_ONLY	= 0b01,		// Register is in READ_ONLY access mode or values should not be overridden
	WRITE_ONLY	= 0b10,		// Register cannot be read and only be written to
	READ_WRITE	= 0b11		// Register can either be read or written to
};

/**
 * Grouping of the values of a register (e.g. for a sensor).<br>
 * It contains the address, the register access (how it can be accessed and if there are any limitations to that)
 * and the default value if present (if not NO_DEFAULT_VALUE).
 */
struct Register
{
	Register(uint8_t address, REGISTER_ACCESS access, int16_t defaultVal) : ADDRESS(address), ACCESS(access), DEFAULT(defaultVal)
	{}

	const uint8_t ADDRESS;
	const REGISTER_ACCESS ACCESS;
	const int16_t DEFAULT;
};

#endif /* SENSORS_REGISTERS_REGISTERS_H_ */
