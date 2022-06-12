#include "PumpControl.hpp"

#include <MessageQueue.hpp>

namespace pump_control
{
    
PumpControl::PumpControl(gpio_num_t gpio, uint32_t onTime, uint32_t offTime) : m_gpio(gpio)
{
    LOG_INFO("PumpControl", "GPIO num" << gpio);
    gpio_pad_select_gpio(m_gpio);
    auto error = gpio_set_direction(m_gpio, GPIO_MODE_OUTPUT);
    if (error != ESP_OK)
    {
        LOG_ERROR("Pump.Control", "Set gpio output mode error: " << esp_err_to_name(error));
        vTaskDelay(portMAX_DELAY);
    }
    setLevelGpio(false);
    

    m_timerOn = xTimerCreate("On", onTime / portTICK_PERIOD_MS, pdFALSE, this, PumpControl::offTimerHendler);
    if (m_timerOn == NULL)
    {
        LOG_ERROR("Pump.Control", "Create timer on");
    }
    m_timerOff = xTimerCreate("Off", offTime / portTICK_PERIOD_MS, pdFALSE, this, PumpControl::onTimerHendler);   
    if (m_timerOff == NULL)
    {
        LOG_ERROR("Pump.Control", "Create timer off");
    }
}


void PumpControl::setLevelGpio(bool level) const noexcept 
{
    LOG_INFO("Pump.Control", "Set gpio level: " << std::boolalpha << level);
    auto error = gpio_set_level(m_gpio, level);
    if (error != ESP_OK)
    {
        gpio_set_level(m_gpio, pdFALSE);
        LOG_ERROR("Pump.Control", "Set gpio level error: " << esp_err_to_name(error));
        vTaskDelay(portMAX_DELAY);
    }
}

void PumpControl::startOnPump() noexcept 
{
    if (m_actualNumOfImpuls >= m_numOfImpuls)
    {
        m_actualNumOfImpuls = 0;
        utils::Message message{-1, 1};
        auto serviceMessanger = utils::getInstanceQueue();
        serviceMessanger->sendMessage(message);
        setLevelGpio(false);
        return;
    } 
    else 
    {
        ++m_actualNumOfImpuls;
    }
    setLevelGpio(true);

    auto resultSatrtTimer = xTimerStart(m_timerOn, 0);
    if (resultSatrtTimer == pdFAIL)
    {
        LOG_ERROR("Pump.Control", "Start timer on " << resultSatrtTimer);
        setLevelGpio(false);
        vTaskDelay(portMAX_DELAY);
    }
}

void PumpControl::startOffPump() noexcept
{
    setLevelGpio(false);
    auto resultSatrtTimer = xTimerStart(m_timerOff, 0);
    if (resultSatrtTimer == pdFAIL)
    {
        LOG_ERROR("Pump.Control", "Start timer on " << resultSatrtTimer);
        setLevelGpio(false);
        vTaskDelay(portMAX_DELAY);
    }
}

void PumpControl::onTimerHendler(void* ptr) noexcept
{
    auto pump = reinterpret_cast<PumpControl*>(pvTimerGetTimerID(ptr));
    pump->startOnPump();
}

void PumpControl::offTimerHendler(void* ptr) noexcept
{
    auto pump = reinterpret_cast<PumpControl*>(pvTimerGetTimerID(ptr));
    pump->startOffPump();
}

void PumpControl::startPumping(uint16_t numOfImpuls) noexcept
{
    if (xTimerIsTimerActive(m_timerOn)) xTimerStop(m_timerOn, 0);
    if (xTimerIsTimerActive(m_timerOff)) xTimerStop(m_timerOff, 0);
    LOG_INFO("PUMP.Control", "Start pumping: " << numOfImpuls);
    m_actualNumOfImpuls = 0;
    m_numOfImpuls = numOfImpuls;
    startOnPump();
}

void PumpControl::resumePumping() noexcept
{
    if (xTimerIsTimerActive(m_timerOn)) xTimerStop(m_timerOn, 0);
    if (xTimerIsTimerActive(m_timerOff)) xTimerStop(m_timerOff, 0);
    LOG_INFO("PUMP.Control", "Resume pumping");
    startOnPump();
}

void PumpControl::stopPumping() noexcept
{
    if (xTimerIsTimerActive(m_timerOn)) xTimerStop(m_timerOn, 0);
    if (xTimerIsTimerActive(m_timerOff)) xTimerStop(m_timerOff, 0);
    LOG_INFO("PUMP.Control", "Stop pumping");
    setLevelGpio(false);
}

} // namespace pump_control
