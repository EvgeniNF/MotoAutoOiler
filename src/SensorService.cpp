#include "SensorService.hpp"

uint16_t computeRainInterval(uint16_t interval, float rainCoef) noexcept
{
    return interval / rainCoef;
}

void oiling(SensorService* data, bool rain) noexcept 
{
    auto interval1 = selectInterval(data->sharedData->speed,
                                    data->sharedData->maxTownSpeed,
                                    data->sharedData->intervalRoadPump1,
                                    data->sharedData->intervalTownPump1);

    auto interval2 = selectInterval(data->sharedData->speed,
                                    data->sharedData->maxTownSpeed,
                                    data->sharedData->intervalRoadPump2,
                                    data->sharedData->intervalTownPump2);

    if (rain)
    {
        interval1 = computeRainInterval(interval1, data->sharedData->rainCoef);
        interval2 = computeRainInterval(interval2, data->sharedData->rainCoef);
    }

    if (data->actualDistancePump1 > interval1)
    {
        if(!xTimerIsTimerActive(data->onTimerPump1))
        {
            xTimerStart(data->onTimerPump1, 0);
        }
        data->actualDistancePump1 = 0;
    }

    if (data->actualDistancePump2 > interval2)
    {
        if(!xTimerIsTimerActive(data->onTimerPump2))
        {
            xTimerStart(data->onTimerPump2, 0);
        }
        data->actualDistancePump2 = 0;
    }
}