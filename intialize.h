/*
 * intialize.h
 *
 *  Created on: 10.06.2021
 *      Author: Cedric Boes
 */

#ifndef INTIALIZE_H_
#define INTIALIZE_H_

#include "rodos.h"

#include "devices/stm32f4.h"

#include "messages/telemetry.h"

#include "sensors/constants/lsm9ds1_const.h"
#include "sensors/i2c/lsm9ds1_i2c.h"
#include "sensors/registers/registers.h"
#include "sensors/Lsm9ds1Hal.h"

#include "threads/SignalProcessorThread.h"
#include "threads/TelecommandThread.h"
#include "threads/TelemetryThread.h"

#include "topics.h"


static HAL_I2C i2cDev(I2C_IDX2);
static Lsm9ds1Hal sensor(&i2cDev);

// System T
static Vector3D acc(0, 0, 0);
static Vector3D gyro(0, 0, 0);
static Vector3D gyroSpeed(0, 0, 0);
static Vector3D gyroGauss(0, 0, 0);
static Vector3D magnet(0, 0, 0);
static Matrix3D rotMatrix({0, 0, 0}, {0, 0, 0}, {0, 0, 0});
static float temp = 0.0f;
static uint64_t systemTime = 0;

// Calibration data
static Vector3D accelOff(0, 0, 0);
static Vector3D gyroOff(0, 0, 0);
static Vector3D magnOffMin(0, 0, 0);
static Vector3D magnOffMax(0, 0, 0);
static uint64_t calTime = 0;

// Reading error data
static uint64_t readTime = 0;

// Alive data
static uint64_t aliveTime = 0;

// Message Objects
static TELEMETRY::SYSTEM_T signalData(acc, gyro, gyroSpeed, gyroGauss, magnet, rotMatrix, temp, systemTime);
static TELEMETRY::CALIBRATION_DATA calibrationData(accelOff, gyroOff, magnOffMin, magnOffMax, calTime);
static TELEMETRY::READING_ERROR readingErrorData(readTime);
static TELEMETRY::ALIVE_SIGNAL aliveSignalData(aliveTime);

// Comm-Buffers
static RODOS::CommBuffer<BOARD_STATE> stateBuffer;
static RODOS::CommBuffer<uint64_t> signalIntervalBuffer;
static RODOS::CommBuffer<uint64_t> telemetryIntervalBuffer;
static RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> telemetryMsgBuffer;
static RODOS::CommBuffer<TELECOMMAND::TELECOMMAND_MSG> telecommandBuffer;

// Subscribers
static RODOS::Subscriber stateSubscriber(TOPICS::SYSTEM_STATE_TOPIC, stateBuffer, "SYSTEM_STATE");
static RODOS::Subscriber signalIntervalSubscriber(TOPICS::SIGNAL_INTERVAL_TOPIC, signalIntervalBuffer, "SIGNAL_INTERVAL");
static RODOS::Subscriber telemetryIntervalSubscriber(TOPICS::TELEMETRY_INTERVAL_TOPIC, telemetryIntervalBuffer, "TELEMETRY_INTERVAL");
static RODOS::Subscriber telemetryMsgSubscriber(TOPICS::TELEMETRY_TOPIC, telemetryMsgBuffer, "TELEMETRY_MSG");
static RODOS::Subscriber telecommandSubscriber(TOPICS::TELECOMMAND_TOPIC, telecommandBuffer, "TELECOMMAND_MSG");

// LEDs
HAL_GPIO gpioBlue((GPIO_PIN) STM32F4::BOARD_LEDS::LED_BLUE);
HAL_GPIO gpioRed((GPIO_PIN) STM32F4::BOARD_LEDS::LED_RED);
HAL_GPIO gpioGreen((GPIO_PIN) STM32F4::BOARD_LEDS::LED_GREEN);
HAL_GPIO gpioOrange((GPIO_PIN) STM32F4::BOARD_LEDS::LED_ORANGE);

// Uart
HAL_UART uart(UART_IDX3, GPIO_022, GPIO_023);

// Threads
static SignalProcessorThread signalThread(&sensor,
		400000,
		&signalData,
		&calibrationData,
		&readingErrorData,
		&aliveSignalData,
		&stateBuffer,
		&signalIntervalBuffer,
		&gpioRed,
		10,
		&gpioGreen,
		"SignalProcessorThread");

static TelecommandThread telecommandThread(10,
		&gpioOrange,
		&uart,
		115200,
		END_OF_TIME,
		&stateBuffer,
		"TelecommandThread");

static TelemetryThread telemetryThread(10,
		&gpioBlue,
		&telemetryIntervalBuffer,
		&telemetryMsgBuffer,
		"TelemetryThread");

#endif /* INTIALIZE_H_ */
