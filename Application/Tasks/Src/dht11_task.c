//
// Created by 73227 on 2025/7/1.
//
#include "dht11_task.h"

void dht11_read_task(void *argument){
    for (;;){
        taskENTER_CRITICAL();

        DHT11_SensorMessage_t msg;

        msg.status = dht11_read_data(&msg.data);
        msg.timestamp = HAL_GetTick();

        // 发送到队列
        xQueueSend(dht11SensorQueue, &msg, 0);

        taskEXIT_CRITICAL();

        osDelay(200);
    }
}
