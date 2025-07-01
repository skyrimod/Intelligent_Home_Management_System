//
// Created by 73227 on 2025/7/1.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_TASK_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_TASK_H

#include "dht11.h"
#include "tasks_init.h"

typedef struct {
    dht11_data_t data;
    uint32_t timestamp;
    HAL_StatusTypeDef status;
} DHT11_SensorMessage_t;

void dht11_read_task(void *argument);
#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DHT11_TASK_H
