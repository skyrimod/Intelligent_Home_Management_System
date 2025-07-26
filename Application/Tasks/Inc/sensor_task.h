//
// Created by 73227 on 2025/7/26.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SENSOR_TASK_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SENSOR_TASK_H

#include "tasks_manager.h"
#include "wdg_task.h"

typedef struct {
    float temperature;
    float humidity;
} SensorMsg;

void sensor_task(void *argument);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SENSOR_TASK_H
