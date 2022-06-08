#pragma once

#include <Indicator.hpp>
#include "config.hpp"
#include <freertos/timers.h>

using LedIndicator = indicator::LedIndicator<config::LED_RED,
                            config::LED_GREAN,
                            config::LED_BLUE, 
                            config::LED_COMMON, 
                            indicator::LedType::CommonCathode>;

enum class Mode
{
    Manual,
    Rain,
    ForcedGreasing,
    Pumping,
    Off
};

enum class State
{
    Stopped,
    Greasing
};

enum class PumpState
{
    On,
    Off,
};

struct DeviceState
{
    Mode mode   { Mode::Off };
    State state { State::Stopped };
    PumpState pumpState { PumpState::Off };
};


using Timer = TimerHandle_t;

class IndicatorTask
{
   public:
    IndicatorTask(DeviceState& state);

    void run();

   private:
    void indicateWrokPumpGreen() const noexcept;

    void indicateWrokPumpBlue() const noexcept;

   private:
    DeviceState* m_state;
    LedIndicator m_indicator;
    Timer m_timer;
};


void indicator_task(void* indicatorTaskPtr) noexcept;




