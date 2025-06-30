//
// Created by 73227 on 2025/6/30.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DWT_DELAY_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DWT_DELAY_H

#include "stm32f1xx.h"

void dwt_init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_DWT_DELAY_H
