//
// Created by 73227 on 2025/7/15.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_WDG_TASK_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_WDG_TASK_H

#include "tasks_manager.h"
#include "wdg.h"
#include "elog.h"
#include "task.h"

#define LCD_TASK_BIT (1 << 0)
#define SENSOR_TASK_BIT (1 << 1)
#define ALL_TASKS_BIT   (LCD_TASK_BIT | SENSOR_TASK_BIT)

#define WATCHDOG_TIMEOUT_MS 200

void hw_wdg_task(void *argument);
void sw_wdg_task(void *argument);
#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_WDG_TASK_H
