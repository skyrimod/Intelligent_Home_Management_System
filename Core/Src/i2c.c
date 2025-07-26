//
// Created by 73227 on 2025/7/26.
//
#include "i2c.h"

I2C_HandleTypeDef hi2c;

void i2c_init(void ){
    hi2c.Instance = I2C1;
    hi2c.Init.ClockSpeed = 400000;
    hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.OwnAddress2 = 0;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    HAL_I2C_Init(&hi2c);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c){
    if (hi2c->Instance == I2C1){
        I2C_SCL_CLK_ENABLE();
        I2C_SDA_CLK_ENABLE();
        I2C_CLK_ENABLE();

        GPIO_InitTypeDef gpioInitTypeDef = {0};

        gpioInitTypeDef.Pin = I2C_SCL_PIN;
        gpioInitTypeDef.Mode = GPIO_MODE_AF_OD;
        gpioInitTypeDef.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(I2C_SCL_PROT,&gpioInitTypeDef);

        gpioInitTypeDef.Pin = I2C_SDA_PIN;
        HAL_GPIO_Init(I2C_SDA_PORT, &gpioInitTypeDef);
    }
}