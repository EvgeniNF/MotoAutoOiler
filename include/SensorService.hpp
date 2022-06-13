#pragma once

#include <Sensor.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <config.hpp>
#include <freertos/timers.h>


struct Oiling
{
    uint16_t actualDistancePump1{0};
    uint16_t actualDistancePump2{0};
    uint16_t interval1{0};
    uint16_t interval2{0};
    xTimerHandle onTimerPump1;
    xTimerHandle onTimerPump2;
};


struct SensorService
{
    uint16_t actualDistancePump1{0};
    uint16_t actualDistancePump2{0};
    uint32_t impulsCounter{0};
    config::OilingParameters* sharedData;
    xTimerHandle onTimerPump1;
    xTimerHandle onTimerPump2;
    xTimerHandle offTimerPump1;
    xTimerHandle offTimerPump2;
    xTaskHandle pumpServiceHandle;
};

static void onTimerPump(void* ptr) noexcept;

void oiling(SensorService* data, bool rain) noexcept;

void oilingMode() noexcept;

uint16_t computeRainInterval(uint16_t interval, float rainCoef) noexcept;

static void sensor_service_task(void* ptr) noexcept
{
    auto parameters = reinterpret_cast<SensorService*>(ptr);

    while (true)
    {
        auto distanceBetweenImpls = computeDistanceBetweenImps(parameters->sharedData->numberOfImpulsByResolution,
                                                               parameters->sharedData->weelCircle);
        auto actualDistance = computeDistance(parameters->impulsCounter, distanceBetweenImpls);
        auto timePeriod = computeTimePeriod();
        parameters->sharedData->speed = computeSpeed(actualDistance, timePeriod);
        
        switch (parameters->sharedData->mode)
        {
        case config::Mode::Oiling:
            if (checkSpeed(parameters->sharedData->maxSpeedOiling, 
                           parameters->sharedData->minSpeedOiling,
                           parameters->sharedData->speed))
            {
                oiling(parameters);
            }   
            break;
        
        default:
            break;
        }

    }
}

inline xTaskHandle createSensorService(config::OilingParameters* oilingParameters, xTaskHandle pumpServiceHandle) noexcept
{
    return ;
}
