//
// Created by 73227 on 2025/7/24.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SHT30_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SHT30_H

#include "sensor_config.h"
#include "myiic.h"

/* SHT30设备地址 (ADDR接地=0x44, ADDR接VCC=0x45) */
#define SHT30_ADDR        0x44

/* SHT30命令定义 */
#define SHT30_RESET_CMD         0x30A2
#define SHT30_READ_STATUS_CMD   0xF32D
#define SHT30_CLEAR_STATUS_CMD  0x3041
#define SHT30_MEASURE_HIGH_CMD  0x2400  // 高精度测量
#define SHT30_MEASURE_MED_CMD   0x240B  // 中精度测量
#define SHT30_MEASURE_LOW_CMD   0x2416  // 低精度测量

// 使用高精度模式
#define SHT30_MEASURE_CMD       SHT30_MEASURE_HIGH_CMD

typedef struct {
    uint8_t i2c_addr;
} SHT30_Config;

void SHT30_Register(void);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SHT30_H
