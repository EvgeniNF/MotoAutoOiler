#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

/**
 * @brief Sensor gpio interrupt handler for incriment sensor counter
 * 
 * @param ptr: Sensor conuter ptr uint32_t 
 */
void IRAM_ATTR interruptCallback(void* ptr) noexcept;

/**
 * @brief Initialisation pin and interrupt sensor pin
 * 
 * @param gpioSensor: Input pin with interrupt 
 */
void initSensor(gpio_num_t gpioSensor, uint32_t* interruptCounter) noexcept;

/**
 * @brief Computing distance from counted impuls
 * 
 * @param numOfImpuls          : Impuls from sensor
 * @param distanceBetweenImpuls: Distance between impuls in mm
 * @param timePeriod           : Time period in ms
 * @return uint16_t            : Distance in m 
 */
[[nodiscard]] uint16_t computeDistance(uint16_t numOfImpuls, uint16_t distanceBetweenImpuls) noexcept;

/**
 * @brief Compute speed from distance and time
 * 
 * @param distance    : Distance in m 
 * @param distanceTime: Time in ms 
 * @return uint16_t   : Speed in km/h
 */
[[nodiscard]] uint16_t computeSpeed(uint16_t distance, uint16_t distanceTime) noexcept;
