//
// Created by 73227 on 2025/7/15.
//

#include "wdg_task.h"

void wdg_task(void *argument){
    EventBits_t uxBits;

    iwdg_init();
    iwdg_feed();

    for(;;){
        // 任务信号检测
        uxBits = xEventGroupWaitBits(
                wdgEventGroup,
                ALL_TASKS_BIT,
                pdTRUE,
                pdTRUE,
                WATCHDOG_TIMEOUT_MS
                );

        if ((uxBits & ALL_TASKS_BIT) == ALL_TASKS_BIT){
            // 喂狗
            iwdg_feed();
        }
        osDelay(10);
    }
}
