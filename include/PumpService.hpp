#pragma once

#include <PumpControl.hpp>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

struct Pumps
{
    Pump pump1;
    Pump pump2;
};

enum class Commands : uint32_t
{
    AllOff = 0,
    OnPump1 = 1,
    OffPump1 = 2,
    OnPump2 = 3,
    OffPump2 = 4, 
    AllOn = 5
};

static void pump_service_task(void* ptr) 
{
    auto pumps = reinterpret_cast<Pumps*>(ptr);
    while (true)
    {
        uint32_t command;
        if (xTaskNotifyWait(0x00, ULONG_MAX, &command, portMAX_DELAY) != pdTRUE)
        {
            continue;
        }

        switch (static_cast<Commands>(command))
        {
            case Commands::AllOff:
                setLevelPump(pumps->pump1, false);
                setLevelPump(pumps->pump2, false);
                break;
        
            case Commands::OnPump1:
                setLevelPump(pumps->pump1, true);
                break;
        
            case Commands::OffPump1:
                setLevelPump(pumps->pump1, false);
                break;
        
            case Commands::OnPump2:
                setLevelPump(pumps->pump2, true);
                break;
        
            case Commands::OffPump2:
                setLevelPump(pumps->pump2, false);
                break;

            case Commands::AllOn:
                setLevelPump(pumps->pump1, true);
                setLevelPump(pumps->pump2, true);
                break;

            default:
                break;
        }

    }
}

inline xTaskHandle createPumpService(gpio_num_t gpioPump1, gpio_num_t gpioPump2) noexcept
{
    static Pumps pumps;
    pumps.pump1.pumpId = 0;
    pumps.pump1.pumpPin = gpioPump1;
    pumps.pump2.pumpId = 1;
    pumps.pump2.pumpPin = gpioPump2;

    initPump(pumps.pump1);
    initPump(pumps.pump2);

    xTaskHandle handle;
    xTaskCreate(pump_service_task, "Task.Pump.Control", 1024, &pumps, 1, &handle);
    return handle;
}
