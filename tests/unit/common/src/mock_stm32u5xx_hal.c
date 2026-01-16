#include "../mocks/stm32_hal_i2c.h"
#include <string.h>

/* Simple HAL mock used by dc-motor and servo-motor unit tests */

static HAL_StatusTypeDef mock_i2c_write_return = HAL_OK;
static HAL_StatusTypeDef mock_i2c_read_return = HAL_OK;
static uint32_t mock_i2c_write_calls = 0;
static uint32_t mock_i2c_read_calls = 0;
static uint16_t mock_last_i2c_dev_addr = 0;
static uint16_t mock_last_i2c_write_size = 0;
static uint8_t mock_last_i2c_write_data[256];

void HAL_Mock_Reset(void) {
    mock_i2c_write_return = HAL_OK;
    mock_i2c_read_return = HAL_OK;
    mock_i2c_write_calls = 0;
    mock_i2c_read_calls = 0;
    mock_last_i2c_dev_addr = 0;
    mock_last_i2c_write_size = 0;
    memset(mock_last_i2c_write_data, 0, sizeof(mock_last_i2c_write_data));
}

void HAL_Mock_SetI2CWriteReturn(HAL_StatusTypeDef status) {
    mock_i2c_write_return = status;
}

void HAL_Mock_SetI2CReadReturn(HAL_StatusTypeDef status) {
    mock_i2c_read_return = status;
}

uint32_t HAL_Mock_GetI2CWriteCallCount(void) {
    return mock_i2c_write_calls;
}

uint32_t HAL_Mock_GetI2CReadCallCount(void) {
    return mock_i2c_read_calls;
}

uint8_t *HAL_Mock_GetLastI2CWriteData(void) {
    return mock_last_i2c_write_data;
}

uint16_t HAL_Mock_GetLastI2CWriteSize(void) {
    return mock_last_i2c_write_size;
}

uint16_t HAL_Mock_GetLastI2CDevAddress(void) {
    return mock_last_i2c_dev_addr;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c,
                                    uint16_t DevAddress,
                                    uint16_t MemAddress,
                                    uint16_t MemAddSize,
                                    uint8_t *pData,
                                    uint16_t Size,
                                    uint32_t Timeout) {
    (void)hi2c;
    (void)MemAddress;
    (void)MemAddSize;
    (void)Timeout;

    mock_i2c_write_calls++;
    mock_last_i2c_dev_addr = DevAddress;
    mock_last_i2c_write_size = Size;

    uint16_t copy_len = (Size < (uint16_t)sizeof(mock_last_i2c_write_data)) ? Size : (uint16_t)sizeof(mock_last_i2c_write_data);
    if (pData && copy_len > 0) {
        memcpy(mock_last_i2c_write_data, pData, copy_len);
    }

    return mock_i2c_write_return;
}

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c,
                                   uint16_t DevAddress,
                                   uint16_t MemAddress,
                                   uint16_t MemAddSize,
                                   uint8_t *pData,
                                   uint16_t Size,
                                   uint32_t Timeout) {
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddress;
    (void)MemAddSize;
    (void)Timeout;

    mock_i2c_read_calls++;

    if (pData && Size > 0) {
        memset(pData, 0, Size);
    }

    return mock_i2c_read_return;
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
                                          uint16_t DevAddress,
                                          uint8_t *pData,
                                          uint16_t Size,
                                          uint32_t Timeout) {
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    (void)Timeout;
    return HAL_OK;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
                                         uint16_t DevAddress,
                                         uint8_t *pData,
                                         uint16_t Size,
                                         uint32_t Timeout) {
    (void)hi2c;
    (void)DevAddress;
    if (pData && Size > 0) {
        memset(pData, 0, Size);
    }
    (void)Timeout;
    return HAL_OK;
}

