#ifndef STM32U5XX_HAL_H
#define STM32U5XX_HAL_H

#include <stdint.h>

/* 1. Register Definitions */
typedef struct {
    uint32_t CR1;
    uint32_t CNT;  // Needed for speed calculation
    uint32_t ARR;
    uint32_t PSC;
} TIM_TypeDef;

/* 2. Handle Types (Satisfying main.h and others) */
typedef struct {
    TIM_TypeDef *Instance;
} TIM_HandleTypeDef;

typedef uint32_t UART_HandleTypeDef;
typedef uint32_t I2C_HandleTypeDef;       // Added for main.h
typedef uint32_t FDCAN_HandleTypeDef;     // Added for main.h
typedef uint32_t FDCAN_TxHeaderTypeDef;   // Added for main.h
typedef uint32_t HAL_StatusTypeDef;

/* 3. Status Constants */
#define HAL_OK      0x00U
#define HAL_ERROR   0x01U
#define HAL_BUSY    0x02U
#define HAL_TIMEOUT 0x03U
#ifndef __HAL_RCC_TIM1_CLK_ENABLE
#define __HAL_RCC_TIM1_CLK_ENABLE() (void)0
#endif

/* 4. Function Prototypes for CMock */
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif // STM32U5XX_HAL_H