#pragma once

#include <driver/gpio.h>

namespace config 
{

using GpioPin = gpio_num_t;

/// -------------------------- Usage outputs pins -------------------------- ///

static constexpr GpioPin PUMP_1 = GPIO_NUM_0;  /// <<- Oil pump number 1
static constexpr GpioPin PUMP_2 = GPIO_NUM_0;  /// <<- Oil pump number 2

static constexpr GpioPin LED_RED =    GPIO_NUM_0; /// <<- Led output red
static constexpr GpioPin LED_GREAN =  GPIO_NUM_0; /// <<- Led output grean
static constexpr GpioPin LED_BLUE =   GPIO_NUM_0; /// <<- Led output blue
static constexpr GpioPin LED_COMMON = GPIO_NUM_0; /// <<- Led output common

/// ----------------------------------------------------------------------- ///

/// -------------------------- Usage inputs pins -------------------------- ///

static constexpr GpioPin SPEED_SENSOR   = GPIO_NUM_0; /// <<- Speed sensor
static constexpr GpioPin BUTTON         = GPIO_NUM_0; /// <<- Button
static constexpr GpioPin TEMP_SENSOR    = GPIO_NUM_0; /// <<- Temperature sensor (optional)
static constexpr GpioPin VOLTAGE_SENSOR = GPIO_NUM_0; /// <<- Analog voltage sensor

/// ----------------------------------------------------------------------- ///

}

