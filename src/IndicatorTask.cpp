#include "../include/IndicatorTask.hpp"

void indicator_task(void* indicatorTaskPtr) noexcept 
{
    auto task = reinterpret_cast<IndicatorTask*>(indicatorTaskPtr);
    
    if (task == nullptr) 
    {
        LOG_ERROR("Indicator.Task", "Task is nullptr");
    }
    else 
    {
        task->run();
    }
}

IndicatorTask::IndicatorTask(DeviceState& state) :
m_state(&state), m_indicator() 
{
    m_timerBlinkBlue = xTimerCreate("Blink.Blue", 500, pdTRUE, &m_indicator, LedIndicator::taskBlinkBlue); 
    m_timerBlinkGreen = xTimerCreate("Blink.Grean", 500, pdTRUE, &m_indicator, LedIndicator::taskBlinkGreen); 
}


void IndicatorTask::run() 
{   
    m_state->changeState.wait();
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
        stopAllTimers();
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
        stopAllTimers();
        break;

    case Mode::ForcedGreasing:
        switch (m_state->state)
        {
        case State::Stopped: 
            xTimerStart(m_timerBlinkGreen, 0);
            break;
        
        case State::Greasing: 
            m_indicator.onBlue();
            stopAllTimers();
            break;
        }
        break;
    
    case Mode::Pumping:
        switch (m_state->state)
        {
        case State::Stopped: 
            xTimerStart(m_timerBlinkBlue, 0);
            break;
        
        case State::Greasing: 
            stopAllTimers();
            m_indicator.onGreen();
            break;
        }
        break;
    
    case Mode::Off:
        m_indicator.allOff();
        stopAllTimers();
        break;
    }
}

void IndicatorTask::stopAllTimers() noexcept 
{
    if (xTimerIsTimerActive(m_timerBlinkBlue)) 
    {
        xTimerStop(m_timerBlinkBlue, 0);
    }
    if (xTimerIsTimerActive(m_timerBlinkBlue)) 
    {
        xTimerStop(m_timerBlinkGreen, 0);
    }
}
