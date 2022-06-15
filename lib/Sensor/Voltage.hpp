#pragma once
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <Arduino.h>


struct Message
{
    uint16_t serviceId;
    uint16_t data[5];
    uint16_t size;
};

namespace sensor
{

struct Range
{
    uint16_t minValue;
    uint16_t maxValue;
};    

class Volatage
{
   public: 
    Volatage(uint16_t serviceId, xQueueHandle messageQueue, gpio_num_t gpio, Range range);
    ~Volatage() = default;

   public:
    void run() noexcept; 

   private: 
    bool addValueToBufferFromAnalogPin() noexcept;
    bool bufferIsFull() noexcept;
    uint16_t computeVoltage() noexcept;

   private:
    uint16_t const m_serviceId;
    xQueueHandle m_messageQueue;
    gpio_num_t const m_gpioPin;
    Range const m_rangeValue;
    uint16_t m_actualIndex{0};
    static constexpr uint16_t bufferSize{20};
    uint16_t m_buffer[bufferSize]; 
};

} // namespace sensor
