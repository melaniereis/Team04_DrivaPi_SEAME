/**
  ******************************************************************************
  * @file    firmware/Core/Src/supervisor.c
  * @author  DrivaPi Team
  * @brief   This file contains the supervisor thread function.
  ******************************************************************************
  * @attention
  *
  */

#include "app_threadx.h"

/**
 * @brief
 *
 * @param initial_input
 * @return VOID
 */
void ld1_ThreadEntry(ULONG initial_input)
{
	//const char *msg_tick = "Supervising\r\n";

	while (1)
	{
//		ServoSweep(0, 20, 120,15);
		//HAL_UART_Transmit(&huart1, (uint8_t *)msg_tick, strlen(msg_tick), 10);
		tx_thread_sleep(50);
	}
}
