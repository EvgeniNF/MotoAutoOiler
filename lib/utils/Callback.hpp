#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <functional>
#include <iostream>

namespace utils
{

class Callback 
{
   public:
    Callback() = default;
    virtual ~Callback() = default;

   protected:
    static void timerFunction(void* callbackPtr) noexcept;

    static void taskFunction(void* callbackPtr) noexcept;

    virtual void call() const noexcept = 0;

};

}
