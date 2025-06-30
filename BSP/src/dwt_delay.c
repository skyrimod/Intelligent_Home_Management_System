//
// Created by 73227 on 2025/6/30.
//
#include "dwt_delay.h"

void dwt_init(void ){
    // 启用DWT
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delay_us(uint16_t us){
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = us * (SystemCoreClock / 1000000);
//    while (DWT->CYCCNT - start < cycles);
// 修复的等待逻辑（处理计数器溢出）
    while (1) {
        uint32_t current = DWT->CYCCNT;
        uint32_t elapsed;

        if (current >= start) {
            elapsed = current - start;
        } else {
            // 处理计数器回绕
            elapsed = (0xFFFFFFFF - start) + current + 1;
        }

        if (elapsed >= cycles) break;
    }
}

void delay_ms(uint16_t ms){
    for (int i = 0; i < ms; ++i) {
        delay_us(1000);
    }
}
