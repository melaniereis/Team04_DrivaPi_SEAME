#include "dc_motor_test.h"

/**
 * @brief
 *
 * @param s
 */
static void UART_Print(const char *s)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)s, strlen(s), HAL_MAX_DELAY);
}

/**
 * @brief
 *
 * @param speed
 */
void Motor_Test_High(double speed) {

    UART_Print("-> FORWARD (1.5s)...\r\n");
    Motor_Forward(speed);
    tx_thread_sleep(150);
    tx_thread_sleep(30);

    UART_Print("-> BACKWARD (1.5s)...\r\n");
    Motor_Backward(speed);
    tx_thread_sleep(150);
    Motor_Stop();
    tx_thread_sleep(30);

    UART_Print("-> LEFT (1.2s)...\r\n");
    Motor_Left(speed);
    tx_thread_sleep(120);
    Motor_Stop();
    tx_thread_sleep(30);

    UART_Print("-> RIGHT (1.2s)...\r\n");
    Motor_Right(speed);
    tx_thread_sleep(120);
    Motor_Stop();

    UART_Print("Motor_Test_High: complete\r\n");
}
