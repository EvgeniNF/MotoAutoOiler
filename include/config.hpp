#pragma once

#include <driver/gpio.h>

#define DEBUG

namespace config 
{

using GpioPin = gpio_num_t;

/// -------------------------- Usage outputs pins -------------------------- ///

static constexpr GpioPin PUMP_1 = GPIO_NUM_0;  /// <<- Oil pump number 1
static constexpr GpioPin PUMP_2 = GPIO_NUM_0;  /// <<- Oil pump number 2

static constexpr GpioPin LED_RED =    GPIO_NUM_25; /// <<- Led output red
static constexpr GpioPin LED_GREAN =  GPIO_NUM_26; /// <<- Led output grean
static constexpr GpioPin LED_BLUE =   GPIO_NUM_27; /// <<- Led output blue
static constexpr GpioPin LED_COMMON = GPIO_NUM_14; /// <<- Led output common

/// ----------------------------------------------------------------------- ///

/// -------------------------- Usage inputs pins -------------------------- ///

static constexpr GpioPin BUTTON         = GPIO_NUM_0; /// <<- Button
static constexpr GpioPin TEMP_SENSOR    = GPIO_NUM_0; /// <<- Temperature sensor (optional)
static constexpr GpioPin VOLTAGE_SENSOR = GPIO_NUM_0; /// <<- Analog voltage sensor

/// ----------------------------------------------------------------------- ///

struct PumpingParameters
{
    
};

struct PumpIntervals
{
    
};

struct SpeedLimits
{
    
};

struct SensorParameters
{
    
};

enum class Mode
{
    Off,
    PumpingOn,
    PumpingOff,
    Oiling,
    OilingRain,
    ForsedPumpingOn,
    ForsedPumpingOff
};

struct OilingParameters
{
    Mode mode{Mode::Off};
    uint16_t speed{0};
    uint16_t numberOfImpulsByResolution{12};            // Чисто импульсов на оборот колеса
    uint16_t weelCircle{2040};                          // Длина окружности колеса
    static constexpr GpioPin SPEED_SENSOR = GPIO_NUM_0; // <<- Speed sensor
    
    uint16_t maxTownSpeed{80};               // Скорость смены режима с Города на Трассу
    uint16_t minSpeedOiling{20};             // Минимальная скорость смазки
    uint16_t maxSpeedOiling{180};            // Максимальная скорость смазки
    
    uint16_t intervalTownPump1 {1400};    // Интервал смазки в режиме Город по пробегу в м для насоса 1
    uint16_t intervalTownPump2 {1400};    // Интервал смазки в режиме Город по пробегу в м для насоса 2
    uint16_t intervalRoadPump1 {1700};    // Интервал смазки в режиме Трасса по пробегу в м для насоса 1
    uint16_t intervalRoadPump2 {1700};    // Интервал смазки в режиме Трасса по пробегу в м для насоса 2
    float rainCoef{0.0};                     // Коэф. увеличения частоты смазки в дождь 
    
    uint16_t pumpingImpulsInterval{1000}; // Интервал между импульсами при прокачке
    uint16_t durationImpuls{500};         // Длительность импульса во всех режимах
    uint16_t numberOfImpulsInPumping{60}; // Количество импульсов за цикл прокачки
};

struct Message
{
    uint16_t serviceId;   /// <<-
    uint16_t message[5];   /// <<-
    uint16_t sizeMessage; 
};

}

