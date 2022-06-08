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
            break;
        }
        break;

    case Mode::ForcedGreasing:
        switch (m_state->state)
        {
        case State::Stopped:
            break;
        case State::Greasing:
            break;
        }
        break;
    
    case Mode::Pumping:
        switch (m_state->state)
        {
        case State::Stopped:
            break;
        case State::Greasing:
            break;
        }
        break;
    
    case Mode::Off:
        m_indicator.allOff();
        break;
    }
}
