#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../include/config.hpp"
#include <Indicator.hpp>
#include <logs.hpp>
#include "IndicatorTask.hpp"

extern "C" void app_main() 
{
    DeviceState state;
    IndicatorTask task(state);
    xTaskCreate(indicator_task, "Indicator", 1000u, &task, 5, nullptr);
}
