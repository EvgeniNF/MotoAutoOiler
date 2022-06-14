#include "ButtonService.hpp"


void buttonTaskHandler(void* ptr) noexcept
{
    auto service = reinterpret_cast<ButtonService*>(ptr);
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