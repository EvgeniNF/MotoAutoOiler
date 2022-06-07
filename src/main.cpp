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

enum class Mode 
{
    Off,
    Pumping,
    Mileage_Manual,
    Mileage_Rain
};

enum class PumpState 
{
    On,
    Off
};

struct IndicatorTaskParameters 
{
    LedIndicator indicator;
    Mode mode;
    PumpState pump1;
    PumpState pump2;
};

void indicator_task(void* ptr) 
{
    auto taskParameters = reinterpret_cast<IndicatorTaskParameters*>(ptr);

    while (true)
    {
        switch (taskParameters->mode)
        {
        case Mode::Pumping:
            break;
        case Mode::Mileage_Manual:
            break;
        case Mode::Mileage_Rain:
            break;
        case Mode::Off:
            break;
        }
    }
    

}

extern "C" void app_main() 
{
    IndicatorTaskParameters indicatorTaskParameters;
    xTaskCreate(indicator_task, "Task.Indicator", 1000, &indicatorTaskParameters, 5, nullptr);
}