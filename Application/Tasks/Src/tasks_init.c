//
// Created by 73227 on 2025/7/1.
//
#include "tasks_init.h"
#include "init_task.h"
#include "mq2_task.h"
#include "dht11_task.h"
#include "lcd_task.h"

QueueHandle_t dht11SensorQueue;
QueueHandle_t mq2SensorQueue;

osThreadId_t initTaskHandle;
const osThreadAttr_t initTask_attr = {
        .name = "initTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityRealtime,
};

osThreadId_t dht11ReadHandle;
const osThreadAttr_t dht11ReadTask_attr = {
        .name = "dht11ReadTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityHigh,
};

osThreadId_t mq2ReadHandle;
const osThreadAttr_t mq2ReadTask_attr = {
        .name = "mq2ReadTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityHigh,
};

osThreadId_t lcdShowHandle;
const osThreadAttr_t lcdShowTask_attr = {
        .name = "lcdShowTask",
        .stack_size = 192 * 4,
        .priority = (osPriority_t) osPriorityLow,
};

void my_tasks_init(void ){
    // 消息队列
    dht11SensorQueue = xQueueCreate(5, sizeof(DHT11_SensorMessage_t));
    mq2SensorQueue = xQueueCreate(2, sizeof(MQ2_SensorMessage_t));
    // 创建任务
    initTaskHandle = osThreadNew(init_task, NULL, &initTask_attr);
    dht11ReadHandle = osThreadNew(dht11_read_task, NULL, &dht11ReadTask_attr);
//    mq2ReadHandle = osThreadNew(mq2_read_task, NULL, &mq2ReadTask_attr);
    lcdShowHandle = osThreadNew(lcd_show_task, NULL, &lcdShowTask_attr);
}
