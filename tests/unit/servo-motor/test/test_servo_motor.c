/**
 ******************************************************************************
 * @file    test_servo_motor.c
 * @brief   Unit tests for real servo_motor.c using CMock (speed-sensor pattern)
 * @details Tests the actual firmware code by including it directly and mocking
 *          HAL/PCA9685 dependencies using CMock from common/mocks
 ******************************************************************************
 */

#include "main.h"
#include "mock_stm32u5xx_hal.h"
#include "mock_tx_api.h"
#include "mock_pca9685.h"

/* --- 3. GLOBAL VARIABLES (required by firmware) --- */
TX_MUTEX g_speedDataMutex;
TX_EVENT_FLAGS_GROUP g_eventFlags;
TX_QUEUE g_queueSpeedCmd;
TX_QUEUE g_queueSteerCmd;

/* --- 4. FAKE HARDWARE --- */
I2C_HandleTypeDef hi2c3;
UART_HandleTypeDef huart1;

/* --- 5. INCLUDE SOURCE FILE DIRECTLY --- */
#include "../../../firmware/Core/Src/servo_motor.c"

/* --- 6. TEST INFRASTRUCTURE --- */
void Error_Handler(void) {
    // No-op for tests
}

void UartPrintf(const char *format, ...) {
    // Stub for tests
}

// ========================================================
// TESTS FOR SetServoAngle - Basic Angles
// ========================================================

void test_SetServoAngle_ShouldReturnSuccessFor0Degrees(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MIN_PULSE, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 0);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldAccept90Degrees(void)
{
    // Arrange
    uint16_t expected_pulse = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * 90) / 180;
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, expected_pulse, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 90);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldAccept180Degrees(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MAX_PULSE, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 180);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

// ========================================================
// TESTS FOR SetServoAngle - Angle Clamping (> 180°)
// ========================================================

void test_SetServoAngle_ShouldClampAbove180Degrees(void)
{
    // Arrange: 181° should clamp to 180°
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MAX_PULSE, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 181);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldClamp181Degrees(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MAX_PULSE, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 181);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldClamp250Degrees(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MAX_PULSE, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 250);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldClamp500Degrees(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MAX_PULSE, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 500);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldClampMaxUint16(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MAX_PULSE, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 65535);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

// ========================================================
// TESTS FOR SetServoAngle - Pulse Calculation
// ========================================================

void test_SetServoAngle_ShouldCalculateCorrectPulseFor45Degrees(void)
{
    // Arrange
    uint16_t expected_pulse = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * 45) / 180;
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, expected_pulse, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 45);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldCalculateCorrectPulseFor135Degrees(void)
{
    // Arrange
    uint16_t expected_pulse = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * 135) / 180;
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, expected_pulse, HAL_OK);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 135);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

// ========================================================
// TESTS FOR SetServoAngle - Error Handling
// ========================================================

void test_SetServoAngle_ShouldReturnFailureWhenPCA9685Fails(void)
{
    // Arrange
    uint16_t pulse = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * 90) / 180;
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, pulse, HAL_ERROR);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 90);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_SetServoAngle_ShouldReturnFailureWhenPCA9685Busy(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MIN_PULSE, HAL_BUSY);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 0);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_SetServoAngle_ShouldReturnFailureWhenPCA9685Timeout(void)
{
    // Arrange
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, SERVO_CH, 0, SERVO_MIN_PULSE, HAL_TIMEOUT);
    
    // Act
    int result = SetServoAngle(SERVO_CH, 0);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(0, result);
}

// ========================================================
// TESTS FOR SetServoAngle - Different Channels
// ========================================================

void test_SetServoAngle_ShouldUseCorrectChannel0(void)
{
    // Arrange
    uint16_t pulse = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * 90) / 180;
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, 0, 0, pulse, HAL_OK);
    
    // Act
    int result = SetServoAngle(0, 90);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldUseCorrectChannel5(void)
{
    // Arrange
    uint16_t pulse = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * 90) / 180;
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, 5, 0, pulse, HAL_OK);
    
    // Act
    int result = SetServoAngle(5, 90);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_SetServoAngle_ShouldUseCorrectChannel15(void)
{
    // Arrange
    uint16_t pulse = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) * 90) / 180;
    PCA9685_SetPWM_ExpectAndReturn(PCA9685_ADDR_SERVO, 15, 0, pulse, HAL_OK);
    
    // Act
    int result = SetServoAngle(15, 90);
    
    // Assert
    TEST_ASSERT_EQUAL_INT(1, result);
}
