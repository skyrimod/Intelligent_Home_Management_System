//
// Created by 73227 on 2025/7/9.
//
#include "log_task.h"

void log_init(void ){
    elog_init();

    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL &~ (ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_LVL | ELOG_FMT_TAG);

    elog_start();
}

void log_task(void *argument){
    LogMessage msg;
    static uint8_t *lastSentBuffer = NULL;

    for (;;){
        if (xQueueReceive(logQueue, &msg, portMAX_DELAY) == pdPASS){
            // 等待上次DMA完成
            xSemaphoreTake(dmaSemaphore, portMAX_DELAY);

            // 释放上一次发送的缓存
            if (lastSentBuffer){
                vPortFree(lastSentBuffer);
                lastSentBuffer = NULL;
            }

            // 启动本次DMA发送并记录缓冲区
            lastSentBuffer = msg.data;
            uart1_send_dma(msg.data, msg.len);
        }
    }
}

// DMA发送完成后回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    if (huart->Instance == UART1){
        // 发送信号量通知任务DMA完成
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(dmaSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// DMA错误处理
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart){
    if (huart->Instance == UART1){
        // 错误处理：释放信号量，避免系统死锁
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(dmaSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        // 添加日志等
    }
}
