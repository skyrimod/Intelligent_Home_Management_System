//
// Created by 73227 on 2025/7/1.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_TASKS_MANAGER_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_TASKS_MANAGER_H

#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

extern QueueHandle_t dht11SensorQueue;
extern QueueHandle_t mq2SensorQueue;
extern QueueHandle_t logQueue;
extern QueueHandle_t sensorQueue;

extern EventGroupHandle_t wdgEventGroup;

extern SemaphoreHandle_t dmaSemaphore;
extern SemaphoreHandle_t logSemaphore;

void my_tasks_init(void );

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_TASKS_MANAGER_H
