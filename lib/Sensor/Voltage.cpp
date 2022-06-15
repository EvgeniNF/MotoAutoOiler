#include "Voltage.hpp"
#include <utility>
#include <iostream>
namespace sensor
{

Volatage::Volatage(uint16_t serviceId, xQueueHandle messageQueue, gpio_num_t gpio, Range range) :
m_serviceId(serviceId), m_messageQueue(messageQueue), m_gpioPin(gpio), m_rangeValue(std::move(range))
{
    pinMode(m_gpioPin, INPUT);
}

void Volatage::run() noexcept 
{
    while (true)
    {
        if (!addValueToBufferFromAnalogPin())
        {
            uint16_t value = computeVoltage();
            Message message
            {
                .serviceId = m_serviceId,
                .data = {value},
                .size = 1
            };
            if (m_messageQueue != NULL)
            {
                xQueueSend(m_messageQueue, &message, 0);
            }
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}


bool Volatage::bufferIsFull() noexcept
{
    return m_actualIndex >= bufferSize;
}

bool Volatage::addValueToBufferFromAnalogPin() noexcept
{
    if (bufferIsFull())
    {
        m_actualIndex = 0;
        return false;
    }
    m_buffer[m_actualIndex] = analogRead(m_gpioPin);
    ++m_actualIndex;
    return true;
}

uint16_t Volatage::computeVoltage() noexcept
{
    uint32_t avarageValue = 0;
    for (uint16_t i = 0; i < bufferSize; ++i) 
    {
        avarageValue += m_buffer[i];
        m_buffer[i] = 0;
    }

    avarageValue /= bufferSize;
    float normValue = static_cast<float>(avarageValue - 0) / static_cast<float>(4095 - 0);
    float scaleValue = normValue * (m_rangeValue.maxValue - m_rangeValue.minValue) + m_rangeValue.minValue;

    uint16_t voltage = scaleValue;
    return voltage;
}

}

