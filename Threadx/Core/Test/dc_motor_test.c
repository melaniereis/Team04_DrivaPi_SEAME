#include "dc_motor_test.h"

/**
 * @brief
 *
 * @param s
 */
static void UartPrint(const char *s)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)s, strlen(s), HAL_MAX_DELAY);
}

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
