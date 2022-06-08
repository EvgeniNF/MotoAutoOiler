#pragma once

#include <freertos/FreeRTOS.h>
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
    constexpr bool Low() const noexcept 
    {
        return LOW<ledType>::value;
    }

    constexpr bool Hight() const noexcept
    {
        return HIGH<ledType>::value;
    }

};

using GpioPin = gpio_num_t;

template<GpioPin redPin, GpioPin greenPin,
         GpioPin bluePin, GpioPin commonPin, 
         LedType ledType>
class LedIndicator 
{
    public:
     
     LedIndicator() 
     {
         gpio_pad_select_gpio(redPin);
         gpio_set_direction(redPin, GPIO_MODE_INPUT_OUTPUT);

         gpio_pad_select_gpio(greenPin);
         gpio_set_direction(greenPin, GPIO_MODE_INPUT_OUTPUT);
        
         gpio_pad_select_gpio(bluePin);
         gpio_set_direction(bluePin, GPIO_MODE_INPUT_OUTPUT);
        
         gpio_pad_select_gpio(commonPin);
         gpio_set_direction(commonPin, GPIO_MODE_INPUT_OUTPUT);

         gpio_set_level(redPin, m_level.Low());
         gpio_set_level(greenPin, m_level.Low());
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
         gpio_set_level(greenPin, m_level.Low());
         gpio_set_level(bluePin, m_level.Hight());
         LOG_INFO("Led.Indicator", "On blue led");
     }
     
     void onGreen() const noexcept 
     {
         gpio_set_level(redPin, m_level.Low());
         gpio_set_level(greenPin, m_level.Hight());
         gpio_set_level(bluePin, m_level.Low());
         LOG_INFO("Led.Indicator", "On gren led");
     }
     
     void onRed() const noexcept 
     {
         gpio_set_level(redPin, m_level.Hight());
         gpio_set_level(greenPin, m_level.Low());
         gpio_set_level(bluePin, m_level.Low());
         LOG_INFO("Led.Indicator", "On red led");
     }

     void allOff() const noexcept 
     {
         gpio_set_level(redPin, m_level.Low());
         gpio_set_level(greenPin, m_level.Low());
         gpio_set_level(bluePin, m_level.Low());
         LOG_INFO("Led.Indicator", "Off all");
     }
     
     void blinkBlue() const noexcept
     {
         if (gpio_get_level(bluePin) == m_level.Low()) 
         {
             onBlue();
         } 
         else 
         {
             allOff();
         }
     }

     void blinkRed() const noexcept
     {
         if (gpio_get_level(redPin) == m_level.Low()) 
         {
             onRed();
         } 
         else 
         {
             allOff();
         }
     }
     
     void blinkGreen() const noexcept
     {
         if (gpio_get_level(greenPin) == m_level.Low()) 
         {
             onGreen();
         } 
         else 
         {
             allOff();
         }
     }

     static void taskBlinkBlue(void* indicator) noexcept 
     {
         auto object = reinterpret_cast<LedIndicator*>(indicator);
         object->blinkBlue();
     }

     static void taskBlinkRed(void* indicator) noexcept 
     {
         auto object = reinterpret_cast<LedIndicator*>(indicator);
         object->blinkRed();
     }

     static void taskBlinkGreen(void* indicator) noexcept 
     {
         auto object = reinterpret_cast<LedIndicator*>(indicator);
         object->blinkGreen();
     }

    private:
      Level<ledType> m_level;
};

}
