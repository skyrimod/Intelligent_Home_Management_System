//
// Created by 73227 on 2025/7/26.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_I2C_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_I2C_H

#include "stm32f1xx.h"
#include "main.h"

// SCL
#define I2C_SCL_PROT GPIOB
#define I2C_SCL_PIN GPIO_PIN_6
#define I2C_SCL_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE();}while(0)

// SDA
#define I2C_SDA_PORT GPIOB
#define I2C_SDA_PIN GPIO_PIN_7
#define I2C_SDA_CLK_ENABLE() do{ __HAL_RCC_GPIOB_CLK_ENABLE();}while(0)

#define I2C_CLK_ENABLE() do{__HAL_RCC_I2C1_CLK_ENABLE();}while(0)

extern I2C_HandleTypeDef hi2c;

void i2c_init(void );

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_I2C_H
