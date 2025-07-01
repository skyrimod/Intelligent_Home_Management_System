//
// Created by 73227 on 2025/7/1.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_MQ2_TASK_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_MQ2_TASK_H

#include "mq2.h"
#include "tasks_init.h"

typedef struct {
    MQ2_DATA_t data;
    HAL_StatusTypeDef status;
} MQ2_SensorMessage_t;

void mq2_read_task(void *argument);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_MQ2_TASK_H
