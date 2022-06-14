#pragma once

#include <PumpControl.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

struct Pumps
{
    Pump pump1;
    Pump pump2;
};

enum class Commands
{
    AllOff   = 0,
    AllOn    = 1,
    OnPump1  = 2,
    OffPump1 = 3,
    OnPump2  = 4,
    OffPump2 = 5, 
};

[[nodiscard]] xTaskHandle createPumpService(gpio_num_t gpioPump1, gpio_num_t gpioPump2) noexcept;

void pumpTaskHendler(void* ptr) noexcept;
