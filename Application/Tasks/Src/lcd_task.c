//
// Created by 73227 on 2025/7/1.
//
#include "lcd_task.h"
#include "dht11_task.h"
#include "mq2_task.h"
#include "stdio.h"

void lcd_show_task(void *argument){
    DHT11_SensorMessage_t dht11Msg;
    MQ2_SensorMessage_t mq2Msg;
    uint16_t adcx = 0;
    float temp = 0;
    for (;;){
        lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
        lcd_show_string(30, 70, 200, 16, 16, "DHT11 TEST", RED);
        if (xQueueReceive(dht11SensorQueue  , &dht11Msg, pdMS_TO_TICKS(3500))){
            if (dht11Msg.status == HAL_OK){
                lcd_show_string(30, 90, 200, 16, 16, "DHT11 DATA OK", RED);
                lcd_show_string(30, 120, 200, 16, 16, "Temp:  C", BLUE);
                lcd_show_num(30 + 40, 120, dht11Msg.data.temperature, 2, 16, BLUE);
                lcd_show_string(30, 150, 200, 16, 16, "Humi:  %", BLUE);
                lcd_show_num(30 + 40, 150, dht11Msg.data.humidity, 2, 16, BLUE);
            } else{
                lcd_show_string(30, 90, 200, 16, 16, "DHT11 ERROR", RED);
            }
        } else{
            lcd_show_string(30, 90, 200, 16, 16, "DHT11 Read Data Timeout!!!", RED);
        }

        if (xQueueReceive(mq2SensorQueue, &mq2Msg, pdMS_TO_TICKS(3000))){
            lcd_show_string(30, 180, 200, 16, 16, "MQ2 DATA: ", RED);
            lcd_show_string(30, 210, 200, 16, 16, "MQ2 Vol:  0.000", BLUE);
            // 显示整数
            temp = mq2Msg.data.voltage;
            adcx = temp;
            lcd_show_xnum(30 + 80, 210, adcx, 1, 16, 0, BLUE);
            // 显示小数
            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(30 + 80 + 16, 210, temp, 3, 16, 0x80, BLUE);

            lcd_show_string(30, 240, 200, 16, 16, "MQ2 ppm:  0.000", BLUE);
            // 显示整数
            temp = mq2Msg.data.ppm;
            adcx = temp;
            lcd_show_xnum(30 + 80, 240, adcx, 1, 16, 0, BLUE);
            // 显示小数
            temp -= adcx;
            temp *= 1000;
            lcd_show_xnum(30 + 80 + 16, 240, temp, 3, 16, 0x80, BLUE);
        }
        osDelay(500);
    }
}