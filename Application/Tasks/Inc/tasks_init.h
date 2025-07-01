//
// Created by 73227 on 2025/7/1.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_TASKS_INIT_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_TASKS_INIT_H

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

extern QueueHandle_t dht11SensorQueue;
extern QueueHandle_t mq2SensorQueue;

void my_tasks_init(void );

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_TASKS_INIT_H
