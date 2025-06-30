//
// Created by 73227 on 2025/6/29.
//
/*
 * DHT11使用单总线，有us级通信需求，所以将一次读数据划分为两次任务
 * 一、开始信号 18ms
 * 二、等待响应 + 读数据 nus
 * */
#include "dht11.h"
#include "dwt_delay.h"
#include "cmsis_os2.h"

GPIO_InitTypeDef gpio_init_struct = {0};



void dht11_init(void){
    DHT11_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin = DHT11_DATA_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_DATA_PORT, &gpio_init_struct);

    HAL_GPIO_WritePin(DHT11_DATA_PORT, DHT11_DATA_PIN, GPIO_PIN_SET);


}
// 需要18ms延时
void dht11_start(void){
    // 拉低
    DHT11_DATA_OUT(0);
    osDelay(18);
}

HAL_StatusTypeDef dht11_read_data(dht11_data_t *data ){
    uint8_t timeout = 150;
    uint8_t raw[5] = {0};

    // 拉高40us等待响应
    DHT11_DATA_OUT(1);
    delay_us(40);

    // 切换输入模式
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(DHT11_DATA_PORT, &gpio_init_struct);

    // 等待响应,引入超时机制
    while (HAL_GPIO_ReadPin(DHT11_DATA_PORT, DHT11_DATA_PIN) == GPIO_PIN_SET){
        if (timeout-- == 0){
            return HAL_ERROR;
        }
        delay_us(1);
    }

    // 读取数据
    for (int i = 0; i < 40; ++i) {
        // 等待50us结束
        timeout = 100;
        while (HAL_GPIO_ReadPin(DHT11_DATA_PORT, DHT11_DATA_PIN) == GPIO_PIN_RESET){
            if (timeout-- == 0)
                break;
            delay_us(1);
        }

        // 测量高电平时间
        uint32_t count = 0;
        timeout = 100;
        while (HAL_GPIO_ReadPin(DHT11_DATA_PORT, DHT11_DATA_PIN) == GPIO_PIN_SET){
            if (timeout-- == 0) break;
            delay_us(1);
            count++;
        }

        raw[i/8] <<= 1;
        if (count > 40) raw[i/8] |= 1;
    }

    // 恢复输出模式
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(DHT11_DATA_PORT, &gpio_init_struct);

    // 数据校验
    if (raw[4] != raw[0] + raw[1] + raw[2] + raw[3]){
        return HAL_ERROR;
    }

    data->humidity = raw[0] + raw[1]/10.0f;
    data->temperature = raw[2] + raw[3]/10.0f;
    return HAL_OK;
}
