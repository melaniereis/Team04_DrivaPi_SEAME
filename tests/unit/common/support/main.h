#ifndef MAIN_H
#define MAIN_H

/* Standard C library headers */
#include <string.h>
#include <setjmp.h>
#include <math.h>
#include <stdarg.h>

/* Unity test framework */
#include "unity.h"

/* ThreadX and HAL headers (for type definitions) */
#include "tx_api.h"
#include "stm32u5xx_hal.h"

void Error_Handler(void);
void UartPrintf(const char *format, ...);

/* Globals (will be defined by test) */
extern TX_MUTEX g_speedDataMutex;
extern TX_EVENT_FLAGS_GROUP g_eventFlags;
extern TX_QUEUE g_queueSpeedCmd;
extern TX_QUEUE g_queueSteerCmd;
extern I2C_HandleTypeDef hi2c3;
extern UART_HandleTypeDef huart1;
extern float g_vehicleSpeed;

#endif
