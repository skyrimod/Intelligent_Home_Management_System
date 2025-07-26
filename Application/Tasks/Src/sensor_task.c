//
// Created by 73227 on 2025/7/26.
//
#include "sensor_task.h"
#include "sht30.h"
#include "i2c.h"

void sensor_task(void *argument){
    SensorMsg msg = {0};

    // 注册sht30
    SHT30_Register();

    SHT30_Config sht30_cfg = {
           .hi2c = &hi2c,
           .i2c_addr = 0x44     // SHT30默认地址
    };

    SensorConfig sen_cfg = {
            .type = SENSOR_SHT30,
            .hw_config = &sht30_cfg
    };

    SensorStatus status = Sensor_Init(&sen_cfg);
    if (status != SENSOR_OK){
        Error_Handler();
    }

    for (;;){

        float temp, humi;

        // 读取温度
        status = Sensor_Read(&sen_cfg, &temp, SENSOR_TEMP);
        if (status == SENSOR_OK){
            msg.temperature = temp;
        }

        // 读取湿度
        status = Sensor_Read(&sen_cfg, &humi, SENSOR_HUMI);
        if (status == SENSOR_OK){
            msg.humidity = humi;
        }

        // 放入队列
        xQueueSend(sensorQueue, &msg, 0);

        xEventGroupSetBits(wdgEventGroup, SENSOR_TASK_BIT);

        osDelay(100);
    }
}
