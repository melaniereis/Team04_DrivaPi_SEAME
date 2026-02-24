/**
 ******************************************************************************
 * @file    test_dc_motor.c
 * @brief   Unit tests for MotorSetPWM() function using CMock
 * @details Tests the real MotorSetPWM from firmware/Core/Src/dc_motor.c
 *          The function is compiled from the actual firmware source file
 ******************************************************************************
 */

#include "main.h"
#include "mock_stm32u5xx_hal.h"
#include "mock_tx_api.h"
#include "mock_pca9685.h"

/* --- GLOBAL VARIABLES (required by firmware) --- */
TX_MUTEX g_speedDataMutex;
TX_EVENT_FLAGS_GROUP g_eventFlags;
TX_QUEUE g_queueSpeedCmd;
TX_QUEUE g_queueSteerCmd;

/* --- FAKE HARDWARE --- */
I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart1;

/* --- INCLUDE FIRMWARE SOURCE FILES DIRECTLY --- */
#include "../../../firmware/Core/Src/motor_utils.c"
#include "../../../firmware/Core/Src/dc_motor.c"

/* --- TEST INFRASTRUCTURE --- */
void Error_Handler(void) {
    // No-op for tests
}

// ========================================================
// TESTS FOR MotorSetPWM - ONLY TESTED FUNCTION
// ========================================================

void test_MotorSetPWM_ShouldSetBothMotors(void)
{
    // Arrange - positive counts move motors forward
    int32_t left_counts = 2048;
    int32_t right_counts = 2048;
    const uint16_t max = 4095;
    uint16_t left_pwm = ClampU16(left_counts);
    uint16_t right_pwm = ClampU16(right_counts);
    
    // Left motor forward: A=max, B=0
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, left_pwm, HAL_OK);
    
    // Right motor forward: A=0, B=max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, right_pwm, HAL_OK);
    
    // Act
    MotorSetPWM(left_counts, right_counts);
    
    // Assert
}

void test_MotorSetPWM_WithNegativeCounts(void)
{
    // Arrange - negative counts move motors backward
    int32_t left_counts = -2048;
    int32_t right_counts = -2048;
    const uint16_t max = 4095;
    uint16_t left_pwm = ClampU16(-left_counts);   // 2048
    uint16_t right_pwm = ClampU16(-right_counts); // 2048
    
    // Left motor backward: A=0, B=max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, left_pwm, HAL_OK);
    
    // Right motor backward: A=max, B=0
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, right_pwm, HAL_OK);
    
    // Act
    MotorSetPWM(left_counts, right_counts);
    
    // Assert
}

void test_MotorSetPWM_WithMixedDirections(void)
{
    // Arrange - left forward, right backward
    int32_t left_counts = 3000;
    int32_t right_counts = -3000;
    const uint16_t max = 4095;
    uint16_t left_pwm = ClampU16(left_counts);      // 3000
    uint16_t right_pwm = ClampU16(-right_counts);   // 3000
    
    // Left motor forward: A=max, B=0
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, left_pwm, HAL_OK);
    
    // Right motor backward: A=max, B=0
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, right_pwm, HAL_OK);
    
    // Act
    MotorSetPWM(left_counts, right_counts);
    
    // Assert
}

void test_MotorSetPWM_WithClampingPositive(void)
{
    // Arrange - value above max should clamp to 4095
    int32_t left_counts = 5000;
    int32_t right_counts = 10000;
    const uint16_t max = 4095;
    uint16_t left_pwm = ClampU16(left_counts);    // 4095 (clamped)
    uint16_t right_pwm = ClampU16(right_counts);  // 4095 (clamped)
    
    // Left motor forward at clamped max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, left_pwm, HAL_OK);
    
    // Right motor forward at clamped max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, right_pwm, HAL_OK);
    
    // Act
    MotorSetPWM(left_counts, right_counts);
    
    // Assert
}

void test_MotorSetPWM_WithClampingNegative(void)
{
    // Arrange - large negative values should clamp to 4095 after negation
    int32_t left_counts = -5000;
    int32_t right_counts = -10000;
    const uint16_t max = 4095;
    uint16_t left_pwm = ClampU16(-left_counts);    // 4095 (clamped from 5000)
    uint16_t right_pwm = ClampU16(-right_counts);  // 4095 (clamped from 10000)
    
    // Left motor backward at clamped max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, left_pwm, HAL_OK);
    
    // Right motor backward at clamped max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, right_pwm, HAL_OK);
    
    // Act
    MotorSetPWM(left_counts, right_counts);
    
    // Assert
}
void test_MotorSetPWM_WithZeroCounts(void)
{
    // Arrange - zero counts stop both motors (all channels to 0)
    int32_t left_counts = 0;
    int32_t right_counts = 0;
    
    // Left motor stops (all 0)
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, 0, HAL_OK);
    
    // Right motor stops (all 0)
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, 0, HAL_OK);
    
    // Act
    MotorSetPWM(left_counts, right_counts);
    
    // Assert
}

void test_MotorSetPWM_WithMaxCounts(void)
{
    // Arrange - max positive counts
    int32_t left_counts = 4095;
    int32_t right_counts = 4095;
    const uint16_t max = 4095;
    uint16_t left_pwm = ClampU16(left_counts);
    uint16_t right_pwm = ClampU16(right_counts);
    
    // Left motor forward at max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, left_pwm, HAL_OK);
    
    // Right motor forward at max
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, max, HAL_OK);
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, right_pwm, HAL_OK);
    
    // Act
    MotorSetPWM(left_counts, right_counts);
    
    // Assert
}
