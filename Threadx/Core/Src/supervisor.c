#include "app_threadx.h"

/**
 * @brief
 *
 * @param initial_input
 * @return VOID
 */
void ld1_ThreadEntry(ULONG initial_input)
{
	const char *msg_tick = "Supervising\r\n";

	while (1)
	{
		HAL_UART_Transmit(&huart1, (uint8_t *)msg_tick, strlen(msg_tick), 10);
		tx_thread_sleep(50);
	}
}
