/**
  ******************************************************************************
  * @file    dc_motor_test.c
  * @author  Team04 DrivaPi
  * @brief   This file contains the implementation of DC motor test functions.
  ******************************************************************************
  * @attention
  *
*/
#include "dc_motor_test.h"

/**
 * @brief
 *
 * @param speed
 */
void MotorTestHigh(double speed) {

    UartPrint("-> FORWARD (1.5s)...\r\n");
    MotorForward(speed);
    tx_thread_sleep(150);
    tx_thread_sleep(30);

    UartPrint("-> BACKWARD (1.5s)...\r\n");
    MotorBackward(speed);
    tx_thread_sleep(150);
    MotorStop();
    tx_thread_sleep(30);

    UartPrint("-> LEFT (1.2s)...\r\n");
    MotorLeft(speed);
    tx_thread_sleep(120);
    MotorStop();
    tx_thread_sleep(30);

    UartPrint("-> RIGHT (1.2s)...\r\n");
    MotorRight(speed);
    tx_thread_sleep(120);
    MotorStop();

    UartPrint("Motor_Test_High: complete\r\n");
}
