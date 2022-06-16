#include "Pump.hpp"
#include <Arduino.h>
#include <Message.hpp>
#include <iostream>

namespace device
{

Pump::Pump(uint16_t serviceId, gpio_num_t pin, xQueueHandle messageQueue, uint16_t timeOn) :
    m_serviceId(serviceId), m_pumpGpio(pin), m_messageQueue(messageQueue) 
{
    m_onTimer = xTimerCreate("PumpOnTimer", timeOn / portTICK_PERIOD_MS, pdFALSE, this, timerOnHandler);
    pinMode(m_pumpGpio, OUTPUT);
    digitalWrite(m_pumpGpio, LOW);
}

void Pump::startPuls() noexcept
{
    digitalWrite(m_pumpGpio, HIGH);
    xTimerStart(m_onTimer, 0);
}

void Pump::off() noexcept
{
    digitalWrite(m_pumpGpio, LOW);
    if (xTimerIsTimerActive(m_onTimer))
    {
        xTimerStop(m_onTimer, 0);
    }

    utils::Message message
    {
        .serviceId = m_serviceId,
        .data = {0},
        .size = 1
    };
    xQueueSendFromISR(m_messageQueue, &message, 0);
}

void Pump::changeOnTime(uint16_t timeOn) noexcept
{
    off();
    xTimerChangePeriod(m_onTimer, timeOn / portTICK_PERIOD_MS, 0);
}

void Pump::timerOnHandler(void* pumpPtr) noexcept
{
    auto pump = reinterpret_cast<Pump*>(pvTimerGetTimerID(pumpPtr));
    pump->off();
}

}
