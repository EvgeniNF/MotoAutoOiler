#include "Speed.hpp"
#include <freertos/task.h>
#include <Arduino.h>
#include <FunctionalInterrupt.h>

namespace sensor
{

Speed::Speed(gpio_num_t gpio, xQueueHandle messageQueue, uint16_t serviceId,
             uint16_t const& numberOfImpulsByResolution, uint16_t const& weelCircle)
: m_serviceId(serviceId), 
  m_messageQueue(messageQueue), 
  m_numberOfImpulsByResolution(&numberOfImpulsByResolution),
  m_weelCircle(&weelCircle)
{
    pinMode(gpio, INPUT_PULLUP);
    attachInterrupt(gpio, std::bind(&Speed::interruptHandler, this), RISING);
}

void Speed::interruptHandler() noexcept
{
    ++m_impulsCounter;
}

uint16_t Speed::computeSpeed(uint16_t distance, uint16_t distanceTime) noexcept 
{
    float speed = (distance / distanceTime) * 3600u;
    return static_cast<uint16_t>(std::round(speed));
}  

uint16_t Speed::computeDistance() noexcept
{
    auto const distanceBetweenImpuls = static_cast<float>(*m_weelCircle / *m_numberOfImpulsByResolution);
    uint16_t distance = (m_impulsCounter * distanceBetweenImpuls) / 1000u;
    return distance;
}

void Speed::run() noexcept
{
    uint32_t previosTime = 0;
    m_impulsCounter = 0;
    while (true)
    {
        uint16_t distance = computeDistance();
        m_impulsCounter = 0;

        uint32_t actualTime = xTaskGetTickCount() / portTICK_PERIOD_MS;
        uint16_t deltaTime = actualTime - previosTime;
        previosTime = actualTime;
        uint16_t speed = computeSpeed(distance, deltaTime);
        
        utils::Message message
        {
            .serviceId = m_serviceId,
            .data = {distance, speed},
            .size = 2
        };

        xQueueSend(m_messageQueue, &message, 0);

        vTaskDelay(1500 / portTICK_PERIOD_MS);
    }
}

}
