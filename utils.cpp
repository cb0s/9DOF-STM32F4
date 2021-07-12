/*
 * utils.cpp
 *
 *  Created on: 28.05.2021
 *      Author: Cedric Boes
 */
#include "utils.h"


void UTILS::clearBuffer(uint8_t *buffer, size_t length)
{
	for (size_t i = 0 ; i < length; i++)
	{
		buffer[i] = 0x0;
	}
}

void UTILS::blockingDelay(uint64_t nanoDelay)
{
	uint64_t start = RODOS::NOW();
	while (RODOS::NOW() < start + (int64_t) nanoDelay) asm("nop");;
}

uint64_t UTILS::bufferToUInt64T(uint8_t *buffer)
{
	return static_cast<uint64_t>(buffer[7]) << 56 |
			static_cast<uint64_t>(buffer[6]) << 48 |
			static_cast<uint64_t>(buffer[5]) << 40 |
			static_cast<uint64_t>(buffer[4]) << 32 |
			static_cast<uint32_t>(buffer[3]) << 24 |
			static_cast<uint32_t>(buffer[2]) << 16 |
			static_cast<uint16_t>(buffer[1]) << 8 |
			buffer[0];
}

void UTILS::minimizedVector(Vector3D &toMin, Vector3D &toCompare)
{
	toMin.x = toMin.x > toCompare.x ? toCompare.x : toMin.x;
	toMin.y = toMin.y > toCompare.y ? toCompare.y : toMin.y;
	toMin.z = toMin.z > toCompare.z ? toCompare.z : toMin.z;
}

void UTILS::maximizedVector(Vector3D &toMax, Vector3D &toCompare)
{
	toMax.x = toMax.x < toCompare.x ? toCompare.x : toMax.x;
	toMax.y = toMax.y < toCompare.y ? toCompare.y : toMax.y;
	toMax.z = toMax.z < toCompare.z ? toCompare.z : toMax.z;
}
