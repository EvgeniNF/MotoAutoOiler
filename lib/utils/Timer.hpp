#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include "Callback.hpp"
#include <functional>

namespace utils
{

class Timer : protected Callback
{
   public:
    enum Type : UBaseType_t { one_shot, reloaded};

   public: 
    Timer(uint16_t period, std::function<void()> cbFunction, Type type=Type::one_shot, const char* name="");
    
    ~Timer();
    
   public: 
    void start() const noexcept;
    
    void stop() const noexcept;

    void setNewPeriod(uint16_t newPeriod) noexcept;

   protected:
    void call() const noexcept final; 

   private:
    std::function<void()> m_callback;
    xTimerHandle m_timerHandler; 
};

} // namespace utils
