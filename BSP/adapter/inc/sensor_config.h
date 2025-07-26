//
// Created by 73227 on 2025/7/18.
//
#ifndef INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SEN_CONFIG_H
#define INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SEN_CONFIG_H

#include <stdint.h>

typedef enum {
    SENSOR_TEMP,
    SENSOR_HUMI
} SensorValueType;

// 在头文件配置要调用的传感器
typedef enum {
    SENSOR_SHT30,
    SENSOR_DHT11,
    SENSOR_DS18B02,
    MAX_SENSOR_TYPES
} SensorType;

typedef enum {
    SENSOR_OK=0,
    SENSOR_ERR_INVALID_PARAM,
    SENSOR_ERR_COMM_FAIL,
    SENSOR_ERR_DATA_CORRUPTED,
    SENSOR_ERR_NOT_SUPPORTED
} SensorStatus;

typedef SensorStatus (*SensorInitFunc)(void *);
typedef SensorStatus (*SensorReadFunc)(void *, float *, SensorValueType);
typedef SensorStatus (*SensorDeinitFunc)(void *);

typedef struct {
    SensorInitFunc init;
    SensorReadFunc read;
    SensorDeinitFunc deinit;
} SensorOperations;

typedef struct {
    SensorType type;
    void *hw_config;
} SensorConfig;

void Sensor_RegisterDriver(SensorType type, SensorInitFunc init, SensorReadFunc read, SensorDeinitFunc deinit);
SensorStatus Sensor_Init(SensorConfig *config);
SensorStatus Sensor_Read(SensorConfig *config, float *value, SensorValueType valueType);

#endif //INTELLIGENT_HOME_MANAGEMENT_SYSTEM_SEN_CONFIG_H
