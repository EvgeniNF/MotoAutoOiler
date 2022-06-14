#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <SensorService.hpp>
#include "config.hpp"


extern "C" void app_main() 
{ 
    static config::OilingParameters parameters;
    static xQueueHandle messageQueue = xQueueCreate(20, sizeof(config::Message));
    auto sensorServiceHendler = createSensorService(parameters, messageQueue, 1);
    if (sensorServiceHendler == NULL)
    {
        vTaskDelay(10'000u / portTICK_PERIOD_MS);
        esp_restart();
    }
}
