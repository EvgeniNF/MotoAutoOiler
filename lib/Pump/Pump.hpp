#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <driver/gpio.h>
#include <freertos/queue.h>
#include <Timer.hpp>


namespace device
{

class Pumps {
   public:
    Pumps(uint16_t id, uint16_t onTime, uint16_t offTime, gpio_num_t pump1, gpio_num_t pump2, xQueueHandle m_messageHandler);
    ~Pumps() = default;

   public:
    void pulsPump1() noexcept;
    void pulsPump2() noexcept;
    void startPumping() noexcept;
    void stop() noexcept;
    bool isRunning() const noexcept;

   private:
    void offPump1() const noexcept;
    void offPump2() const noexcept;

    void callbackOn1() noexcept;
    void callbackOn2() noexcept;
    void callbackAllOn() noexcept;
    void callbackOffAll() noexcept;

   private:
    uint16_t const m_id;
    xQueueHandle m_messageHandler;
    gpio_num_t m_pump1;
    gpio_num_t m_pump2;
    utils::Timer m_timerOn1;
    utils::Timer m_timerOn2;
    utils::Timer m_timerOnAll;
    utils::Timer m_timerOffAll;
    bool m_runningFlag{true};
};

}
