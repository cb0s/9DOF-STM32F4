/*
 * sensors/registers/lsm9ds1.h
 *
 * Defines all the registers of the LSM9DS1-sensor so that they are in a more convenient format.
 * To ensure that no reserved
 *
 *  Created on: 03.06.2021
 *      Author: Cedric Boes
 */
#ifndef SENSORS_REGISTERS_LSM9DS1_REG_H_
#define SENSORS_REGISTERS_LSM9DS1_REG_H_

#include "registers.h"

/**
 * Defines all registers for the accelerometer, gyroscope and thermometer.
 */
// If not performant enough replace with macro
namespace LSM9DS1_GXL_REGS {
	// Accelerometer, Thermometer, Gyroscope
	extern Register ACT_THS;
	extern Register ACT_DUR;

	extern Register INT_GEN_CFG_XL;
	extern Register INT_GEN_THS_X_XL;
	extern Register INT_GEN_THS_Y_XL;
	extern Register INT_GEN_THS_Z_XL;
	extern Register INT_GEN_DUR_XL;

	extern Register REFERENCE_G;

	extern Register INT1_CTRL;
	extern Register INT_CTRL;

	extern Register WHO_AM_I_GXL;

	extern Register CTRL_REG1_G;
	extern Register CTRL_REG2_G;
	extern Register CTRL_REG3_G;

	extern Register ORIENT_CFG_G;
	extern Register INT_GEN_SRC_G;

	extern Register OUT_TEMP_L;
	extern Register OUT_TEMP_H;

	extern Register STATUS_REG_G;

	extern Register OUT_X_G_L;
	extern Register OUT_X_G_H;
	extern Register OUT_Y_G_L;
	extern Register OUT_Y_G_H;
	extern Register OUT_Z_G_L;
	extern Register OUT_Z_G_H;

	extern Register CTRL_REG4;
	extern Register CTRL_REG5_XL;
	extern Register CTRL_REG6_XL;
	extern Register CTRL_REG7_XL;
	extern Register CTRL_REG8;
	extern Register CTRL_REG9;
	extern Register CTRL_REG10;

	extern Register INT_GEN_SRC_XL;

	extern Register STATUS_REG_XL;

	extern Register OUT_X_XL_L;
	extern Register OUT_X_XL_H;
	extern Register OUT_Y_XL_L;
	extern Register OUT_Y_XL_H;
	extern Register OUT_Z_XL_L;
	extern Register OUT_Z_XL_H;

	extern Register FIFO_CTRL;
	extern Register FIFO_SRC;

	extern Register INT_GEN_CFG_G;
	extern Register INT_GEN_THS_X_G_H;
	extern Register INT_GEN_THS_X_G_L;
	extern Register INT_GEN_THS_Y_G_H;
	extern Register INT_GEN_THS_Y_G_L;
	extern Register INT_GEN_THS_Z_G_H;
	extern Register INT_GEN_THS_Z_G_L;
	extern Register INT_GEN_DUR_G;

	extern Register *USED_GXL[];

	extern size_t SIZE_GXL;
}

/**
 * Defines all registers for the magnetometer.
 */
namespace LSM9DS1_M_REGS {
	// Magnetic sensor
	extern Register OFFSET_X_REG_M_L;
	extern Register OFFSET_X_REG_M_H;
	extern Register OFFSET_Y_REG_M_L;
	extern Register OFFSET_Y_REG_M_H;
	extern Register OFFSET_Z_REG_M_L;
	extern Register OFFSET_Z_REG_M_H;

	extern Register WHO_AM_I_M;

	extern Register CTRL_REG1_M;
	extern Register CTRL_REG2_M;
	extern Register CTRL_REG3_M;
	extern Register CTRL_REG4_M;
	extern Register CTRL_REG5_M;

	extern Register STATUS_REG_M;

	extern Register OUT_X_M_L;
	extern Register OUT_X_M_H;
	extern Register OUT_Y_M_L;
	extern Register OUT_Y_M_H;
	extern Register OUT_Z_M_L;
	extern Register OUT_Z_M_H;

	extern Register INT_CFG_M;
	extern Register INT_SRC_M;
	extern Register INT_THS_M_L;
	extern Register INT_THS_M_H;

	extern Register *USED_M[];

	extern size_t SIZE_M;
}

#endif /* SENSORS_REGISTERS_LSM9DS1_REG_H_ */
