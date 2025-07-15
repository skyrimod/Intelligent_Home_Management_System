//
// Created by 73227 on 2025/7/1.
//
#include "tasks_manager.h"
#include "init_task.h"
#include "mq2_task.h"
#include "dht11_task.h"
#include "lcd_task.h"
#include "log_task.h"
#include "wdg_task.h"

// 队列
QueueHandle_t dht11SensorQueue;
QueueHandle_t mq2SensorQueue;
QueueHandle_t logQueue;

// 事件组
EventGroupHandle_t wdgEventGroup;

// 信号量
SemaphoreHandle_t dmaSemaphore;
SemaphoreHandle_t logSemaphore;

typedef struct {
    uint8_t enable;
    void (*task)(void *argument);
    void *argument;
    const osThreadAttr_t attr;
} TaskInfo_t;

TaskInfo_t task_tab[] = {
        {
            .enable = 1,
            .task = init_task,
            .argument = NULL,
            .attr = {
                    .name = "initTask",
                    .stack_size = 1024 * 2,
                    .priority = (osPriority_t) osPriorityRealtime,
            }
        },
        {
            .enable = 1,
            .task = dht11_read_task,
            .argument = NULL,
            .attr = {
                    .name = "dht11ReadTask",
                    .stack_size = 512,
                    .priority = (osPriority_t) osPriorityHigh,
            }
        },
        {
            .enable = 1,
            .task = lcd_show_task,
            .argument = NULL,
            .attr = {
                    .name = "lcdShowTask",
                    .stack_size = 1024 * 4,
                    .priority = (osPriority_t) osPriorityNormal,
            }
        },
        {
            .enable = 1,
            .task = wdg_task,
            .argument = NULL,
            .attr = {
                    .name = "wdgTask",
                    .stack_size = 128 * 2,
                    .priority = (osPriority_t) osPriorityBelowNormal,
            }
        }
};

//osThreadId_t mq2ReadHandle;
//const osThreadAttr_t mq2ReadTask_attr = {
//        .name = "mq2ReadTask",
//        .stack_size = 512,
//        .priority = (osPriority_t) osPriorityHigh,
//};
//
//osThreadId_t logHandle;
//const osThreadAttr_t logTask_attr = {
//        .name = "logTask",
//        .stack_size = 1024 * 2,
//        .priority = (osPriority_t) osPriorityLow,
//};


void my_tasks_init(void ){
    // 消息队列
    dht11SensorQueue = xQueueCreate(5, sizeof(DHT11_SensorMessage_t));
//    mq2SensorQueue = xQueueCreate(2, sizeof(MQ2_SensorMessage_t));

    // 创建事件组
    wdgEventGroup = xEventGroupCreate();

    // 创建信号量
    logSemaphore = xSemaphoreCreateMutex();

    // 创建任务
    uint8_t len = sizeof(task_tab)/sizeof(TaskInfo_t);
    for (int i = 0; i < len; ++i) {
        if (task_tab[i].enable){
            osThreadNew(task_tab[i].task, task_tab[i].argument, &(task_tab[i].attr));
        }
    }
}
