#include "blink_demo.h"

#include "esp_log.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"


static const gpio_num_t red_led = GPIO_NUM_1;
static const uint32_t  red_delay_ms = 1000;
static const gpio_num_t green_led = GPIO_NUM_2;
static const uint32_t  green_delay_ms = 100;


typedef struct {
    int led_pin;
    int delay_ms;
} LEDTaskParams;

void led_task(void *parameters){

    LEDTaskParams *params = (LEDTaskParams *)parameters;

    int led = params->led_pin;
    int delay = params->delay_ms;

    while (1) {
        gpio_set_level(led, 1);
        vTaskDelay(pdMS_TO_TICKS(delay));
        gpio_set_level(led, 0);
        vTaskDelay(pdMS_TO_TICKS(delay));
    }
}

// blinks two LEDs with freeRTOS tasks
void blink_demo(void)
{
    vTaskDelay(pdMS_TO_TICKS(5000));
    char *taskName = pcTaskGetName(NULL);
    ESP_LOGI(taskName, "Starting Program");

    gpio_reset_pin(red_led);
    gpio_set_direction(red_led, GPIO_MODE_OUTPUT);

    gpio_reset_pin(green_led);
    gpio_set_direction(green_led, GPIO_MODE_OUTPUT);


    // declare static so that it is not in the stack and can't be overwritten
    // while in use by LEDTask
    static LEDTaskParams redParams = { 
        .led_pin = red_led,
        .delay_ms = red_delay_ms
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
        .led_pin = green_led,
        .delay_ms = green_delay_ms
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