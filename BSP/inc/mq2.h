//
// Created by 73227 on 2025/7/1.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_MQ2_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_MQ2_H

#include "stm32f1xx.h"
typedef struct {
    float voltage;
    float ppm;
} MQ2_DATA_t;

#define MQ2_AO_GPIO_PORT    GPIOA
#define MQ2_AO_GPIO_PIN     GPIO_PIN_1

#define MQ2_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

void mq2_init(void );
MQ2_DATA_t mq2_getValue(void );


#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_MQ2_H
