#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

struct Button
{
    gpio_num_t pin;
    xSemaphoreHandle hendler;
};

void IRAM_ATTR buttonInterruptHandler(void* ptr) noexcept;

void initButton(Button& button) noexcept;

void waitInterrupt(Button& button) noexcept;

bool buttonHandler(Button& button) noexcept;
