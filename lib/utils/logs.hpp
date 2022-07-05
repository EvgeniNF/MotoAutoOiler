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

#ifdef DEBUG

#define LOG_ERROR(logger, message)                  \
    std::cout << "["  << xTaskGetTickCount() << "]" \
              << error                              \
              << "[" << logger << "]"               \
              << "{" << message << "}" << std::endl

#define LOG_WARNING(logger, message)                \
    std::cout << "["  << xTaskGetTickCount() << "]" \
              << warning                            \
              << "[" << logger << "]"               \
              << "{" << message << "}" << std::endl

#define LOG_INFO(logger, message)                   \
    std::cout << "["  << xTaskGetTickCount() << "]" \
              << info                               \
              << "[" << logger << "]" <<            \
              << "{" << message << "}" << std::endl

#else

#define LOG_ERROR(logger, message)

#define LOG_WARNING(logger, message)

#define LOG_INFO(logger, message)

#endif