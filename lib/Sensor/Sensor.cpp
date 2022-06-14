#include "Sensor.hpp"
#include <esp_intr_alloc.h>
#include <cmath>
#include <freertos/task.h>

void IRAM_ATTR interruptCallback(void* ptr) noexcept 
{
    auto counter = reinterpret_cast<uint32_t*>(ptr);
    ++(*counter);
}

void initSensor(gpio_num_t gpioSensor, uint32_t* interruptCounter) noexcept 
{
    gpio_pad_select_gpio(gpioSensor);
    auto errorCode = gpio_set_direction(gpioSensor, GPIO_MODE_INPUT);
    configASSERT(errorCode == ESP_OK);
    gpio_set_pull_mode(gpioSensor, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(gpioSensor, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL2);
    gpio_isr_handler_add(gpioSensor, interruptCallback, interruptCounter);
    gpio_intr_enable(gpioSensor);
}

uint16_t computeDistance(uint16_t numOfImpuls, uint16_t distanceBetweenImpuls) noexcept
{
    uint16_t distance = (numOfImpuls * distanceBetweenImpuls) / 1000u;
    return distance;
}

uint16_t computeSpeed(uint16_t distance, uint16_t distanceTime) noexcept 
{
    float speed = (distance / distanceTime) * 3600u;
    return static_cast<uint16_t>(std::round(speed));
}
