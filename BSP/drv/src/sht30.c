//
// Created by 73227 on 2025/7/24.
//
#include <stdbool.h>
#include "sht30.h"
#include "dwt_delay.h"
#include "elog.h"

/**
 * @brief       写命令到SHT30
 * @param       cmd: 16位命令
 * @retval      状态: 0成功, 非0失败
 */
static uint8_t SHT30_WriteCommand(uint16_t cmd)
{
    uint8_t data[2];

    data[0] = cmd >> 8;        // 命令高字节
    data[1] = cmd & 0xFF;      // 命令低字节

    iic_start();                           // 启动IIC通信
    iic_send_byte(SHT30_ADDR << 1);        // 发送设备地址+写
    if(iic_wait_ack()) {                   // 等待ACK
        iic_stop();
        return 1;  // 未收到ACK
    }

    iic_send_byte(data[0]);                // 发送命令高字节
    if(iic_wait_ack()) {
        iic_stop();
        return 2;  // 发送高字节失败
    }

    iic_send_byte(data[1]);                // 发送命令低字节
    if(iic_wait_ack()) {
        iic_stop();
        return 3;  // 发送低字节失败
    }

    iic_stop();                            // 停止IIC通信

    return 0;  // 发送成功
}

/**
 * @brief       读取6字节数据（温度+湿度+CRC）
 * @param       dat: 接收数据缓冲区(6字节)
 * @retval      状态: 0成功, 非0失败
 */
static uint8_t SHT30_ReadData(uint8_t *dat)
{
    iic_start();                           // 启动IIC通信
    iic_send_byte((SHT30_ADDR << 1) | 1);  // 发送设备地址+读
    if(iic_wait_ack()) {                   // 等待ACK
        iic_stop();
        return 1;  // 未收到ACK
    }

    // 读取前5个字节（每字节后发送ACK）
    for(uint8_t i = 0; i < 5; i++) {
        dat[i] = iic_read_byte(1);         // 发送ACK继续读取
    }

    // 读取最后一个字节（发送NACK停止）
    dat[5] = iic_read_byte(0);             // 发送NACK停止读取

    iic_stop();                            // 停止IIC通信

    return 0;  // 读取成功
}


static SensorStatus SHT30_Init(void *config){
    SHT30_Config *cfg = (SHT30_Config*)config;

    // 1. 发送软复位命令
    if(SHT30_WriteCommand(SHT30_RESET_CMD) != 0) {
        return SENSOR_ERR_COMM_FAIL;  // 复位失败
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

    // 发送测量命令
    if(SHT30_WriteCommand(SHT30_MEASURE_CMD) != 0) {
        return SENSOR_ERR_COMM_FAIL;  // 发送测量命令失败
    }

    delay_ms(18);

    if(SHT30_ReadData(rx_data) != 0) {
        return SENSOR_ERR_COMM_FAIL;  // 读取数据失败
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


