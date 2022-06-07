#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../include/config.hpp"
#include <Indicator.hpp>
#include <logs.hpp>

using LedIndicator = indicator::LedIndicator<config::LED_RED,
                            config::LED_GREAN,
                            config::LED_BLUE, 
                            config::LED_COMMON, 
                            indicator::LedType::CommonCathode>;

void task_blick_led(void* ptr) 
{
    auto ledIndicator = reinterpret_cast<LedIndicator*>(ptr);

    while (true)
    {
        ledIndicator->onBlue();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ledIndicator->allOff();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    

}

extern "C" void app_main() 
{
    LedIndicator ledIndicator;
    xTaskCreate(task_blick_led, "Task.Blink", 1000, &ledIndicator, 1, nullptr);
}