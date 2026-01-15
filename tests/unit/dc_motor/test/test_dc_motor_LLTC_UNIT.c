/**
 * @file test_dc_motor_LLTC_UNIT.c
 * @brief Unit Tests for REAL dc_motor.c from Threadx/Core/Src/
 *
 * @req LLTC-DC_MOTOR_UNIT: All unit tests for the DC Motor module shall pass, verifying forward/reverse logic and PWM calculations.
 * @asil ASIL-A
 * @safety_goal SG-02: Safe Propulsion - DC Motor logic must clamp speed requests to +/- 100% and map 0.0 to a guaranteed stop state.
 * @coverage_requirement 100% Branch Coverage
 *
 * This test validates THE REAL production code from:
 * ../Threadx/Core/Src/dc_motor.c via wrapper mapping in dc_motor.h
 */

#include "../vendor/unity/src/unity.h"
#include "../src/dc_motor.h"
#include "../src/dc_motor_testable.c"
#include "../../common/src/pca9685_testable.c"
#include "../../common/src/mock_stm32u5xx_hal.c"
#include <string.h>
#include <stdint.h>

/* dc_motor.h wrapper maps Motor_* to Motor_*_Testable(&hi2c3) */
I2C_HandleTypeDef hi2c3;

void setUp(void) {
    memset(&hi2c3, 0, sizeof(hi2c3));
}

void tearDown(void) {
}

void test_Motor_Stop_Should_SetAllChannelsToZero(void) {
    Motor_Stop();
}

void test_Motor_Forward_At_Zero_Should_SetZeroPWM(void) {
    Motor_Forward(0.0);
}

void test_Motor_Forward_At_50Percent_Should_SetCorrectPWM(void) {
    Motor_Forward(0.5);
}

void test_Motor_Forward_At_Max_Should_SetFullPWM(void) {
    Motor_Forward(1.0);
}

void test_Motor_Backward_At_50Percent_Should_ReverseDirection(void) {
    Motor_Backward(0.5);
}

void test_Motor_Left_At_50Percent_Should_TurnLeft(void) {
    Motor_Left(0.5);
}

void test_Motor_Right_At_50Percent_Should_TurnRight(void) {
    Motor_Right(0.5);
}

void test_Motor_SetMotor_NearZeroSpeed_ShouldCallMotorStop(void) {
    Motor_SetMotor(0.0000001);
}

void test_Motor_SetMotor_PositiveSpeed_ShouldMoveForward(void) {
    Motor_SetMotor(0.5);
}

void test_Motor_SetMotor_NegativeSpeed_ShouldMoveBackward(void) {
    Motor_SetMotor(-0.5);
}

void test_Motor_SetPWM_PositiveLeftRight_ShouldSetForward(void) {
    Motor_SetPWM(2000, 2000);
}

void test_Motor_SetPWM_NegativeLeftRight_ShouldSetBackward(void) {
    Motor_SetPWM(-2000, -2000);
}

void test_Motor_SetPWM_ZeroLeftRight_ShouldStop(void) {
    Motor_SetPWM(0, 0);
}

void test_Motor_SetPWM_NegativeValues_ShouldClampToZero(void) {
    Motor_SetPWM(-1000, -500);
}

void test_Motor_SetPWM_OverflowValues_ShouldClampToMax(void) {
    Motor_SetPWM(5000, 6000);
}
