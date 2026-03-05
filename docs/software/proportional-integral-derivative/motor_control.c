#include "motor_control.h"

void MotorPIDUpdate(MotorPIDState *state, float current_speed)
{
    // Step 1: Calculate error (difference between target and current speed)
    state->error = state->target_speed - current_speed;
    
    // Step 2: Proportional term (immediate response)
    float p_term = state->gain_p * state->error;
    
    // Step 3: Integral term (eliminates steady-state error)
    state->integral += state->error * PID_SAMPLE_TIME;
    
    // Anti-windup: prevent integral from growing too large
    if (state->integral > 100.0f) 
        state->integral = 100.0f;
    if (state->integral < -100.0f) 
        state->integral = -100.0f;
    
    float i_term = state->gain_i * state->integral;
    
    // Step 4: Derivative term (reduces overshoot)
    float derivative = (state->error - state->error_prev) / PID_SAMPLE_TIME;
    float d_term = state->gain_d * derivative;
    
    // Step 5: Sum all three terms
    state->pwm_output = p_term + i_term + d_term;
    
    // Step 6: Clamp output to valid PWM range [0.0, 1.0]
    if (state->pwm_output > 1.0f) 
        state->pwm_output = 1.0f;
    if (state->pwm_output < 0.0f) 
        state->pwm_output = 0.0f;
    
    // Step 7: Convert normalized PWM (0.0–1.0) to raw value (0–4095)
    state->pwm_raw = (uint16_t)(state->pwm_output * 4095.0f);
    
    // Step 8: Apply dead zone minimum (motor won't move below this PWM)
    if (state->pwm_raw > 0 && state->pwm_raw < PWM_MIN)
        state->pwm_raw = PWM_MIN;
    
    // Step 9: Send PWM to PCA9685 via I2C
    // Example: PCA9685_SetPWM(state->pca9685_channel, 0, state->pwm_raw);
    PCA9685_SetPWM(state->pca9685_channel, 0, state->pwm_raw);
    
    // Step 10: Store current error for derivative calculation in next cycle
    state->error_prev = state->error;
    state->current_speed = current_speed;
}


// In your main control loop (100 ms interval, matches SpeedSensor thread):
extern float       g_vehicleSpeed;     // from speed_sensor.c (measured m/s)
extern float       g_targetSpeed;      // from CAN message (remote command m/s)
extern MotorPIDState g_motorPidState;   // motor PID controller state

void UpdateMotorControl(void)
{
    // CAN receiver has already populated g_targetSpeed
    
    // Update motor PID controller with current speed feedback
    g_motorPidState.target_speed = g_targetSpeed;
    MotorPIDUpdate(&g_motorPidState, g_vehicleSpeed);
}
