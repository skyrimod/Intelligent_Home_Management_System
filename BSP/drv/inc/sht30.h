//
// Created by 73227 on 2025/7/24.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SHT30_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SHT30_H

#include "sensor_config.h"
#include "i2c.h"

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t i2c_addr;
} SHT30_Config;

void SHT30_Register(void);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SHT30_H
