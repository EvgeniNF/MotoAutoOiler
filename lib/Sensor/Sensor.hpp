#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

/**
 * @brief Sensor gpio interrupt handler for incriment sensor counter
 * 
 * @param ptr: Sensor conuter ptr uint32_t 
 */
static void IRAM_ATTR interruptCallback(void* ptr) noexcept;

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

/**
 * @brief Сhecking for speed in range
 * 
 * @param maxSpeed   : Max speed
 * @param minSpeed   : Min speed   
 * @param actualSpeed: Tested speed
 * @return true      : In range
 * @return false     : Not in range
 */
[[nodiscard]] bool checkSpeed(uint16_t maxSpeed, uint16_t minSpeed, uint16_t actualSpeed) noexcept;

/**
 * @brief Select interval by thresh value: 
 *        speed <= threshValue = interval1
 *        speed > threshValue = interval2
 * @param value      
 * @param threshValue 
 * @param interval1  
 * @param interval2  
 * @return uint16_t interval1 or interval2
 */
[[nodiscard]] uint16_t selectInterval(uint16_t value, uint16_t threshValue, uint16_t interval1, uint16_t interval2) noexcept;

[[nodiscard]] uint16_t computeDistanceBetweenImps(uint16_t numOfImpulsbyResulution, uint16_t wheelDistance) noexcept;

[[nodiscard]] uint32_t computeTimePeriod() noexcept;