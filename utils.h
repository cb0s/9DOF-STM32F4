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

namespace UTILS {
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

	/**
	 * Clears the given data-buffer.
	 */
	extern void clearBuffer(uint8_t *buffer, size_t length);
};

#endif /* UTILS_H_ */
