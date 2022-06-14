#include "VoltageSensorService.hpp"
#include <esp_adc_cal.h>
#include <config.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

xTaskHandle createVoltageSensorService(gpio_num_t analogPin, 
                                       xQueueHandle serviceMessanger,
                                       uint16_t priority,
                                       RangeVolatage range) noexcept
{
    static VolategeSensorService parmaters
    {
        .serviceId = 2,
        .messageQueue = serviceMessanger,
        .range = range,
        .channel = ADC1_CHANNEL_MAX
    };

    if (!selectChannel(analogPin, parmaters.channel))
    {
        return NULL;
    }
    xTaskHandle taskHandler;
    xTaskCreate(voltageSensorTaskHandler, "Task.Voltage.Sensor", 2048, &parmaters, priority, &taskHandler);
    return taskHandler;
}

bool selectChannel(gpio_num_t gpio, adc1_channel_t& selectedChannel) noexcept
{
    switch (gpio)
    {
    case GPIO_NUM_36:
        selectedChannel = ADC1_CHANNEL_0;
        break;
    case GPIO_NUM_37:
        selectedChannel = ADC1_CHANNEL_1;
        break;
    case GPIO_NUM_38:
        selectedChannel = ADC1_CHANNEL_2;
        break;
    case GPIO_NUM_39:
        selectedChannel = ADC1_CHANNEL_3;
        break;
    case GPIO_NUM_32:
        selectedChannel = ADC1_CHANNEL_4;
        break;
    case GPIO_NUM_33:
        selectedChannel = ADC1_CHANNEL_5;
        break;
    case GPIO_NUM_34:
        selectedChannel = ADC1_CHANNEL_6;
        break;
    case GPIO_NUM_35:
        selectedChannel = ADC1_CHANNEL_7;
        break;
    default:
        return false;
    }
    return true;
}

void voltageSensorTaskHandler(void* ptr) noexcept
{
    auto parameters = reinterpret_cast<VolategeSensorService*>(ptr);
    adc1_config_channel_atten(parameters->channel, ADC_ATTEN_DB_11);
    esp_adc_cal_characteristics_t adcChars;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adcChars);
    adc1_config_width(ADC_WIDTH_BIT_12);

    constexpr uint16_t updatePeriod = 3000u;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    constexpr TickType_t tickUpdatePeriod = updatePeriod / portTICK_PERIOD_MS;

    while (true)
    {
        uint32_t adcVolatage = 0;
        constexpr uint16_t numOfSamples = 20;
        for (uint16_t i = 0; i < numOfSamples; ++i)
        {
            adcVolatage += esp_adc_cal_raw_to_voltage(adc1_get_raw(parameters->channel), &adcChars);
        }
        adcVolatage /= numOfSamples;
        float normValue = norm(adcVolatage);
        float actualVoltage = scale(normValue, parameters->range);
        
        config::Message message
        {
            .serviceId = parameters->serviceId,
            .message = {static_cast<uint16_t>(actualVoltage * 10)},
            .sizeMessage = 1
        };

        xQueueSend(parameters->messageQueue, &message, 0);
        vTaskDelayUntil(&xLastWakeTime, tickUpdatePeriod);
    }
}

float norm(uint32_t value) noexcept
{
    constexpr uint16_t minValue = 0;
    constexpr uint16_t maxValue = 1100;
    float normValue = (value - minValue) / (maxValue - minValue);
    return normValue;
}

float scale(float value, RangeVolatage const& range) noexcept
{
    return value * (range.maxValue - range.minValue) + range.minValue;
}
