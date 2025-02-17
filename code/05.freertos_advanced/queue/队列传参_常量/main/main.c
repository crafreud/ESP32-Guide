#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

static const char *TAG = "main";

void Task_1(void *pvParameters)
{
    // 取得队列句柄
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    int i = 0;

    for (;;)
    {
        // 发送数据到队列
        if (xQueueSend(xQueue, &i, 0)!= pdPASS) {
            ESP_LOGI(TAG, "数据发送失败");
        }
        else
        {
            ESP_LOGI(TAG, "数据发送成功");
            i++;
        }

        if(i == 10)
        {
            i = 0;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void Task_2(void *pvParameters)
{
    // 取得队列句柄
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    for (;;)
    {
        int receivedData;
        if (xQueueReceive(xQueue, &receivedData, 0) != pdPASS)
        {
            ESP_LOGI(TAG, "数据接收失败");
        }
        else
        {
            ESP_LOGI(TAG, "数据接收成功，数据为：%d", receivedData);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    TaskHandle_t taskHandle_1 = NULL;
    TaskHandle_t taskHandle_2 = NULL;
    QueueHandle_t xQueue;

    // 创建队列
    xQueue = xQueueCreate(10, sizeof(int));

    if (xQueue != NULL)
    {
        ESP_LOGI(TAG, "队列创建成功");
        // 发送数据任务
        xTaskCreate(Task_1, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_1);
        // 接收数据任务
        xTaskCreate(Task_2, "Task_1", 1024 * 4, (void *)xQueue, 12, &taskHandle_2);
    }
    else
    {
        ESP_LOGI(TAG, "队列创建失败");
    }
}
