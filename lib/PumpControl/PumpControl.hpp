#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>


struct Pump
{
    uint16_t pumpId;
    gpio_num_t pumpPin;
};

void initPump(Pump const& gpioPin) noexcept;

void setLevelPump(Pump const& gpioPin, bool level) noexcept;

