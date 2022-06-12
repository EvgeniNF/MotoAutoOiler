#pragma once

#include <PumpControl.hpp>
#include <freertos/semphr.h>

enum class Commands
{
    PumpingRun = 0,
    Stop = 1,
    OilingPump1 = 2,
    OilingPump2 = 3,
    OilingAll = 4,
    Resume = 5,
    ChangeParameter = 6,
};

struct PumpServiceData
{
    uint32_t* durationImp;
    uint32_t* impInterval;
    uint16_t* numberImp;  
};

class PumpService
{
   public:
    PumpService(PumpServiceData const& data, gpio_num_t gpioPump1, gpio_num_t gpioPump2) : 
    m_data(data), 
    pump1(gpioPump1, *m_data.durationImp, *m_data.impInterval),
    pump2(gpioPump2, *m_data.durationImp, *m_data.impInterval)
    {};

    xTaskHandle createService()
    {
        xTaskHandle handle;
        xTaskCreate(serviceHendle, "PumpService", 2048, this, 1, &handle);
        return handle;
    }

    static void serviceHendle(void* servicePtr)
    {
        auto pumpService = reinterpret_cast<PumpService*>(servicePtr);
        while (true)
        {
            uint32_t ulNotifiedValue;
            xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY);
            switch (ulNotifiedValue)
            {
            case 0:
                pumpService->pump1.startPumping(*pumpService->m_data.numberImp);
                pumpService->pump2.startPumping(*pumpService->m_data.numberImp);
                break;
            case 1:
                pumpService->pump1.stopPumping();
                break;
            case 2:
                pumpService->pump1.startPumping(1);
                break;
            case 3:
                pumpService->pump2.startPumping(1);
                break;
            case 4:
                pumpService->pump1.startPumping(1);
                pumpService->pump2.startPumping(1);
                break;
            case 5:
                pumpService->pump1.resumePumping();
                pumpService->pump2.resumePumping();
                break;
            case 6:
                pumpService->pump1.setNewParameters(*pumpService->m_data.numberImp, *pumpService->m_data.impInterval, *pumpService->m_data.durationImp);
                pumpService->pump2.setNewParameters(*pumpService->m_data.numberImp, *pumpService->m_data.impInterval, *pumpService->m_data.durationImp);
                break;
            default:
                LOG_ERROR("PumpService", "UNCNOW COMMAND");
                break;
            }
        }
    }

    PumpServiceData m_data;
    pump_control::PumpControl pump1;
    pump_control::PumpControl pump2;
};
