//
// Created by 73227 on 2025/7/26.
//
#include <stddef.h>
#include "sensor_config.h"

static SensorOperations ops[MAX_SENSOR_TYPES] = {0};

// 注册传感器
void Sensor_RegisterDriver(SensorType type, SensorInitFunc init, SensorReadFunc read, SensorDeinitFunc deinit){
    if (type < MAX_SENSOR_TYPES){
        ops[type] = (SensorOperations){
            .init = init,
            .read = read,
            .deinit = deinit
        };
    }
}

// 初始化传感器
SensorStatus Sensor_Init(SensorConfig *config){
    if (config == NULL || config->hw_config == NULL){
        return SENSOR_ERR_INVALID_PARAM;
    }

    if (config->type >= MAX_SENSOR_TYPES || ops[config->type].init == NULL){
        return SENSOR_ERR_NOT_SUPPORTED;
    }

    return ops[config->type].init(config->hw_config);
}

// 读取传感器数据
SensorStatus Sensor_Read(SensorConfig *config, float *value, SensorValueType valueType){
    if (config == NULL){
        return SENSOR_ERR_INVALID_PARAM;
    }

    if (config->type >= MAX_SENSOR_TYPES || ops[config->type].read == NULL){
        return SENSOR_ERR_NOT_SUPPORTED;
    }

    return ops[config->type].read(config->hw_config, value, valueType);
}

// 注销传感器
SensorStatus Sensor_Deinit(SensorConfig *config){
    if (config == NULL){
        return SENSOR_ERR_INVALID_PARAM;
    }

    if (config->type >= MAX_SENSOR_TYPES || ops[config->type].deinit == NULL){
        return SENSOR_ERR_NOT_SUPPORTED;
    }

    return ops[config->type].deinit(config->hw_config);
}
