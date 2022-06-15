#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <Message.hpp>


namespace sensor
{

class Speed
{
   public:
    Speed(gpio_num_t gpio, xQueueHandle messageQueue, uint16_t serviceId,
          uint16_t const& numberOfImpulsByResolution, uint16_t const& weelCircle);
    ~Speed() = default;
    
   public:
    void run() noexcept;

   private:
    void interruptHandler() noexcept;

    static uint16_t computeSpeed(uint16_t distance, uint16_t distanceTime) noexcept;

    uint16_t computeDistance() noexcept;

   private:
    uint16_t const m_serviceId;
    xQueueHandle m_messageQueue;
    const uint16_t* m_numberOfImpulsByResolution;
    const uint16_t* m_weelCircle;
    uint16_t m_impulsCounter = 0;
};

}
