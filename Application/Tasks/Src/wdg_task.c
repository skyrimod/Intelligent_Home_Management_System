//
// Created by 73227 on 2025/7/15.
//

#include "wdg_task.h"

// 软件看门狗数据结构
typedef struct {
    TaskHandle_t handle;
    const char *name;
    TickType_t lastExecTime;
    uint32_t minStack;
} TaskMonitorInfo_t;

// 系统状态监控结构
typedef struct {
    uint32_t freeHeap;
    uint32_t minHeapEver;
    uint32_t taskCount;
    uint8_t faultFlags;
} SystemStatus_t;

static TaskMonitorInfo_t monitorInfo[] = {
        {NULL, "dht11ReadTask", 0, UINT32_MAX},
        {NULL, "lcdShowTask", 0, UINT32_MAX},
        {NULL, "swdgTask", 0, UINT32_MAX}
};

static SystemStatus_t systemStatus = {0};

char TaskRunTimeBuf[200];

void sw_wdg_task(void *argument){
    const TickType_t xMonitorInterval = pdMS_TO_TICKS(1000);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t taskFailure = 0;

    // 获取任务句柄
    for (int i = 0; i < sizeof(monitorInfo)/sizeof(TaskMonitorInfo_t); ++i) {
        monitorInfo[i].handle = xTaskGetHandle(monitorInfo[i].name);
    }

    for (;;) {
        osDelay(xMonitorInterval);
        // 重置状态
        taskFailure = 0;
        systemStatus.faultFlags = 0;
        // 更新系统状态
        systemStatus.freeHeap = xPortGetFreeHeapSize();
        systemStatus.minHeapEver = xPortGetMinimumEverFreeHeapSize();
        systemStatus.taskCount = uxTaskGetNumberOfTasks();

        // 任务运行时间与CPU利用率
        vTaskGetRunTimeStats(TaskRunTimeBuf);
        log_i("%s", TaskRunTimeBuf);

        // 检测每个任务的状态
        for (int i = 0; i < sizeof(monitorInfo) / sizeof(TaskMonitorInfo_t); ++i) {
            eTaskState state = eTaskGetState(monitorInfo[i].handle);

            // 任务未运行
            if (state == eSuspended || state == eDeleted || state == eInvalid) {
                log_e("Task %s in bad state: %d", monitorInfo[i].name, state);
                taskFailure = 1;
                systemStatus.faultFlags |= 1 << i;
            }

            // 检测执行进度
//            TickType_t currentTime = xTaskGetTickCount();
//            if ((currentTime - monitorInfo[i].lastExecTime) > MAX_TASK_INTERVAL){
//                log_w("Task %s delay detected,", monitorInfo[i].name);
//            }
//            monitorInfo[i].lastExecTime = currentTime;

            // 检测堆栈使用
            UBaseType_t stackMark = uxTaskGetStackHighWaterMark(monitorInfo[i].handle);
            if (stackMark < monitorInfo[i].minStack) {
                monitorInfo[i].minStack = stackMark;
            }

            if (stackMark < 100) {
                log_w("Task %s stack near overflow! Free: %lu", monitorInfo[i].name, stackMark);
            }

            // 检测内存状态
            if (systemStatus.freeHeap < 1024 || systemStatus.minHeapEver < 1024) {
                log_w("Memory log: Free=%u MinEver=%u", systemStatus.freeHeap, systemStatus.minHeapEver);
//                taskFailure = 1;
//                systemStatus.faultFlags |= MEMORY_FAULT_FLAG;
            }

            // 处理故障
            if (taskFailure) {
                // while(1) 不喂狗，触发服务
            } else {
                // 系统健康
            }
        }
    }
}

void hw_wdg_task(void *argument){
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
