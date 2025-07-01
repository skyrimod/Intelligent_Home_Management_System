//
// Created by 73227 on 2025/7/1.
//
#include "mq2.h"
#include "dwt_delay.h"
#include "math.h"

ADC_HandleTypeDef g_adc_handle;

void mq2_init(void ){
    // 初始化ADC
    g_adc_handle.Instance = ADC1;
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    g_adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
    g_adc_handle.Init.ContinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfConversion = 1;
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;
    g_adc_handle.Init.NbrOfDiscConversion = 0;
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&g_adc_handle);

    // 校准ADC
    HAL_ADCEx_Calibration_Start(&g_adc_handle);


}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc){
    if (hadc->Instance = ADC1){
        GPIO_InitTypeDef gpio_init_struct={0};
        RCC_PeriphCLKInitTypeDef periphClkInitType = {0};

        // 使能时钟
        MQ2_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();

        periphClkInitType.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        periphClkInitType.AdcClockSelection = RCC_ADCPCLK2_DIV2;
        HAL_RCCEx_PeriphCLKConfig(&periphClkInitType);

        // 初始化GPIOA
        gpio_init_struct.Pin = MQ2_AO_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(MQ2_AO_GPIO_PORT, &gpio_init_struct);
    }
}

void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime){
    ADC_ChannelConfTypeDef adc_channel_type = {0};
    // 配置ADC通道
    adc_channel_type.Channel = ch;
    adc_channel_type.Rank = rank;
    adc_channel_type.SamplingTime = stime;
    HAL_ADC_ConfigChannel(&g_adc_handle, &adc_channel_type);
}

MQ2_DATA_t mq2_getValue(void){
    uint32_t adcVol = 0;
    MQ2_DATA_t mq2Data = {0};
    uint8_t times = 10;

    // 读取10次，取平均值
    for (int i = 0; i < times; i++){
        adc_channel_set(&g_adc_handle, ADC_CHANNEL_1, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5);

        HAL_ADC_Start(&g_adc_handle);
        HAL_ADC_PollForConversion(&g_adc_handle, 10);
        adcVol +=(uint16_t) HAL_ADC_GetValue(&g_adc_handle);
        delay_ms(5);
    }

    // 取平均
    adcVol /= times;

    // 计算vol
    float vol = (float )adcVol * 5.0 / 4096;
    // 计算ppm
    float RS = (5 - vol) / (vol * 0.5);
    float R0 = 6.64;
    float ppm = pow(11.5428 * R0 / RS, 0.6549f);

    mq2Data.voltage = vol;
    mq2Data.ppm = ppm;
    return mq2Data;
}

