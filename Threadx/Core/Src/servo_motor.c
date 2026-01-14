/**
  ******************************************************************************
  * @file    Threadx/Core/Src/servo_motor.c
  * @author  DrivaPi Team
  * @brief   This file contains the servo motor control functions.
  ******************************************************************************
  * @attention
  *
  */

#include "servo_motor.h"

/**
 * @brief
 *
 * @param hi2c
 * @param addr7
 * @param channel
 * @param angle_deg
 * @param min_pulse
 * @param max_pulse
 * @return int
 */
int SetServoAngle(I2C_HandleTypeDef *hi2c, uint8_t addr7, uint8_t channel, uint16_t angle_deg, uint16_t min_pulse,
uint16_t max_pulse)
{
	if (angle_deg > 180u)
		angle_deg = 180u;

	if (min_pulse >= max_pulse)
		return -1;

	uint32_t range = (uint32_t)max_pulse - (uint32_t)min_pulse;
	uint32_t pulse = (uint32_t)min_pulse + (range * angle_deg) / 180u;

	HAL_StatusTypeDef st = PCA9685_SetPWM(hi2c, addr7, channel, 0, ClampU16((int32_t)pulse));
	if (st != HAL_OK)
		return -2;

	return 0;
}

/**
 * @brief
 *
 * @param hi2c
 * @param addr7
 * @param channel
 * @param start_angle
 * @param end_angle
 * @param step_angle
 * @param delay_ms
 * @param min_pulse
 * @param max_pulse
 * @return int
 */
int ServoSweep(I2C_HandleTypeDef *hi2c, uint8_t addr7, uint8_t channel, uint16_t start_angle, uint16_t end_angle,
uint16_t step_angle, uint32_t delay_ms, uint16_t min_pulse, uint16_t max_pulse)
{
	double freq = (double)SERVO_DEFAULT_FREQ_HZ;
	if (PCA9685_SetFrequency(hi2c, addr7, freq) != HAL_OK)
	{
		const char *msg = "Servo_Sweep: Failed to set frequency, continuing anyway\r\n";
		HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	}

	if (step_angle == 0)
		step_angle = 1;

	if (start_angle <= end_angle)
	{
		for (uint16_t angle = start_angle; angle <= end_angle; angle += step_angle)
		{
			SetServoAngle(hi2c, addr7, channel, angle, min_pulse, max_pulse);
			HAL_Delay(delay_ms);
			if (angle > UINT16_MAX - step_angle)
				break;
		}
	}
	else
	{
		for (int angle = (int)start_angle; angle >= (int)end_angle; angle -= (int)step_angle)
		{
			SetServoAngle(hi2c, addr7, channel, (uint16_t)angle, min_pulse, max_pulse);
			HAL_Delay(delay_ms);
			if (angle - (int)step_angle > angle)
				break;
		}
	}
	const char *msg = "Servo_Sweep: complete\r\n";
	HAL_UART_Transmit(&huart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
	return 0;
}

/**
 * @brief
 *
 * @param initial_input
 * @return void
 */
void ServoMotor(ULONG initial_input)
{
	t_can_message msg;
	ULONG actual_flags;

	while (1)
	{
		tx_event_flags_get(&g_eventFlags, FLAG_CAN_STEER_CMD, TX_OR_CLEAR, &actual_flags, TX_NO_WAIT);

		while (tx_queue_receive(&g_queueSteerCmd, &msg, TX_NO_WAIT) == TX_SUCCESS)
		{
			float angle_f = *((float *)msg.data);
			uint16_t angle = (uint16_t)angle_f;
			(void)SetServoAngle(&hi2c3, PCA9685_ADDR_SERVO, SERVO_CH, angle, SERVO_DEFAULT_MIN_PULSE_COUNTS, SERVO_DEFAULT_MAX_PULSE_COUNTS);
		}
		tx_thread_sleep(50);
	}
}