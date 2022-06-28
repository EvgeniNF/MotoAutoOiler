#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <iostream>

namespace 
{
    constexpr char error[]   = "[ERROR]";
    constexpr char warning[] = "[WARNING]";
    constexpr char info[]    = "[INFO]";
}  

#define LOG_ERROR(logger, message)                  \
    std::cout << "["  << xTaskGetTickCount() << "]" \
              << error                              \
              << "{" << message << "}" << std::endl

#define LOG_WARNING(logger, message)                \
    std::cout << "["  << xTaskGetTickCount() << "]" \
              << warning                            \
              << "{" << message << "}" << std::endl

#define LOG_INFO(logger, message)                   \
    std::cout << "["  << xTaskGetTickCount() << "]" \
              << info                               \
              << "{" << message << "}" << std::endl
