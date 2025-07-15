//
// Created by 73227 on 2025/7/15.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_WDG_TASK_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_WDG_TASK_H

#include "tasks_manager.h"
#include "wdg.h"

#define LCD_TASK_BIT (1 << 0)
#define DHT11_TASK_BIT  (1 << 1)
#define ALL_TASKS_BIT   (LCD_TASK_BIT | DHT11_TASK_BIT)

#define WATCHDOG_TIMEOUT_MS 200

void wdg_task(void *argument);
#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_WDG_TASK_H
