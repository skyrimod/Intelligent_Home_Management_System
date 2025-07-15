//
// Created by 73227 on 2025/7/15.
//

#include "wdg.h"

IWDG_HandleTypeDef hiwdg = {0};

void iwdg_init(void ){
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
    hiwdg.Init.Reload = 625 * 3;

    if(HAL_IWDG_Init(&hiwdg) != HAL_OK){
        while (1);
    }
}

void iwdg_feed(void ){
    HAL_IWDG_Refresh(&hiwdg);
}