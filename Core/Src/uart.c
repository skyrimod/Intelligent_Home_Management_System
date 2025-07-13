//
// Created by 73227 on 2025/7/9.
//
#include "uart.h"
#include "main.h"

UART_HandleTypeDef huart = {0};
DMA_HandleTypeDef hdma = {0};

void HAL_UART_MspInit(UART_HandleTypeDef *huart){
    GPIO_InitTypeDef gpio_init_struct = {0};

    if (huart->Instance == USART1){
        UART1_GPIO_CLK_ENABLE();
        UART1_CLK_ENABLE();

        gpio_init_struct.Pin = UART1_TX_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(UART1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = UART1_RX_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(UART1_GPIO_PORT, &gpio_init_struct);

        // 初始化DMA
        __HAL_RCC_DMA1_CLK_ENABLE();

        hdma.Instance = DMAx_CHy;
        hdma.Init.Mode = DMA_NORMAL;
        hdma.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma.Init.MemInc = DMA_MINC_ENABLE;
        hdma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma.Init.Priority = DMA_PRIORITY_HIGH;

        if (HAL_DMA_Init(&hdma) != HAL_OK){
            Error_Handler();
        }

        // 连接UART1与DMA
        __HAL_LINKDMA(huart, hdmatx, hdma);

        HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    }
}

void uart_init(uint32_t baudrate){
    // 初始化UART1
    huart.Instance = UART1;
    huart.Init.BaudRate = baudrate;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart) != HAL_OK){
        Error_Handler();
    }

}

void uart1_send_dma(uint8_t *buf, uint16_t len){
    HAL_UART_Transmit_DMA(&huart, buf, len);
}


