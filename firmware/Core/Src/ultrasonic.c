#include "soft_i2c.h"
#include "app_threadx.h"
#include "dc_motor.h" // Needed for SetMotor/MotorStop
#include <stdio.h>
#include <stdlib.h>

// SRF08 Commands
#define SRF08_ADDR          0xE0
#define CMD_REG             0x00
#define RANGE_REG           0x02
#define CMD_CENTIMETERS     0x51

// Safety Thresholds
#define BRAKE_THRESHOLD_CM  7   // Hard Stop
#define TTC_THRESHOLD_MS    200 // Predictive Stop
#define DT_SECONDS          0.06f

void UltrasonicEntry(ULONG initial_input)
{
    // --- INIT ---
    Soft_I2C_Init();
    tx_thread_sleep(10);

    // --- CONFIGURATION ---
    uint8_t verify_range = 0;
    int config_attempts = 0;
    while (verify_range != 0x46 && config_attempts < 10){
        config_attempts++;
        Soft_I2C_Start();
        if (Soft_I2C_WriteByte(SRF08_ADDR) == 1) {
            Soft_I2C_WriteByte(0x02); Soft_I2C_WriteByte(0x46); // 3m Range
        }
        Soft_I2C_Stop();
        tx_thread_sleep(5);

        // Verify
        Soft_I2C_Start();
        if (Soft_I2C_WriteByte(SRF08_ADDR) == 1) {
            Soft_I2C_WriteByte(0x02); Soft_I2C_Stop();
            Soft_I2C_Start(); Soft_I2C_WriteByte(SRF08_ADDR|1);
            verify_range = Soft_I2C_ReadByte(0);
        }
        Soft_I2C_Stop();
        tx_thread_sleep(5);
    }

    // Print Status
    if (verify_range == 0x46)
    	HAL_UART_Transmit(&huart1, (uint8_t*)"[US] CONFIG OK\r\n", 16, 100);
    else
    	HAL_UART_Transmit(&huart1, (uint8_t*)"[US] CONFIG FAIL\r\n", 18, 100);

    // --- VARIABLES ---
    uint8_t high_byte, low_byte;
    int16_t range_cm = 0;
    int16_t dist_old = 0;
    float velocity_cm_s = 0;
    float ttc_ms = 9999;

    while(1)
    {
        // 1. PING
        Soft_I2C_Start();
        if (Soft_I2C_WriteByte(SRF08_ADDR) == 0) { Soft_I2C_Stop(); tx_thread_sleep(1); continue; }
        Soft_I2C_WriteByte(CMD_REG); Soft_I2C_WriteByte(CMD_CENTIMETERS); Soft_I2C_Stop();

        // 2. WAIT
        tx_thread_sleep(6);

        // 3. READ
        Soft_I2C_Start();
        if (Soft_I2C_WriteByte(SRF08_ADDR) == 0) { Soft_I2C_Stop(); continue; }
        Soft_I2C_WriteByte(RANGE_REG); Soft_I2C_Stop();

        Soft_I2C_Start();
        Soft_I2C_WriteByte(SRF08_ADDR | 1);
        high_byte = Soft_I2C_ReadByte(1);
        low_byte  = Soft_I2C_ReadByte(0);
        Soft_I2C_Stop();

        range_cm = (high_byte << 8) | low_byte;

        // 4. PHYSICS & SAFETY
        if (range_cm > 0 && range_cm < 80)
        {
            // Velocity
            int16_t delta = dist_old - range_cm;
            if (abs(delta) > 1) velocity_cm_s = (float)delta / DT_SECONDS;
            else velocity_cm_s = 0;

            // TTC
            if (velocity_cm_s > 10.0f) ttc_ms = ((float)range_cm / velocity_cm_s) * 1000.0f;
            else ttc_ms = 9999.0f;

            // Log
            char msg[64];
            snprintf(msg, sizeof(msg), "D:%d cm | TTC:%.0f ms\r\n", range_cm, ttc_ms);
            HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 10);

            // --- THE KILL SWITCH (Direct Action) ---
            if (ttc_ms < TTC_THRESHOLD_MS || range_cm < BRAKE_THRESHOLD_CM)
            {

        		tx_mutex_get(&g_emergencyMutex, TX_WAIT_FOREVER);
        		g_emergencyBrake = true;
        		tx_mutex_put(&g_emergencyMutex);
                HAL_UART_Transmit(&huart1, (uint8_t*)"!!! KILLING MOTORS !!!\r\n", 24, 100);

                // A. Suspend the DC Motor Driver Thread
                // This prevents it from overwriting our stop command

                // B. Active Braking (Reverse)
                MotorStop();


                if (range_cm > BRAKE_THRESHOLD_CM)
                	SetMotor(-1.0);

                // C. Hold Brake for 10 ticks
                // Since this is the Ultrasonic thread, sleeping here effectively
                // pauses the sensor updates, which is fine because we are stopping.
                tx_thread_sleep(7);

                // D. Final Stop
                MotorStop();

                tx_thread_sleep(10);
            }
            else
            {
        		tx_mutex_get(&g_emergencyMutex, TX_WAIT_FOREVER);
        		g_emergencyBrake = false;
        		tx_mutex_put(&g_emergencyMutex);
            }
            dist_old = range_cm;
        }
        tx_thread_sleep(1);
    }
}
