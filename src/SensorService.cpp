#include "SensorService.hpp"

void sensorTaskHandler(void* ptr) noexcept
{
    auto parameters = reinterpret_cast<SensorService*>(ptr);

    constexpr uint16_t updatePeriod = 1500u;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    constexpr TickType_t tickUpdatePeriod = updatePeriod / portTICK_PERIOD_MS;

    while (true)
    {
        auto distanceBetweenImps = *parameters->weelCircle / *parameters->numberImpulsByResolution;
        auto distance = computeDistance(parameters->interruptCounter, distanceBetweenImps);
        auto speed = computeSpeed(distance, updatePeriod);

        config::Message message
        {
            .serviceId = parameters->serviceId,
            .message = {distance, speed}
        };

        xQueueSend(parameters->messageQueue, &message, 0);

        parameters->interruptCounter = 0;
        vTaskDelayUntil(&xLastWakeTime, tickUpdatePeriod);
    }
}

xTaskHandle createSensorService(config::OilingParameters const& parameters, 
                                xQueueHandle serviceMessanger, 
                                uint16_t priority) noexcept
{
    
    static SensorService serviceParameters
    {
        .serviceId = 1,
        .sensorPin = parameters.SPEED_SENSOR,
        .numberImpulsByResolution = &parameters.numberOfImpulsByResolution,
        .weelCircle = &parameters.weelCircle,
        .messageQueue = serviceMessanger,
        .interruptCounter = 0
    };

    initSensor(serviceParameters.sensorPin, &serviceParameters.interruptCounter);

    xTaskHandle taskHandle;
    xTaskCreate(sensorTaskHandler, "Task.Sensor", 2048, &serviceParameters, priority, &taskHandle);
    return taskHandle;
}
