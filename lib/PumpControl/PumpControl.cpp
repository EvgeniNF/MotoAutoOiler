#include "PumpControl.hpp"

void initPump(Pump const& pump) noexcept
{
    gpio_pad_select_gpio(pump.pumpPin);

    auto errorCode = gpio_set_direction(pump.pumpPin, GPIO_MODE_OUTPUT);
    configASSERT(errorCode == ESP_OK);

    errorCode = gpio_set_level(pump.pumpPin, pdFALSE);
    configASSERT(errorCode == ESP_OK);
}

void setLevelPump(Pump const& pump, bool level) noexcept
{
    gpio_set_level(pump.pumpPin, level);
}
