#include "logs_demo.h"

#include <stdio.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// tests logs
void log_demo(void)
{
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGE("TEST", "Error works");
    ESP_LOGW("TEST", "Warning works");
    ESP_LOGI("TEST", "Info works");
    ESP_LOGD("TEST", "Debug works");
    ESP_LOGV("TEST", "Verbose works");
}