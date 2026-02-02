/**
  ******************************************************************************
  * @file           : sensors.c
  * @brief          : Sensor threads management and drivers
  * @details        : Each sensor runs in its own thread to collect telemetry
  *                   data for the Qt HMI application
  *                   Includes HTS221 driver implementation
  ******************************************************************************
  */

#include "sensors.h"
#include "app_threadx.h"
#include <string.h>

/* External I2C handle */
extern I2C_HandleTypeDef hi2c2;

/* Global sensor data */
TX_MUTEX g_sensorDataMutex;
HTS221_Data_t g_hts221_data;
Battery_Data_t g_battery_data;

/* ============================================================================
 * HTS221 Driver Implementation
 * ============================================================================ */

/* HTS221 Calibration data */
typedef struct {
    int16_t H0_T0_out;
    int16_t H1_T0_out;
    int16_t T0_out;
    int16_t T1_out;
    uint16_t H0_rh;
    uint16_t H1_rh;
    uint16_t T0_degC;
    uint16_t T1_degC;
} HTS221_Calibration_t;

static HTS221_Calibration_t calib_data;

static HAL_StatusTypeDef HTS221_ReadReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data)
{
    uint16_t addr = HTS221_I2C_ADDRESS << 1;
    uint8_t reg_addr = reg | 0x80;
    return HAL_I2C_Mem_Read(hi2c, addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

static HAL_StatusTypeDef HTS221_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data)
{
    uint16_t addr = HTS221_I2C_ADDRESS << 1;
    return HAL_I2C_Mem_Write(hi2c, addr, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

static HAL_StatusTypeDef HTS221_ReadCalibration(I2C_HandleTypeDef *hi2c)
{
    uint8_t t0_msb, t1_msb;
    uint8_t buffer[16];
    uint16_t addr = HTS221_I2C_ADDRESS << 1;
    
    if (HAL_I2C_Mem_Read(hi2c, addr, 0x30 | 0x80, I2C_MEMADD_SIZE_8BIT, buffer, 16, 100) != HAL_OK)
        return HAL_ERROR;
    
    calib_data.H0_rh = buffer[0] >> 1;
    calib_data.H1_rh = buffer[1] >> 1;
    calib_data.T0_degC = buffer[2];
    calib_data.T1_degC = buffer[3];
    t0_msb = (buffer[5] & 0x03);
    t1_msb = (buffer[5] & 0x0C) >> 2;
    calib_data.T0_degC = ((t0_msb << 8) | calib_data.T0_degC) >> 3;
    calib_data.T1_degC = ((t1_msb << 8) | calib_data.T1_degC) >> 3;
    calib_data.H0_T0_out = (int16_t)(buffer[6] | (buffer[7] << 8));
    calib_data.H1_T0_out = (int16_t)(buffer[8] | (buffer[9] << 8));
    calib_data.T0_out = (int16_t)(buffer[12] | (buffer[13] << 8));
    calib_data.T1_out = (int16_t)(buffer[14] | (buffer[15] << 8));
    return HAL_OK;
}

HAL_StatusTypeDef HTS221_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t who_am_i;
    
    if (HTS221_ReadReg(hi2c, HTS221_WHO_AM_I, &who_am_i) != HAL_OK)
    {
        UartPrint("HTS221: Failed to read WHO_AM_I\r\n");
        return HAL_ERROR;
    }
    if (who_am_i != HTS221_WHO_AM_I_VALUE)
    {
        UartPrintf("HTS221: Wrong WHO_AM_I (0x%02X, expected 0x%02X)\r\n", who_am_i, HTS221_WHO_AM_I_VALUE);
        return HAL_ERROR;
    }
    
    if (HTS221_WriteReg(hi2c, HTS221_CTRL_REG1, 0x85) != HAL_OK)
    {
        UartPrint("HTS221: Failed to write CTRL_REG1\r\n");
        return HAL_ERROR;
    }
    if (HTS221_ReadCalibration(hi2c) != HAL_OK)
    {
        UartPrint("HTS221: Failed to read calibration\r\n");
        return HAL_ERROR;
    }
    
    UartPrint("HTS221: Initialized successfully\r\n");
    SoftwareDelay(500);
    return HAL_OK;
}

HAL_StatusTypeDef HTS221_ReadBoth(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity)
{
    uint8_t h_low, t_low, t_high;
    int16_t h_raw, t_raw;
    uint16_t addr = HTS221_I2C_ADDRESS << 1;
    
    if (HAL_I2C_Mem_Read(hi2c, addr, HTS221_HUMIDITY_OUT_L, I2C_MEMADD_SIZE_8BIT, &h_low, 1, 100) != HAL_OK)
        return HAL_ERROR; 
    if (HAL_I2C_Mem_Read(hi2c, addr, HTS221_TEMP_OUT_L, I2C_MEMADD_SIZE_8BIT, &t_low, 1, 100) != HAL_OK)
        return HAL_ERROR;
    if (HAL_I2C_Mem_Read(hi2c, addr, HTS221_TEMP_OUT_H, I2C_MEMADD_SIZE_8BIT, &t_high, 1, 100) != HAL_OK)
        return HAL_ERROR;

    h_raw = (int8_t)h_low;
    t_raw = (int16_t)(t_low | (t_high << 8));
    
    float t_temp = (float)(t_raw - calib_data.T0_out) * (float)(calib_data.T1_degC - calib_data.T0_degC) / 
                   (float)(calib_data.T1_out - calib_data.T0_out) + calib_data.T0_degC;
    float h_temp = ((float)(h_raw - calib_data.H0_T0_out)) * ((float)(calib_data.H1_rh - calib_data.H0_rh)) / 
                   ((float)(calib_data.H1_T0_out - calib_data.H0_T0_out)) + (float)calib_data.H0_rh;
    if (h_temp < 0.0f) h_temp = 0.0f;
    if (h_temp > 100.0f) h_temp = 100.0f;
    *temperature = t_temp;
    *humidity = h_temp;
    return HAL_OK;
}
/* ============================================================================
 * @brief  HTS221 Temperature/Humidity sensor thread
 * @note   Reads temperature and humidity every 10 second
 * @param  initial_input: Thread input parameter (unused)
 */
void SensorHTS221Thread(ULONG initial_input)
{
    float temp, hum;
    HAL_StatusTypeDef status;
    uint32_t read_count = 0;
    uint32_t can_send_count = 0;
    (void)initial_input;
    UartPrint("HTS221 Thread: Started\r\n");
    tx_thread_sleep(100);
    
    while (1)
    {
        status = HTS221_ReadBoth(&hi2c2, &temp, &hum);   
        if (status == HAL_OK)
        {
            if (tx_mutex_get(&g_sensorDataMutex, TX_WAIT_FOREVER) == TX_SUCCESS)
            {
                g_hts221_data.temperature = temp;
                g_hts221_data.humidity = hum;
                g_hts221_data.timestamp = tx_time_get();
                g_hts221_data.data_valid = 1;
                tx_mutex_put(&g_sensorDataMutex);
            }
            read_count++;
            can_send_count++;

            /* Send via CAN every 5 readings (5 seconds) to avoid saturation */
            if (can_send_count >= 5)
            {
                t_can_message msg;
                
                msg.id = CAN_ID_HTS221_TEMPERATURE;
                msg.len = 4;
                memcpy(msg.data, &temp, 4);
                CanSend(&msg);

                msg.id = CAN_ID_HTS221_HUMIDITY;
                msg.len = 4;
                memcpy(msg.data, &hum, 4);
                CanSend(&msg);
                
                can_send_count = 0;
            }
            
            if (read_count % 10 == 0)
            {
                UartPrintf("HTS221: T=%.2f°C, H=%.1f%%, Count=%lu\r\n", 
                           temp, hum, read_count);
            }
        }
        else
        {
            UartPrint("HTS221 Thread: Read error\r\n");
            if (tx_mutex_get(&g_sensorDataMutex, TX_WAIT_FOREVER) == TX_SUCCESS)
            {
                g_hts221_data.data_valid = 0;
                tx_mutex_put(&g_sensorDataMutex);
            }
        }
        tx_thread_sleep(100);
    }
}

/**
 * @brief  Initialize sensor mutex and data structures
 * @note   Call this before ThreadInit() to prepare sensor resources
 */
void SensorsInit(void)
{
    tx_mutex_create(&g_sensorDataMutex, "Sensor Data Mutex", TX_NO_INHERIT);
    memset(&g_hts221_data, 0, sizeof(HTS221_Data_t));
    memset(&g_battery_data, 0, sizeof(Battery_Data_t));
    UartPrint("Sensors: Mutex and data structures initialized\r\n");
}

/* ============================================================================
 * Battery Monitor Driver Implementation
 * ============================================================================ */

/**
 * @brief  Initialize Battery Monitor
 * @param  hi2c: I2C handle
 * @retval HAL status
 */
HAL_StatusTypeDef Battery_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t buffer[2];
    uint16_t config_reg;
    HAL_StatusTypeDef status;
    uint16_t dev_addr = BATTERY_I2C_ADDRESS << 1;
    
    UartPrint("Battery: Initializing INA219 Power Monitor at 0x41...\r\n");
    status = HAL_I2C_Mem_Read(hi2c, dev_addr, INA219_REG_CONFIG, 
                               I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
    
    if (status != HAL_OK)
    {
        UartPrint("Battery: ERROR - Failed to communicate with INA219\r\n");
        return HAL_ERROR;
    }
    
    config_reg = (buffer[0] << 8) | buffer[1];
    UartPrintf("Battery: INA219 Config = 0x%04X\r\n", config_reg);
    
    /* Read a test voltage to verify it's working */
    status = HAL_I2C_Mem_Read(hi2c, dev_addr, INA219_REG_BUS_V, 
                               I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
    
    if (status == HAL_OK)
    {
        uint16_t bus_voltage_raw = (buffer[0] << 8) | buffer[1];
        uint16_t voltage_bits = (bus_voltage_raw >> 3) & 0x1FFF;
        float voltage = voltage_bits * 0.004f;
        
        UartPrintf("Battery: INA219 initialized successfully! Current voltage: %.3fV\r\n", voltage);
        
        if (voltage < 9.0f || voltage > 13.0f)
        {
            UartPrintf("Battery: WARNING - Voltage %.3fV outside 3S range (9.0-12.6V)\r\n", voltage);
        }
        
        return HAL_OK;
    }
    
    UartPrint("Battery: ERROR - Failed to read voltage from INA219\r\n");
    return HAL_ERROR;
}

/**
 * @brief  Read battery voltage and percentage
 * @param  hi2c: I2C handle
 * @param  voltage: Pointer to store voltage value
 * @param  percentage: Pointer to store percentage value
 * @retval HAL status
 */
HAL_StatusTypeDef Battery_Read(I2C_HandleTypeDef *hi2c, float *voltage, uint8_t *percentage)
{
    uint8_t buffer[2];
    HAL_StatusTypeDef status;
    uint16_t dev_addr = BATTERY_I2C_ADDRESS << 1;
    
    if (voltage == NULL || percentage == NULL)
    {
        return HAL_ERROR;
    }
    
    status = HAL_I2C_Mem_Read(hi2c, dev_addr, INA219_REG_BUS_V, 
                               I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
    
    if (status != HAL_OK)
    {
        *voltage = 0.0f;
        *percentage = 0;
        return HAL_ERROR;
    }
    
    /* Decode INA219 Bus Voltage Register
     * Bit 15-3: Voltage value (LSB = 4mV)
     * Bit 2: Reserved
     * Bit 1: CNVR (Conversion Ready) - 1 when conversion complete
     * Bit 0: OVF (Math Overflow) - 1 on overflow
     */
    uint16_t bus_voltage_raw = (buffer[0] << 8) | buffer[1];
    
    if (bus_voltage_raw & INA219_BUS_V_OVF)
    {
        UartPrint("Battery: WARNING - INA219 math overflow detected\r\n");
    }
    uint16_t voltage_bits = (bus_voltage_raw >> 3) & 0x1FFF;
    *voltage = voltage_bits * 0.004f;

    if (*voltage >= BATTERY_3S_MAX_V)
    {
        *percentage = 100;
    }
    else if (*voltage <= BATTERY_3S_MIN_V)
    {
        *percentage = 0;
    }
    else if (*voltage >= BATTERY_3S_NOM_V)
    {
        float pct = 50.0f + ((*voltage - BATTERY_3S_NOM_V) / (BATTERY_3S_MAX_V - BATTERY_3S_NOM_V)) * 50.0f;
        *percentage = (uint8_t)pct;
    }
    else
    {
        float pct = ((*voltage - BATTERY_3S_MIN_V) / (BATTERY_3S_NOM_V - BATTERY_3S_MIN_V)) * 50.0f;
        *percentage = (uint8_t)pct;
    }
    
    return HAL_OK;
}

/* ============================================================================
 * Battery Sensor Thread
 * ============================================================================ */

/**
 * @brief  Battery monitor sensor thread
 * @note   Reads battery status every 5 seconds
 * @param  initial_input: Thread input parameter (unused)
 */
void SensorBatteryThread(ULONG initial_input)
{
	return ;
    float voltage;
    uint8_t percentage;
    HAL_StatusTypeDef status;
    uint32_t read_count = 0;
    uint32_t error_count = 0;
    uint8_t low_battery_warned = 0;
    
    (void)initial_input;
    
    UartPrint("Battery Thread: Started\r\n");
    tx_thread_sleep(100);
    
    while (1)
    {
        status = Battery_Read(&hi2c3, &voltage, &percentage);
        
        if (status == HAL_OK)
        {
            error_count = 0;
            if (tx_mutex_get(&g_sensorDataMutex, TX_WAIT_FOREVER) == TX_SUCCESS)
            {
                g_battery_data.voltage = voltage;
                g_battery_data.percentage = percentage;
                g_battery_data.timestamp = tx_time_get();
                g_battery_data.data_valid = 1;
                
                tx_mutex_put(&g_sensorDataMutex);
            }
            read_count++;
            
            t_can_message msg;
            

            msg.id = CAN_ID_BATTERY_VOLTAGE;
            msg.len = 4;
            memcpy(msg.data, &voltage, 4);
            CanSend(&msg);
            msg.id = CAN_ID_BATTERY_PERCENTAGE;
            msg.len = 1;
            msg.data[0] = percentage;
            CanSend(&msg);

            if (voltage < BATTERY_3S_LOW_V && !low_battery_warned)
            {
                UartPrintf("Battery: *** LOW BATTERY WARNING *** V=%.2fV < %.1fV\r\n", 
                           voltage, BATTERY_3S_LOW_V);
                low_battery_warned = 1;
            }
            else if (voltage >= BATTERY_3S_LOW_V)
            {
                low_battery_warned = 0;
            }
            UartPrintf("Battery: %.2fV (%d%%) - CAN sent [%lu]\r\n", 
                       voltage, percentage, read_count);
        }
        else
        {
            error_count++;
            UartPrintf("Battery: Read error #%lu (status=%d)\r\n", error_count, status);
            if (error_count >= 3)
            {
                if (tx_mutex_get(&g_sensorDataMutex, TX_WAIT_FOREVER) == TX_SUCCESS)
                {
                    g_battery_data.data_valid = 0;
                    tx_mutex_put(&g_sensorDataMutex);
                }
            }
        }
        tx_thread_sleep(500);
    }
}
