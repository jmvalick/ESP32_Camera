#include "blink_demo.h"

#include <stdio.h>

#include "esp_log.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#define GPIO_ON 1
#define GPIO_OFF 0

#define RED_LED GPIO_NUM_1
#define RED_DELAY 1000
#define GREEN_LED GPIO_NUM_2
#define GREEN_DELAY 100


typedef struct {
    int led_pin;
    int delay_ms;
} LEDTaskParams;

void led_task(void *parameters){

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

// blinks two LEDs with freeRTOS tasks
void blink_demo(void)
{
    vTaskDelay(pdMS_TO_TICKS(5000));
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
        .delay_ms = RED_DELAY
    };

    TaskHandle_t redLEDtask = NULL;

    BaseType_t redLEDstatus = xTaskCreate(
        led_task,
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
        .delay_ms = GREEN_DELAY
    };

    TaskHandle_t greenLEDtask = NULL;

    BaseType_t greenLEDstatus = xTaskCreate(
        led_task,
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