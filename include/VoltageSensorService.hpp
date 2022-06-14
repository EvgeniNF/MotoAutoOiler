#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/adc.h>
#include <freertos/queue.h>

struct RangeVolatage
{
    float minValue;            /// <<- Min value voltage
    float maxValue;            /// <<- Max value voltage
};

struct VolategeSensorService
{
    uint16_t const serviceId;  /// <<- Unique sevice id 
    xQueueHandle messageQueue; /// <<- Responce message queue 
    RangeVolatage range;       /// <<- Range voltage 
    adc1_channel_t channel;    /// <<- Adc channel
};

/**
 * @brief Create a Voltage Sensor Service object
 * 
 * @param analogPin       : Gpio pin with adc
 * @param serviceMessanger: Message queue
 * @param priority        : Task priority 
 * @param range           : Range input voltage
 * @return xTaskHandle    : Service voltage sensor
 */
[[nodiscard]] xTaskHandle createVoltageSensorService(gpio_num_t analogPin, 
                                                     xQueueHandle serviceMessanger,
                                                     uint16_t priority,
                                                     RangeVolatage range) noexcept;

/**
 * @brief Volatage sensor service
 * 
 * @param ptr: VolatgeSensorService parameters pointer
 */
void voltageSensorTaskHandler(void* ptr) noexcept;

/**
 * @brief Select channel from gpio
 * 
 * @param gpio           : Input gpio
 * @param selectedChannel: Ouotput channel
 * @return true          : Channel is selected
 * @return false         : Channel not found 
 */
bool selectChannel(gpio_num_t gpio, adc1_channel_t& selectedChannel) noexcept;

/**
 * @brief Normalization value from 0 ... 1100 to 0.0 ... 1.0
 * 
 * @param value  : Input value
 * @return float : Normalize value 
 */
float norm(uint32_t value) noexcept;

/**
 * @brief Scale value 
 * 
 * @param value  : Inpute normalize value
 * @param range  : Output range
 * @return float : Computed value
 */
float scale(float value, RangeVolatage const& range) noexcept;
