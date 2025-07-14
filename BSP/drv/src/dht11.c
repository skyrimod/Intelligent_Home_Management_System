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

// 开始信号
static void dht_reset(void)
{
    DHT11_DQ_OUT(0);
    delay_ms(20);
    DHT11_DQ_OUT(1);
    delay_us(30);
}

// 相应信号
uint8_t dht11_check(void) {
    uint8_t retry = 0;
    uint8_t rval = 0;

    while (DHT11_DQ_IN && retry < 100)  /* DHT11会拉低约83us */
    {
        retry++;
        delay_us(1);
    }

    if (retry >= 100) {
        rval = 1;
    } else {
        retry = 0;

        while (!DHT11_DQ_IN && retry < 100) /* DHT11拉低后会再次拉高约87us */
        {
            retry++;
            delay_us(1);
        }
        if (retry >= 100) rval = 1;
    }

    return rval;
}
// 读1bit
uint8_t dht_read_bit()
{
    uint8_t retry = 0;
    // 捕获低电平
    while(DHT11_DQ_IN && retry < 100){
        retry++;
        delay_us(1);
    }

    retry = 0;

    // 捕获高电平
    while(!DHT11_DQ_IN && retry < 100){
        retry++;
        delay_us(1);
    }

    delay_us(40);

    if(DHT11_DQ_IN){
        return 1;
    }else{
        return 0;
    }
}

// 读1bytes
static uint8_t dht_read_bytes(void){
    uint8_t i;
    uint8_t data = 0;

    for(i = 0; i < 8; i++){
        data <<= 1;
        data |= dht_read_bit();
    }
    return data;
}

// 读数据
uint8_t dht11_read_data(dht11_data_t *data){

    uint8_t buf[5];
    uint8_t i;
    dht_reset();

    if(dht11_check() == 0){
        for(i = 0; i<5; i++){
            buf[i] = dht_read_bytes();
        }

        // 校验数据
        if(buf[0] + buf[1] + buf[2] + buf[3] == buf[4]){
            data->humidity = buf[0];
            data->temperature = buf[2];
        }
    }else{
        return 1;
    }

    return 0;
}
uint8_t dht11_init(void){
    DHT11_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin = DHT11_DATA_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_DATA_PORT, &gpio_init_struct);

    dht_reset();
    return dht11_check();
}


