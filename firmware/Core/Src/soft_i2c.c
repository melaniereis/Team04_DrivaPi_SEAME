#include "soft_i2c.h"

// Short delay to mimic 100kHz speed
static void i2c_delay(void) {
    // 250 is the calculated sweet spot for 100kHz on STM32U5
    for (volatile int i = 0; i < 250; i++) {
        __NOP();
    }
}

void Soft_I2C_Init(void) {
    // Clocks are enabled by main.c if you generated code from CubeMX
    // but we ensure pins are set high to be safe.
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
}

void Soft_I2C_Start(void) {
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    i2c_delay();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
    i2c_delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
}

void Soft_I2C_Stop(void) {
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
    i2c_delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    i2c_delay();
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
    i2c_delay();
}

uint8_t Soft_I2C_WriteByte(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & 0x80)
        	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);
        else
        	HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET);
        byte <<= 1;
        i2c_delay();
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET); // Clock High
        i2c_delay();
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET); // Clock Low
        i2c_delay();
    }
    // ACK (Clock pulse for ACK, ignoring value)
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET); // Release SDA1
    i2c_delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    i2c_delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);

    // Read SDA. If LOW, Slave is ACK-ing. If HIGH, Slave is ignoring us.
    uint8_t ack = (HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN) == GPIO_PIN_RESET) ? 1 : 0;

    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET); // Clock Low
    i2c_delay();

    return ack; // Return 1 for Success, 0 for Failure
}

uint8_t Soft_I2C_ReadByte(uint8_t ack) {
    uint8_t byte = 0;
    HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET); // Release SDA
    for (uint8_t i = 0; i < 8; i++) {
        byte <<= 1;
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
        i2c_delay();
        if (HAL_GPIO_ReadPin(SDA_PORT, SDA_PIN)) byte |= 1;
        HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
        i2c_delay();
    }
    // Send ACK/NACK
    if (ack) HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_RESET); // ACK
    else     HAL_GPIO_WritePin(SDA_PORT, SDA_PIN, GPIO_PIN_SET);   // NACK
    i2c_delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_SET);
    i2c_delay();
    HAL_GPIO_WritePin(SCL_PORT, SCL_PIN, GPIO_PIN_RESET);
    return byte;
}


