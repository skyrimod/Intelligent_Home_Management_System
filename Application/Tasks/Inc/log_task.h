//
// Created by 73227 on 2025/7/9.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_LOG_TASK_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_LOG_TASK_H

#include "uart.h"
#include "elog.h"
#include "tasks_manager.h"

typedef struct {
    uint8_t *data;
    uint16_t len;
} LogMessage;

void log_init(void );
void log_task(void *argument);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_LOG_TASK_H
