#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/timers.h>


class Button
{
   public: 
    Button(uint16_t serviceId, xQueueHandle messageQueue, gpio_num_t gpio);
    ~Button() = default;

   public:
    void run() noexcept; 

    void checkClick() noexcept;

    void checkHold() noexcept;

   private: 
    void interruptHandler() noexcept;

    static void timerClickHandler(void* buttonPtr) noexcept;
    
    static void timerHoldHandler(void* buttonPtr) noexcept;

    void stopAllTimers() noexcept;

    void stopHoldTimer() noexcept;

   private:
    uint16_t const m_serviceId;
    xQueueHandle m_messageQueue;
    gpio_num_t const m_buttonPin;
    xSemaphoreHandle m_interruptFlag;
    uint16_t m_clickCounter = 0;
    xTimerHandle m_timerClick;
    xTimerHandle m_timerHold;
    bool m_holdFlag = false;
};
