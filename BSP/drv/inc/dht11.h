//
// Created by 73227 on 2025/6/29.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_H

#include "stm32f1xx.h"

typedef struct {
    uint8_t temperature;
    uint8_t humidity;
} dht11_data_t;

/* 引脚配置 */
#define DHT11_DATA_PORT             GPIOG
#define DHT11_DATA_PIN              GPIO_PIN_11
#define DHT11_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

#define DHT11_DQ_OUT(x)           do{ x? \
                                        HAL_GPIO_WritePin(DHT11_DATA_PORT, DHT11_DATA_PIN, GPIO_PIN_SET): \
                                        HAL_GPIO_WritePin(DHT11_DATA_PORT, DHT11_DATA_PIN, GPIO_PIN_RESET);}while(0)

#define DHT11_DQ_IN         HAL_GPIO_ReadPin(DHT11_DATA_PORT, DHT11_DATA_PIN)  /* 数据端口输入 */


uint8_t dht11_init(void);   /* 初始化DHT11 */
uint8_t dht11_check(void);  /* 检测是否存在DHT11 */
uint8_t dht11_read_data(dht11_data_t *data);   /* 读取温湿度 */


#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_H
