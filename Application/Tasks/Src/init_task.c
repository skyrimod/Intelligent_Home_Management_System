//
// Created by 73227 on 2025/7/1.
//
#include "init_task.h"
#include "dht11.h"
#include "lcd.h"
#include "mq2.h"
#include "dwt_delay.h"
#include "cmsis_os2.h"

void init_task(void *argument){
    dwt_init();

    if (dht11_init() == 1){
        for(;;){
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        }
    }

    mq2_init();

    lcd_init();

    osThreadExit();
}
