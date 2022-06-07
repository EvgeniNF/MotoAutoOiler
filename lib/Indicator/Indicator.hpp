#pragma once

#include <driver/gpio.h>
#include <logs.hpp>

namespace indicator
{

enum class LedType 
{
    CommonAnode,
    CommonCathode
};


template<LedType ledType>
struct HIGH
{
    static constexpr bool value = LedType::CommonCathode == ledType;
};

template<LedType ledType>
struct LOW
{
    static constexpr bool value = LedType::CommonCathode != ledType;
};

template<LedType ledType>
struct Level 
{
    constexpr bool Low() 
    {
        return LOW<ledType>::value;
    }

    constexpr bool Hight() 
    {
        return HIGH<ledType>::value;
    }

};

using GpioPin = gpio_num_t;

template<GpioPin redPin, GpioPin greanPin,
         GpioPin bluePin, GpioPin commonPin, 
         LedType ledType>
class LedIndicator 
{
    public:
     
     LedIndicator() 
     {
        gpio_pad_select_gpio(redPin);
        gpio_set_direction(redPin, GPIO_MODE_INPUT_OUTPUT);

        gpio_pad_select_gpio(greanPin);
        gpio_set_direction(greanPin, GPIO_MODE_INPUT_OUTPUT);
        
        gpio_pad_select_gpio(bluePin);
        gpio_set_direction(bluePin, GPIO_MODE_INPUT_OUTPUT);
        
        gpio_pad_select_gpio(commonPin);
        gpio_set_direction(commonPin, GPIO_MODE_INPUT_OUTPUT);

        gpio_set_level(redPin, m_level.Low());
        gpio_set_level(greanPin, m_level.Low());
        gpio_set_level(bluePin, m_level.Low());
        gpio_set_level(commonPin, m_level.Low());
     };

     LedIndicator(LedIndicator&) = default;
     LedIndicator(LedIndicator&&) noexcept = default;
     LedIndicator& operator=(LedIndicator const&) = default;
     ~LedIndicator() = default;

    public:
     void onBlue() const noexcept 
     {
        gpio_set_level(redPin, m_level.Low());
        gpio_set_level(greanPin, m_level.Low());
        gpio_set_level(bluePin, m_level.Hight());
        LOG_INFO("Led.Indicator", "On blue led");
     }
     
     void onGrean() const noexcept 
     {
        gpio_set_level(redPin, m_level.Low());
        gpio_set_level(greanPin, m_level.Hight());
        gpio_set_level(bluePin, m_level.Low());
        LOG_INFO("Led.Indicator", "On gren led");
     }
     
     void onRed() const noexcept 
     {
        gpio_set_level(redPin, m_level.Hight());
        gpio_set_level(greanPin, m_level.Low());
        gpio_set_level(bluePin, m_level.Low());
        LOG_INFO("Led.Indicator", "On red led");
     }

     void allOff() const noexcept 
     {
        gpio_set_level(redPin, m_level.Low());
        gpio_set_level(greanPin, m_level.Low());
        gpio_set_level(bluePin, m_level.Low());
        LOG_INFO("Led.Indicator", "Off all");
     }

    private:
      Level<ledType> m_level;

};

}

