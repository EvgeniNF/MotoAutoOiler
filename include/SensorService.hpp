#pragma once

#include <Sensor.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <config.hpp>
#include <freertos/queue.h>

struct SensorService
{
    uint16_t const serviceId;                  /// <<- Unique sevice id 
    gpio_num_t const sensorPin;                /// <<- Sensor gpio num
    const uint16_t*  numberImpulsByResolution; /// <<- Numper of impuls
    const uint16_t*  weelCircle;               /// <<- Moto wheel length
    xQueueHandle messageQueue;                 /// <<- Responce message queue 
    uint32_t interruptCounter;                 /// <<- Counter impuls from sensor
};

/**
 * @brief Create a Sensor Service object
 * 
 * @param parameters      : Oiling parametrs {weelCircle, numberImpulsByResolution, sensorPin} 
 * @param serviceMessanger: Queue for request messages 
 * @param priority        : Task priority
 * @return xTaskHandle    : Created task hendler
 */
[[nodiscard]] xTaskHandle createSensorService(config::OilingParameters const& parameters, 
                                              xQueueHandle serviceMessanger, 
                                              uint16_t priority) noexcept;

/**
 * @brief Handle task compute speed and distance
 * 
 * @param ptr: Parameters 
 */
static void sensorTaskHandler(void* ptr) noexcept;
