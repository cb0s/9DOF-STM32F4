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

inline void UTILS::blockingDelay(uint64_t nanoDelay)
{
	uint64_t start = RODOS::NOW();
	while (RODOS::NOW() < start + nanoDelay);
}

inline void UTILS::minimizedVector(Vector3D &toMin, Vector3D &toCompare)
{
	toMin.x = toMin.x > toCompare.x ? toCompare.x : toMin.x;
	toMin.y = toMin.y > toCompare.y ? toCompare.y : toMin.y;
	toMin.z = toMin.z > toCompare.z ? toCompare.z : toMin.z;
}

inline void UTILS::maximizedVector(Vector3D &toMax, Vector3D &toCompare)
{
	toMax.x = toMax.x < toCompare.x ? toCompare.x : toMax.x;
	toMax.y = toMax.y < toCompare.y ? toCompare.y : toMax.y;
	toMax.z = toMax.z < toCompare.z ? toCompare.z : toMax.z;
}
