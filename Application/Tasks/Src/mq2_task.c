//
// Created by 73227 on 2025/7/1.
//
#include "mq2_task.h"

void mq2_read_task(void *argument){
    for(;;){
        taskENTER_CRITICAL();
        MQ2_SensorMessage_t msg = {0};
        MQ2_DATA_t mq2Data = {0};

        mq2Data = mq2_getValue();

        msg.data = mq2Data;
        msg.status = HAL_OK;

        xQueueSend(mq2SensorQueue, &msg, 0);
        taskEXIT_CRITICAL();

        osDelay(300);
    }
}
