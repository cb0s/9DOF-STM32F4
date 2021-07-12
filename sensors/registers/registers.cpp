/*
 * register.cpp
 *
 *  Created on: 08.07.2021
 *      Author: Cedric Boes
 */

#include "registers.h"

bool REG_HELPERS::checkRegister(const Register *reg, const REGISTER_ACCESS requestedAccess, const Register **allowedRegs,
		const size_t allowedRegsSize)
{
	for (int i = 0; i < allowedRegsSize; i++)
	{
		if (reg->ADDRESS == allowedRegs[i]->ADDRESS)
		{
			return (requestedAccess & REGISTER_ACCESS::READ_ONLY) <= (reg->ACCESS & REGISTER_ACCESS::READ_ONLY) &&
					(requestedAccess & REGISTER_ACCESS::WRITE_ONLY) <= (reg->ACCESS & REGISTER_ACCESS::WRITE_ONLY);
		}
	}

	return false;
}
