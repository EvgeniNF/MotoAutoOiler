#pragma once
#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <freertos/queue.h>
#include <Interrupts.hpp>
#include <freertos/timers.h>

namespace button
{

template<gpio_num_t gpio>
class Button
{
    public:
    Button() 
    {
        constexpr uint32_t debounceTime = 500 / portTICK_RATE_MS;
        m_debounceTimer = xTimerCreate("Timer.Debounce", debounceTime, false, NULL, Button::debounce);
        addInterrupt<gpio, GPIO_PULLUP_ONLY, GPIO_INTR_NEGEDGE>();
        hendlers[static_cast<uint32_t>(gpio)] = [this](uint32_t){
            handler();
        };
    }

    static void debounce(void*)
    {
        if (gpio_get_level(gpio) == 1)
        {

        }
        gpio_intr_enable(gpio);
    }

    void resetCounter()
    {
        numOfClicks = 0;
        LOG_INFO("RESET", numOfClicks);
    }

    void handler() 
    {
       gpio_intr_disable(gpio);
       if (xTimerIsTimerActive(m_debounceTimer)) {
           return;
       }
       xTimerStart(m_debounceTimer, 0);
    }

    uint16_t getNumClicks(){
        return numOfClicks;
    }

    private:
    uint16_t numOfClicks = 0;
    xTimerHandle m_debounceTimer;
};

}
