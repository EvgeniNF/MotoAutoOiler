#include "ButtonService.hpp"


void buttonTaskHandler(void* ptr) noexcept
{
    auto service = reinterpret_cast<ButtonService*>(ptr);
    constexpr uint32_t holdTimePeriod = 1500;

    while (true)
    {
        bool flag = buttonHandler(service->button);
        if (flag)
        {
            ++service->clickCounter;
        }



        config::Message message
        {
            .serviceId = service->serviceId,
            .message = {service->clickCounter},
            .sizeMessage = 1
        };
        xQueueSend(service->messageQueue, &message, 0);
    }
}

uint32_t computePreviosCallTime() noexcept
{
    static uint32_t previosTime = 0;
    uint32_t actualTime = xTaskGetTickCount() * portTICK_PERIOD_MS;
    uint32_t deltaMs = actualTime - previosTime;
    previosTime = actualTime;
    return deltaMs;
}
