#include "../include/IndicatorTask.hpp"

void indicator_task(void* indicatorTaskPtr) noexcept 
{
    
}


void IndicatorTask::run() 
{   
    switch (m_state->mode)
    {
    case Mode::Manual:
        switch (m_state->state)
        {
        case State::Stopped:
            m_indicator.onGreen();
            break;
        
        case State::Greasing:
            m_indicator.onBlue();
            break;
        }
        break;

    case Mode::Rain:
        switch (m_state->state)
        {
        case State::Stopped:
            m_indicator.onBlue();
            break;

        case State::Greasing:
            m_indicator.onGreen();
            break;
        }
        break;

    case Mode::ForcedGreasing:
        switch (m_state->state)
        {
        case State::Stopped: /// blink green
            break;
        
        case State::Greasing: /// indicate work pump
            m_indicator.onBlue();
            break;
        }
        break;
    
    case Mode::Pumping:
        switch (m_state->state)
        {
        case State::Stopped: /// blink blue
            break;
        
        case State::Greasing: /// indicate work pump
            m_indicator.onGreen();
            break;
        }
        break;
    
    case Mode::Off:
        m_indicator.allOff();
        break;
    }
}

void IndicatorTask::indicateWrokPumpGreen() const noexcept 
{
    switch (m_state->pumpState) 
    {
        case PumpState::On:
            m_indicator.onGreen();
            break;
        case PumpState::Off:
            m_indicator.onBlue();
            break;
    }
}

void IndicatorTask::indicateWrokPumpBlue() const noexcept 
{
    switch (m_state->pumpState) 
    {
        case PumpState::On:
            m_indicator.onBlue();
            break;
        case PumpState::Off:
            m_indicator.onGreen();
            break;
    }
}
