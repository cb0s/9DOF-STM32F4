/*
 * utils.h
 *
 *  Created on: 28.05.2021
 *      Author: Cedric Boes
 */
#pragma once

#ifndef UTILS_H_
#define UTILS_H_

#include "rodos.h"
#include "matlib.h"

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

	/**
	 * Pauses the executing thread.<br>
	 * Note that this should only be used when there is no access to the executing thread directly.
	 */
	extern void blockingDelay(uint64_t nanoDelay);

	extern void minimizedVector(Vector3D &toMax, Vector3D &toCompare);

	extern void maximizedVector(Vector3D &toMax, Vector3D &toCompare);
};

#endif /* UTILS_H_ */
