/*
 * initialize.cpp
 *
 *  Created on: 11.06.2021
 *      Author: Cedric Boes
 */


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


HAL_I2C i2cDev(I2C_IDX2);
Lsm9ds1Hal sensor(&i2cDev);

// System T
Vector3D acc(0, 0, 0);
Vector3D gyro(0, 0, 0);
Vector3D gyroSpeed(0, 0, 0);
Vector3D gyroGauss(0, 0, 0);
Vector3D magnet(0, 0, 0);
Matrix3D rotMatrix({0, 0, 0}, {0, 0, 0}, {0, 0, 0});
float temp = 0.0f;
uint64_t systemTime = 0;

// Calibration data
Vector3D accelOff(0, 0, 0);
Vector3D gyroOff(0, 0, 0);
Vector3D magnOffMin(0, 0, 0);
Vector3D magnOffMax(0, 0, 0);
uint64_t calTime = 0;

// Reading error data
uint64_t readTime = 0;

// Alive data
uint64_t aliveTime = 0;

// Message Objects
TELEMETRY::SYSTEM_T signalData(acc, gyro, gyroSpeed, gyroGauss, magnet, rotMatrix, temp, systemTime);
TELEMETRY::CALIBRATION_DATA calibrationData(accelOff, gyroOff, magnOffMin, magnOffMax, calTime);
TELEMETRY::READING_ERROR readingErrorData(readTime);
TELEMETRY::ALIVE_SIGNAL aliveSignalData(aliveTime);

// Comm-Buffers
RODOS::CommBuffer<BOARD_STATE> stateBuffer;
RODOS::CommBuffer<uint64_t> signalIntervalBuffer;
RODOS::CommBuffer<uint64_t> telemetryIntervalBuffer;
RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> telemetryMsgBuffer;
RODOS::CommBuffer<TELECOMMAND::TELECOMMAND_MSG> telecommandBuffer;

// Subscribers
RODOS::Subscriber stateSubscriber(TOPICS::SYSTEM_STATE_TOPIC, stateBuffer, "SYSTEM_STATE_S");
RODOS::Subscriber signalIntervalSubscriber(TOPICS::SIGNAL_INTERVAL_TOPIC, signalIntervalBuffer, "SIGNAL_INTERVAL_S");
RODOS::Subscriber telemetryIntervalSubscriber(TOPICS::TELEMETRY_INTERVAL_TOPIC, telemetryIntervalBuffer, "TELEMETRY_INTERVAL_S");
RODOS::Subscriber telemetryMsgSubscriber(TOPICS::TELEMETRY_TOPIC, telemetryMsgBuffer, "TELEMETRY_MSG_S");
RODOS::Subscriber telecommandSubscriber(TOPICS::TELECOMMAND_TOPIC, telecommandBuffer, "TELECOMMAND_MSG_S");

// LEDs
HAL_GPIO gpioBlue((GPIO_PIN) STM32F4::BOARD_LEDS::LED_BLUE);
HAL_GPIO gpioRed((GPIO_PIN) STM32F4::BOARD_LEDS::LED_RED);
HAL_GPIO gpioGreen((GPIO_PIN) STM32F4::BOARD_LEDS::LED_GREEN);
HAL_GPIO gpioOrange((GPIO_PIN) STM32F4::BOARD_LEDS::LED_ORANGE);

// Uart
HAL_UART uart(UART_IDX1);

// Threads
SignalProcessorThread signalThread(&sensor,
		400000,
		&signalData,
		&calibrationData,
		&readingErrorData,
		&aliveSignalData,
		&stateBuffer,
		&signalIntervalBuffer,
		&gpioRed,
		5000*MILLISECONDS,
		&gpioGreen,
		"SignalProcessorThread");

TelecommandThread telecommandThread(5000*MILLISECONDS,
		&gpioOrange,
		&uart,
		115200,
		END_OF_TIME,
		&stateBuffer,
		"TelecommandThread");

TelemetryThread telemetryThread(10*MILLISECONDS,
		&gpioBlue,
		&telemetryIntervalBuffer,
		&telemetryMsgBuffer,
		"TelemetryThread");

