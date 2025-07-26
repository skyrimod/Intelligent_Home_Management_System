//
// Created by 73227 on 2025/7/24.
//
#include <stdbool.h>
#include "sht30.h"
#include "dwt_delay.h"

static SensorStatus SHT30_Init(void *config){
    SHT30_Config *cfg = (SHT30_Config*)config;

    // 检查I2C句柄是否有效
    if (cfg->hi2c == NULL || cfg->hi2c->State != HAL_I2C_STATE_READY){
        return SENSOR_ERR_INVALID_PARAM;
    }

    //SHT30软件复位
    uint8_t reset_cmd[2] = {0x30, 0xA2};
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(cfg->hi2c, cfg->i2c_addr <<1,
                                                       reset_cmd, sizeof (reset_cmd), HAL_MAX_DELAY);

    if (status != HAL_OK){
        return SENSOR_ERR_COMM_FAIL;
    }

    delay_ms(10);
    return SENSOR_OK;
}

static bool sht30_check_crc(uint8_t *data, uint8_t len, uint8_t checksum){
    uint8_t crc = 0xFF;

    for (int i = len; i; --i) {
        crc ^= *data++;
        for (int j = 8; j; --j) {
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
    }

    return crc == checksum;
}

static SensorStatus SHT30_Read(void *config, float *value, SensorValueType valueType){
    SHT30_Config *cfg = (SHT30_Config*)config;
    uint8_t rx_data[6];
    uint8_t cmd[2] = {0x2C, 0x06};

    // 发送测量命令
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(cfg->hi2c, cfg->i2c_addr << 1,
                                                       cmd, sizeof(cmd), HAL_MAX_DELAY);

    if (status != HAL_OK){
        return SENSOR_ERR_COMM_FAIL;
    }

    delay_ms(15);

    status = HAL_I2C_Master_Receive(cfg->hi2c, cfg->i2c_addr << 1,
                                    rx_data, sizeof(rx_data), HAL_MAX_DELAY);

    if (status != HAL_OK){
        return SENSOR_ERR_COMM_FAIL;
    }

    // CRC校验
    if (!sht30_check_crc(&rx_data[0], 2, rx_data[2]) ||
        !sht30_check_crc(&rx_data[3], 2, rx_data[5])){
        return SENSOR_ERR_DATA_CORRUPTED;
    }

    // 数据处理
    uint16_t temp_raw = (rx_data[0] << 8) | rx_data[1];
    uint16_t humi_raw = (rx_data[3] << 8) | rx_data[4];

    // 数值转换
    if (valueType == SENSOR_TEMP){
        *value = -45.0f + 175.0f * (temp_raw / 65535.0f);
    } else{
        *value = 100.0f * (humi_raw / 65535.0f);
    }
    return SENSOR_OK;
}

static SensorStatus SHT30_Deinit(void *config){
    return SENSOR_OK;
}

void SHT30_Register(void){
    Sensor_RegisterDriver(SENSOR_SHT30, SHT30_Init, SHT30_Read, SHT30_Deinit);
}


