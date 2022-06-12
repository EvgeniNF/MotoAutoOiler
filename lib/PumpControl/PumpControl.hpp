#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <driver/gpio.h>
#include <logs.hpp>
#include <freertos/semphr.h>

namespace pump_control
{

    class PumpControl
    {
       public:
        PumpControl(gpio_num_t gpio, uint32_t onTime, uint32_t offTime);

        static void onTimerHendler(void* ptr) noexcept;

        static void offTimerHendler(void* ptr) noexcept;

        void startPumping(uint16_t numOfImpuls) noexcept;

        void resumePumping() noexcept;

        void stopPumping() noexcept;

        void startOnPump() noexcept;

        void startOffPump() noexcept;

        void setNewParameters(uint16_t impls, uint32_t interval, uint32_t duration) noexcept
        {
            if (xTimerIsTimerActive(m_timerOn)) 
            {
                xTimerStop(m_timerOn, 0);
                xTimerChangePeriod(m_timerOn, duration / portTICK_PERIOD_MS, 0);
                xTimerStart(m_timerOn, 0);
            }
            else 
            {
                xTimerChangePeriod(m_timerOn, duration / portTICK_PERIOD_MS, 0);
            }

            if (xTimerIsTimerActive(m_timerOff)) 
            {
                xTimerStop(m_timerOff, 0);
                xTimerChangePeriod(m_timerOff, interval / portTICK_PERIOD_MS, 0);
                xTimerStart(m_timerOff, 0);
            }
            else 
            {
                xTimerChangePeriod(m_timerOff, interval / portTICK_PERIOD_MS, 0);
            }
            
            m_numOfImpuls = impls;
        }

       private:
        void setLevelGpio(bool level) const noexcept;
       private:
        gpio_num_t const m_gpio;
        uint16_t m_numOfImpuls;
        uint16_t m_actualNumOfImpuls = 0;
        xTimerHandle m_timerOn;
        xTimerHandle m_timerOff;
    };

}
