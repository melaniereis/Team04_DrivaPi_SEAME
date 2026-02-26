/**
  ******************************************************************************
  * @file    firmware/Core/Src/servo_motor.c
  * @author  DrivaPi Team
  * @brief   This file contains the servo motor control functions.
  ******************************************************************************
  * @attention
  *
  */

#include "servo_motor.h"

/**
 * @brief Set a servo angle using PCA9685 PWM output.
 *
 * @param channel PWM channel index.
 * @param angle_deg Target angle in degrees.
 * @return int 1 on success.
 */
int SetServoAngle(uint8_t channel, uint16_t angle_deg)
{
	if (angle_deg > 180u)
		angle_deg = 180u;

	uint16_t range = SERVO_MAX_PULSE - SERVO_MIN_PULSE;
	uint16_t pulse = SERVO_MIN_PULSE + (range * angle_deg) / 180u;
	PCA9685_SetPWM(PCA9685_ADDR_SERVO, channel, 0, pulse);
	return 1;
}

/**
 * @brief Sweep a servo between two angles in steps.
 *
 * @param channel PWM channel index.
 * @param start_angle Start angle in degrees.
 * @param end_angle End angle in degrees.
 * @param step_angle Step size in degrees.
 * @return int 0 on completion.
 */
int ServoSweep(uint8_t channel, uint16_t start_angle, uint16_t end_angle,
uint16_t step_angle)
{
	if (step_angle == 0)
		step_angle = 1;

	if (start_angle <= end_angle)
	{
		return 1;
	}
	return 0;
}


/**
 * @brief Servo motor thread entry that handles steering commands.
 *
 * @param initial_input ThreadX initial input (unused).
 * @return void
 */
void ServoMotor(ULONG initial_input)
{
	t_can_message msg;
	ULONG actual_flags;

	while (1)
	{
		tx_event_flags_get(&g_eventFlags, FLAG_CAN_STEER_CMD, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
		
		while (tx_queue_receive(&g_queueSteerCmd, &msg, TX_NO_WAIT) == TX_SUCCESS)
		{
			tx_mutex_get(&g_servoMutex, TX_WAIT_FOREVER);

			float angle_f = *((float *)msg.data);
			uint16_t angle = (uint16_t)angle_f;
			SetServoAngle(SERVO_CH, angle);

			tx_mutex_put(&g_servoMutex);
		}
	}
}
