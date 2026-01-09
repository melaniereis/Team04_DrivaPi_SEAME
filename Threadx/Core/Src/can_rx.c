#include "app_threadx.h"

/**
* @brief
*
* @param msg
* @return uint8_t
*/
uint8_t CanReceive(t_can_message *msg)
{
	FDCAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];

	if (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan1, FDCAN_RX_FIFO0) > 0)
	{
		if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rx_header, rx_data) == HAL_OK)
		{
			msg->id = rx_header.Identifier;
			msg->len = (rx_header.DataLength <= 8) ? rx_header.DataLength : 8;
			memcpy(msg->data, rx_data, msg->len);
			return 1; // Message received
		}
	}
	return 0;
}

/**
* @brief
*
* @param initial_input
* @return void
*/
void CanRx(ULONG initial_input)
{
	t_can_message msg;

	const char *msg_tick = "RX\r\n";
	while (1)
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)msg_tick, strlen(msg_tick), 10);
		if (CanReceive(&msg))
		{
			switch (msg.id)
			{
				case CMD_SPEED:
					tx_queue_send(&queue_speed_cmd, &msg, TX_NO_WAIT);
					tx_event_flags_set(&event_flags, FLAG_CAN_SPEED_CMD, TX_OR);

				case CMD_STEERING:
					tx_queue_send(&queue_steer_cmd, &msg, TX_NO_WAIT);
					tx_event_flags_set(&event_flags, FLAG_CAN_STEER_CMD, TX_OR);
			}
		}
		tx_thread_sleep(100);
	}
}
