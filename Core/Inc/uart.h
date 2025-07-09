//
// Created by 73227 on 2025/7/9.
//

#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_UART_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_UART_H

#include "stm32f1xx.h"

// 引脚定义
#define UART1_GPIO_PORT     GPIOA
#define UART1_TX_GPIO_PIN   GPIO_PIN_9
#define UART1_RX_GPIO_PIN   GPIO_PIN_10

#define UART1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART1_CLK_ENABLE()          __HAL_RCC_USART1_CLK_ENABLE()

#define UART1   USART1

#define DMAx_CHy    DMA1_Channel4

// 接口
void uart_init(uint32_t baudrate);
void uart1_send_dma(uint8_t *buf, uint16_t len);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_UART_H
