/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "dht11.h"
#include "dwt_delay.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    dht11_data_t data;
    uint32_t timestamp;
    HAL_StatusTypeDef status;
} SensorMessage;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SemaphoreHandle_t dht11_binary;
QueueHandle_t xSensorQueue;
const UBaseType_t uxQueueLength = 5;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
        .name = "defaultTask",
        .stack_size = 64 * 4,
        .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
        .name = "ledTask",
        .stack_size = 48 * 4,
        .priority = (osPriority_t) osPriorityLow,
};
osThreadId_t drawPointHandle;
const osThreadAttr_t drawPoint_attributes = {
        .name = "drawPoint",
        .stack_size = 192 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
osThreadId_t initHandle;
const osThreadAttr_t init_attributes = {
        .name = "init",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityRealtime1,
};

osThreadId_t dht11StartHandle;
const osThreadAttr_t dht11Start_attributes = {
        .name = "dht11_start",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityHigh,
};

osThreadId_t dht11ReadHandle;
const osThreadAttr_t dht11Read_attributes = {
        .name = "dht11_read",
        .stack_size = 128 *4,
        .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void draw_test_task(void *argument);
void init_task(void *argument);
void led_toggle_task(void *argument);

void dht11_start_task(void *argument);
void dht11_read_task(void *argument);



void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);

unsigned long getRunTimeCounterValue(void);

void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void) {

}

__weak unsigned long getRunTimeCounterValue(void) {
    return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName) {
    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
    called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    dht11_binary = xSemaphoreCreateBinary();
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    xSensorQueue = xQueueCreate(uxQueueLength, sizeof(SensorMessage));
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of defaultTask */
    initHandle = osThreadNew(init_task, NULL, &init_attributes);
    drawPointHandle = osThreadNew(draw_test_task, NULL, &drawPoint_attributes);
    ledTaskHandle = osThreadNew(led_toggle_task, NULL, &ledTask_attributes);
    dht11StartHandle = osThreadNew(dht11_start_task, NULL, &dht11Start_attributes);
    dht11ReadHandle = osThreadNew(dht11_read_task, NULL, &dht11Read_attributes);
    defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

    /* creation of ledTask */

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
    /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop */
    for (;;) {
        osDelay(1);
    }
    /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_led_toggle_task */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_toggle_task */
void led_toggle_task(void *argument){
    for (;;) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        osDelay(1000);
    }
}
void draw_test_task(void  *argument){
    SensorMessage msg;
    for (;;){
        lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
        lcd_show_string(30, 70, 200, 16, 16, "DHT11 TEST", RED);
        if (xQueueReceive(xSensorQueue, &msg, pdMS_TO_TICKS(3500))){
            if (msg.status == HAL_OK){
                lcd_show_string(30, 90, 200, 16, 16, "DHT11 DATA OK", RED);
                lcd_show_string(30, 120, 200, 16, 16, "Temp:  C", BLUE);
                lcd_show_num(30 + 40, 120, msg.data.temperature, 2, 16, BLUE);
                lcd_show_string(30, 150, 200, 16, 16, "Humi:  %", BLUE);
                lcd_show_num(30 + 40, 150, msg.data.humidity, 2, 16, BLUE);
            } else{
                lcd_show_string(30, 90, 200, 16, 16, "DHT11 ERROR", RED);
            }
        } else{
            lcd_show_string(30, 90, 200, 16, 16, "DHT11 Read Data Timeout!!!", RED);
        }
        osDelay(5000);
    }
}

void init_task(void *argument){
    dwt_init();
    dht11_init();
    lcd_init();
    osThreadExit();
}

// dht11开始信号任务
void dht11_start_task(void *argument){
    dht11_binary = xSemaphoreCreateBinary();
    for(;;){
        dht11_start();
        // 唤醒读取任务
        xSemaphoreGive(dht11_binary);
        // 每3s采集一次数据
        osDelay(3000);
    }
}
// dht等待响应并读取任务
void dht11_read_task(void *argument){
    for(;;){
        xSemaphoreTake(dht11_binary, portMAX_DELAY);

        taskENTER_CRITICAL();

        SensorMessage msg = {0};
        msg.status = dht11_read_data(&msg.data);
        msg.timestamp = HAL_GetTick();

        // 发送到队列
        xQueueSend(xSensorQueue, &msg, 0);

        taskEXIT_CRITICAL();

        osDelay(1000);
    }
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

