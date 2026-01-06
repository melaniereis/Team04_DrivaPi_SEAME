/* tests/unit/speed_sensor/test/support/stm32u5xx_hal_conf.h */
#ifndef STM32U5xx_HAL_CONF_H
#define STM32U5xx_HAL_CONF_H

/* 1. Essential Common Definitions */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED

/* 2. The Modules causing your errors */
/* Enabling these allows stm32u5xx_hal.h to include the right headers,
   defining I2C_HandleTypeDef and FDCAN_HandleTypeDef */
#define HAL_I2C_MODULE_ENABLED
#define HAL_FDCAN_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED

/* 3. Oscillator Values (Required by HAL, copy from your real main.h or conf) */
#if !defined  (HSE_VALUE)
  #define HSE_VALUE    (16000000U)
#endif
#if !defined  (HSI_VALUE)
  #define HSI_VALUE    (16000000U)
#endif
#if !defined  (LSI_VALUE)
  #define LSI_VALUE  (32000U)
#endif
#if !defined  (LSE_VALUE)
  #define LSE_VALUE  (32768U)
#endif

/* 4. Tick config */
#define  TICK_INT_PRIORITY            ((uint32_t)0x0F)

#endif // STM32U5xx_HAL_CONF_H
