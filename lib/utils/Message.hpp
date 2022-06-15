#pragma once

#include <freertos/FreeRTOS.h>

namespace utils
{

struct Message
{
    uint16_t serviceId;
    uint16_t data[5];
    uint16_t size;
};

}