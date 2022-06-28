#pragma once

#include <freertos/FreeRTOS.h>
#include <logs.hpp>

namespace utils {

template<typename Type>
void assert_null(Type value, const char* message) 
{
    if (value == NULL || value == nullptr)
    {
        LOG_ERROR("ASSERT_NULL", message);
        vTaskDelay(portMAX_DELAY);
    }
}

}
