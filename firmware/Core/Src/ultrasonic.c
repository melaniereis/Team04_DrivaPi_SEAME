#include "soft_i2c.h"
#include "app_threadx.h"

// SRF08 Commands and Addresses
#define SRF08_ADDR          0xE0
#define CMD_REG             0x00
#define RANGE_REG           0x02
#define CMD_CENTIMETERS     0x51

// Safety Thresholds
#define BRAKE_THRESHOLD_CM  20  // STOP if object is closer than 20cm
#define WARNING_THRESHOLD_CM 50 // Warn if object is closer than 50cm
#define SENSOR_OFFSET	0

// External reference to the Event Flags we created in thread_init.c
extern TX_EVENT_FLAGS_GROUP system_events;

/**
 * @brief  Ultrasonic Sensor Thread Entry
 * Priority: 3 (High)
 */
void UltrasonicEntry(ULONG initial_input)
{
	// --- 2. PARANOID CONFIGURATION ---
	    // We loop until the sensor confirms it has the correct settings.
	    uint8_t verify_range = 0;
	    int config_attempts = 0;

	    while (verify_range != 0x46 && config_attempts < 10)
	    {
	        config_attempts++;

	        // A. WRITE CONFIG
	        Soft_I2C_Start();
	        if (Soft_I2C_WriteByte(SRF08_ADDR) == 1) { // If ACK
	            Soft_I2C_WriteByte(0x02); // Range Register
	            Soft_I2C_WriteByte(0x46); // Target: 0x46 (3 meters)
	        }
	        Soft_I2C_Stop();

	        tx_thread_sleep(5); // Give it time to save

	        // B. VERIFY (READ BACK)
	        Soft_I2C_Start();
	        if (Soft_I2C_WriteByte(SRF08_ADDR) == 1) { // If ACK
	            Soft_I2C_WriteByte(0x02); // Point to Range Register
	            Soft_I2C_Stop();

	            Soft_I2C_Start(); // Restart
	            Soft_I2C_WriteByte(SRF08_ADDR | 1); // Read Mode
	            verify_range = Soft_I2C_ReadByte(0); // Read with NACK
	        }
	        Soft_I2C_Stop();

	        // Debug Print
	        char debug_msg[64];
	        snprintf(debug_msg, sizeof(debug_msg), "Config Try %d: Read 0x%02X\r\n", config_attempts, verify_range);
	        HAL_UART_Transmit(&huart1, (uint8_t*)debug_msg, strlen(debug_msg), 10);

	        tx_thread_sleep(5);
	    }

	    if (verify_range != 0x46) {
	        HAL_UART_Transmit(&huart1, (uint8_t*)"SENSOR CONFIG FAILED!\r\n", 23, 100);
	        // We continue anyway, but expect issues.
	    } else {
	        HAL_UART_Transmit(&huart1, (uint8_t*)"SENSOR CONFIG OK.\r\n", 19, 100);
	    }

    uint8_t high_byte, low_byte;
    int16_t range_cm = 0;
    int16_t safe_dist = 0;

    // --- DEBUG: VERIFY REGISTERS ---
        uint8_t current_range, current_gain;

        Soft_I2C_Start();
        Soft_I2C_WriteByte(SRF08_ADDR);
        Soft_I2C_WriteByte(0x02); // Point to Range Register
        Soft_I2C_Stop();

        Soft_I2C_Start();
        Soft_I2C_WriteByte(SRF08_ADDR | 1);
        current_range = Soft_I2C_ReadByte(0); // Read Range
        Soft_I2C_Stop();

        char debug_msg[64];
        snprintf(debug_msg, sizeof(debug_msg), "REG CHECK: Range set to 0x%02X\r\n", current_range);
        HAL_UART_Transmit(&huart1, (uint8_t*)debug_msg, strlen(debug_msg), 100);

    while(1)
    {
        // --- STEP 1: PING ---
        Soft_I2C_Start();
        if (Soft_I2C_WriteByte(SRF08_ADDR) == 0) {
            // Bus busy or NACK. Quick retry.
            Soft_I2C_Stop();
            tx_thread_sleep(1);
            continue;
        }
        Soft_I2C_WriteByte(CMD_REG);
        Soft_I2C_WriteByte(CMD_CENTIMETERS);
        Soft_I2C_Stop();

        // --- STEP 2: WAIT (THE SPEED FIX) ---
        // Range is now limited to 2m (14ms physics time).
        // We wait slightly longer to be safe.
        // Try '3' first. If you get garbage/zeros, increase to '4' or '5'.
        tx_thread_sleep(6);

        // --- STEP 3: READ ---
        Soft_I2C_Start();
        if (Soft_I2C_WriteByte(SRF08_ADDR) == 0) {
            Soft_I2C_Stop();
            continue;
        }
        Soft_I2C_WriteByte(RANGE_REG);
        Soft_I2C_Stop();

        Soft_I2C_Start();
        Soft_I2C_WriteByte(SRF08_ADDR | 1);
        high_byte = Soft_I2C_ReadByte(1);
        low_byte  = Soft_I2C_ReadByte(0);
        Soft_I2C_Stop();

        range_cm = (high_byte << 8) | low_byte;

        // --- FILTERING ---
        if (range_cm > 0 && range_cm < 300) {
            safe_dist = range_cm;

            // Send to UART (Comment this out later for max speed!)
            // Printing text takes time.
            char msg[32];
            snprintf(msg, sizeof(msg), "Dist: %i cm\r\n", safe_dist);
            HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), 10);
        }

        // --- BRAKE TRIGGER ---
        if (safe_dist > 0 && safe_dist < BRAKE_THRESHOLD_CM) {
             tx_event_flags_set(&system_events, 0x01, TX_OR);
        }

        // Minimal delay between pings
        tx_thread_sleep(1);
    }
}

