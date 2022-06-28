#include "Timer.hpp"
    
#include "asserts.hpp"

namespace utils
{

Timer::Timer(uint16_t period, std::function<void()> cbFunction, Type type, const char* name) :
m_callback(std::move(cbFunction))
{
    m_timerHandler = xTimerCreate(name, period, type, this, Timer::timerFunction);
    assert_null(m_timerHandler, "Timer was not create!");
}

Timer::~Timer() 
{
    if (xTimerIsTimerActive(m_timerHandler)) 
    {
        xTimerStop(m_timerHandler, 0);
    }
    xTimerDelete(m_timerHandler, 0);
}

void Timer::call() const noexcept
{
    m_callback();
}

void Timer::start() const noexcept
{
    assert_null(m_timerHandler, "Timer was not create!");
    xTimerStart(m_timerHandler, 0);
}

void Timer::stop() const noexcept
{
    if (not xTimerIsTimerActive(m_timerHandler))
    {
        xTimerStop(m_timerHandler, 0);
    }
}

void Timer::setNewPeriod(uint16_t newPeriod) noexcept 
{
    if (xTimerIsTimerActive(m_timerHandler))
    {
        xTimerStop(m_timerHandler, 0);
    }
    xTimerChangePeriod(m_timerHandler, newPeriod, 0);
}

}
