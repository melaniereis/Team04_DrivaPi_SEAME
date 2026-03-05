#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <app_threadx.h>

#define PID_SAMPLE_TIME 0.1f   // 100 ms update rate (matches SpeedSensor thread)
#define PWM_MIN 300u           // Minimum PWM to overcome dead zone
#define PWM_MAX 4095u          // Maximum PWM value

/**
 * @struct MotorPIDState
 * @brief PID controller state for the motor
 */
typedef struct {
	float       target_speed;      // desired speed (m/s)
	float       current_speed;     // measured speed (m/s) from speed_sensor.c
	float       error;             // e[n]
	float       error_prev;        // e[n-1] for derivative
	float       integral;          // sum of errors for integral term
	
	float       gain_p, gain_i, gain_d;        // PID gains
	float       pwm_output;        // computed PWM (0.0–1.0)
	uint16_t    pwm_raw;			// raw PWM (0–4095) for PCA9685
	uint8_t     pca9685_channel;	// PCA9685 channel for motor control
} MotorPIDState;

/**
 * @brief Compute PID control output and send to PCA9685
 */
void MotorPIDUpdate(MotorPIDState *state, float current_speed);

#endif