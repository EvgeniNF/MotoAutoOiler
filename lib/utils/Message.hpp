#pragma once

#include <freertos/FreeRTOS.h>
#include <iostream>

namespace utils
{

struct Message
{
    int16_t serviceId;
    uint16_t data;
};

}
