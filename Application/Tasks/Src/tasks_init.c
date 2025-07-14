//
// Created by 73227 on 2025/7/1.
//
#include "tasks_init.h"
#include "init_task.h"
#include "mq2_task.h"
#include "dht11_task.h"
#include "lcd_task.h"
#include "log_task.h"

QueueHandle_t dht11SensorQueue;
QueueHandle_t mq2SensorQueue;
QueueHandle_t logQueue;

SemaphoreHandle_t dmaSemaphore;
SemaphoreHandle_t logSemaphore;

typedef struct {
    void (*task)(void *argument);
    void *argument;
    const osThreadAttr_t attr;
} TaskInfo_t;

TaskInfo_t task_tab[] = {
        {
            .task = init_task,
            .argument = NULL,
            .attr = {
                    .name = "initTask",
                    .stack_size = 1024 * 2,
                    .priority = (osPriority_t) osPriorityRealtime,
            }
        },
        {
            .task = dht11_read_task,
            .argument = NULL,
            .attr = {
                    .name = "dht11ReadTask",
                    .stack_size = 512,
                    .priority = (osPriority_t) osPriorityHigh,
            }
        },
        {
            .task = lcd_show_task,
            .argument = NULL,
            .attr = {
                    .name = "lcdShowTask",
                    .stack_size = 1024 * 4,
                    .priority = (osPriority_t) osPriorityNormal,
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

    // 创建信号量
    logSemaphore = xSemaphoreCreateMutex();

    // 创建任务
    uint8_t len = sizeof(task_tab)/sizeof(TaskInfo_t);
    for (int i = 0; i < len; ++i) {
        osThreadNew(task_tab[i].task, task_tab[i].argument, &(task_tab[i].attr));
    }
}
