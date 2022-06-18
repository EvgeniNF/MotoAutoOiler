#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <driver/gpio.h>
#include <freertos/queue.h>

namespace device
{

class Pump
{
   public:
    Pump(uint16_t serviceId, gpio_num_t pin, xQueueHandle messageQueue, uint16_t timeOn);
    ~Pump() = default;

   public:
    void startPuls(bool onePuls=true) noexcept;

    void changeOnTime(uint16_t timeOn) noexcept;

    void changeOffTime(uint16_t timeOn) noexcept;

    void startCountOffTime() noexcept;

    void off() noexcept;

   private:
    static void timerOnHandler(void* pumpPtr) noexcept;

    static void timerOffHandler(void* pumpPtr) noexcept;

   private: 
    uint16_t const m_serviceId;
    gpio_num_t const m_pumpGpio;
    xQueueHandle m_messageQueue;
    xTimerHandle m_onTimer;
    xTimerHandle m_offTimer;
    bool m_onePulsFlag = false;
};

}
