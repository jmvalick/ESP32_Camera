#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"

#define GPIO_ON 1
#define GPIO_OFF 0

#define RED_LED 1
#define GREEN_LED 2


typedef struct {
    int led_pin;
    int delay_ms;
} LEDTaskParams;

void LEDTask(void *parameters){

    LEDTaskParams *params = (LEDTaskParams *)parameters;

    int led = params->led_pin;
    int delay = params->delay_ms;

    while (1) {
        gpio_set_level(led, GPIO_ON);
        vTaskDelay(pdMS_TO_TICKS(delay));
        gpio_set_level(led, GPIO_OFF);
        vTaskDelay(pdMS_TO_TICKS(delay));
    }
}

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(5000));
    ESP_LOGE("TEST", "Error works");
    ESP_LOGW("TEST", "Warning works");
    ESP_LOGI("TEST", "Info works");
    ESP_LOGD("TEST", "Debug works");
    ESP_LOGV("TEST", "Verbose works");

    char *taskName = pcTaskGetName(NULL);
    ESP_LOGI(taskName, "Starting Program");

    gpio_reset_pin(RED_LED);
    gpio_set_direction(RED_LED, GPIO_MODE_OUTPUT);

    gpio_reset_pin(GREEN_LED);
    gpio_set_direction(GREEN_LED, GPIO_MODE_OUTPUT);


    // declare static so that it is not in the stack and can't be overwritten
    // while in use by LEDTask
    static LEDTaskParams redParams = { 
        .led_pin = RED_LED,
        .delay_ms = 1000
    };

    TaskHandle_t redLEDtask = NULL;

    BaseType_t redLEDstatus = xTaskCreate(
        LEDTask,
        "Red LED",
        2048,
        &redParams,
        tskIDLE_PRIORITY,
        &redLEDtask);

    if (redLEDstatus != pdPASS)
    {
        ESP_LOGE("TASK", "Red LED task creation failed");
    }


    static LEDTaskParams greenParams = {
        .led_pin = GREEN_LED,
        .delay_ms = 100
    };

    TaskHandle_t greenLEDtask = NULL;

    BaseType_t greenLEDstatus = xTaskCreate(
        LEDTask,
        "Green LED",
        2048,
        &greenParams,
        tskIDLE_PRIORITY,
        &greenLEDtask);

    if (greenLEDstatus != pdPASS)
    {
        ESP_LOGE("TASK", "Green LED task creation failed");
    }
}