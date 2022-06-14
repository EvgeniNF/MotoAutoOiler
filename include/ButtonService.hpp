#pragma once

#include <Button.hpp>
#include "config.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/timers.h>

struct ButtonService
{
    uint16_t const serviceId;                  /// <<- Unique sevice id 
    Button button;                             /// <<- Sensor gpio num
    xQueueHandle messageQueue;                 /// <<- Responce message queue
    uint16_t clickCounter;                     /// <<- 
    xTimerHandle doubleClickTimer;             ///
};



void buttonTaskHandler(void* ptr) noexcept;

