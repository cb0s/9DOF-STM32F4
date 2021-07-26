/*
 * initialize.cpp
 *
 *  Created on: 11.06.2021
 *      Author: Cedric Boes
 */


#include "rodos.h"

#include "devices/stm32f4.h"

#include "messages/telemetry.h"
#include "messages/telecommands.h"
#include "messages/internal.h"

#include "sensors/constants/lsm9ds1_const.h"
#include "sensors/i2c/lsm9ds1_i2c.h"
#include "sensors/registers/registers.h"
#include "sensors/Lsm9ds1HalSpi.h"

#include "threads/SignalProcessorThread.h"
#include "threads/TelecommandThread.h"
#include "threads/TelemetryThread.h"

#include "topics.h"

// Sensor
static HAL_GPIO csAG(GPIO_006);
static HAL_GPIO csM(GPIO_041);
static HAL_SPI lsm9ds1_spi(SPI_IDX1, GPIO_019, GPIO_020, GPIO_021);
Lsm9ds1HalSpi sensor(&lsm9ds1_spi, &csAG, &csM);

// SystemT
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
TELEMETRY::TELEMETRY_MSG telMsg;
INTERNAL_MSG::CALIBRATION calMsg;
INTERNAL_MSG::MEASUREMENT datMsg;

// Comm-Buffers
RODOS::CommBuffer<uint8_t> stateBuffer;
RODOS::CommBuffer<uint64_t> signalIntervalBuffer;
RODOS::CommBuffer<uint64_t> telemetryIntervalBuffer;
RODOS::CommBuffer<INTERNAL_MSG::MEASUREMENT> measurementBuffer;
RODOS::CommBuffer<INTERNAL_MSG::CALIBRATION> calibrationBuffer;
RODOS::CommBuffer<TELEMETRY::TELEMETRY_MSG> telemetryMsgBuffer;
RODOS::CommBuffer<TELECOMMAND::TELECOMMAND_MSG> telecommandBuffer;

// Subscribers
RODOS::Subscriber stateSubscriber(TOPICS::SYSTEM_STATE_TOPIC, stateBuffer, "SYSTEM_STATE_S");
RODOS::Subscriber signalIntervalSubscriber(TOPICS::SIGNAL_INTERVAL_TOPIC, signalIntervalBuffer, "SIGNAL_INTERVAL_S");
RODOS::Subscriber telemetryIntervalSubscriber(TOPICS::TELEMETRY_INTERVAL_TOPIC, telemetryIntervalBuffer, "TELEMETRY_INTERVAL_S");
RODOS::Subscriber currentMeasurementSubscriber(TOPICS::CURRENT_DAT_TOPIC, measurementBuffer, "MEASUREMENT_INTERNAL_S");
RODOS::Subscriber currentCalibrationSubscriber(TOPICS::CURRENT_CAL_TOPIC, calibrationBuffer, "CALIBRATION_INTERNAL_S");
RODOS::Subscriber telemetryMsgSubscriber(TOPICS::TELEMETRY_TOPIC, telemetryMsgBuffer, "TELEMETRY_MSG_S");
RODOS::Subscriber telecommandSubscriber(TOPICS::TELECOMMAND_TOPIC, telecommandBuffer, "TELECOMMAND_MSG_S");

// LEDs
HAL_GPIO gpioBlue((GPIO_PIN) STM32F4::BOARD_LEDS::LED_BLUE);
HAL_GPIO gpioRed((GPIO_PIN) STM32F4::BOARD_LEDS::LED_RED);
HAL_GPIO gpioGreen((GPIO_PIN) STM32F4::BOARD_LEDS::LED_GREEN);
HAL_GPIO gpioOrange((GPIO_PIN) STM32F4::BOARD_LEDS::LED_ORANGE);

//// Uart
//HAL_UART uart(UART_IDX1);
namespace RODOS {
	extern HAL_UART uart_stdout;
}

// Threads
SignalProcessorThread signalThread(&sensor,
		&datMsg,
		&calMsg,
		&telMsg,
		&stateBuffer,
		&signalIntervalBuffer,
		&gpioRed,
		5000*MILLISECONDS,
		&gpioGreen,
		"SignalProcessorThread");

TelecommandThread telecommandThread(MILLISECONDS,
		&gpioOrange,
		&uart_stdout,
		115200,
		END_OF_TIME,
		&stateBuffer,
		"TelecommandThread");

TelemetryThread telemetryThread(1000*MILLISECONDS,
		&gpioBlue,
		&telemetryIntervalBuffer,
		&telemetryMsgBuffer,
		&measurementBuffer,
		&calibrationBuffer,
		"TelemetryThread");
