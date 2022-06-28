#include "Pump.hpp"
#include <Arduino.h>
#include <Message.hpp>
#include <iostream>
#include <utility>
#include <asserts.hpp>


namespace device
{

Pump::Pump(uint16_t serviceId, gpio_num_t pin, xQueueHandle messageQueue, uint16_t timeOn, uint16_t timeOff) :
    m_serviceId(serviceId), m_pumpGpio(pin), m_messageQueue(messageQueue) 
{
    m_onTimer = xTimerCreate("PumpOnTimer", timeOn / portTICK_PERIOD_MS, pdFALSE, this, timerOnHandler);
    utils::assert_null(m_onTimer, "Pump on timer was not create");
    
    m_offTimer = xTimerCreate("PumpOffTimer", timeOff / portTICK_PERIOD_MS, pdFALSE, this, timerOffHandler);
    utils::assert_null(m_onTimer, "Pump off timer was not create");

    pinMode(m_pumpGpio, OUTPUT);
    digitalWrite(m_pumpGpio, LOW);
}

void Pump::startPuls(bool onePuls) noexcept
{
    m_state = true;
    m_onePulsFlag = onePuls;
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

    if (xTimerIsTimerActive(m_offTimer))
    {
        xTimerStop(m_offTimer, 0);
    }
}

void Pump::sendMessage() noexcept
{
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
    pump->startCountOffTime();
    pump->sendMessage();
}

void Pump::timerOffHandler(void* pumpPtr) noexcept
{
    auto pump = reinterpret_cast<Pump*>(pvTimerGetTimerID(pumpPtr));
    pump->startPuls(false);
}

void Pump::startCountOffTime() noexcept
{
    if (!m_onePulsFlag)
    {
        xTimerStart(m_offTimer, 0);
    } 
    else 
    {
        m_state = false;
    }
}

}
