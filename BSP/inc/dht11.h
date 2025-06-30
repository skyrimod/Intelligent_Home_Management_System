//
// Created by 73227 on 2025/6/29.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_H

#include "stm32f1xx.h"

typedef struct {
    float temperature;
    float humidity;
} dht11_data_t;

/* 引脚配置 */
#define DHT11_DATA_PORT             GPIOG
#define DHT11_DATA_PIN              GPIO_PIN_11
#define DHT11_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)

#define DHT11_DATA_OUT(x)           do{ x? \
                                        HAL_GPIO_WritePin(DHT11_DATA_PORT, DHT11_DATA_PIN, GPIO_PIN_SET): \
                                        HAL_GPIO_WritePin(DHT11_DATA_PORT, DHT11_DATA_PIN, GPIO_PIN_RESET);}while(0)

/* public */
void dht11_init(void);
void dht11_start(void);
HAL_StatusTypeDef dht11_read_data(dht11_data_t *data);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_H
