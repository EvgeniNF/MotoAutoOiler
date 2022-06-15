#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Voltage.hpp>
#include <iostream>
#include <Message.hpp>
#include <Button.hpp>

#define LOG(level, message) std::cout << "[" << level << "] {" << message << "}" << std::endl

void task_message_handler(void* messageQueue)
{
    LOG("INFO", "Start task message handler");
    auto queue = reinterpret_cast<xQueueHandle*>(messageQueue);
    while (true)
    {
        utils::Message resiveMessage;
        xQueueReceive(*queue, &resiveMessage, portMAX_DELAY);
        LOG("INFO", "New message: { id:" << resiveMessage.serviceId 
                                         << ", data:" << resiveMessage.data[0] 
                                         << ", size:" << resiveMessage.size << "}");
    }
}

void task_analog_reader(void* messageQueue)
{
    LOG("INFO", "Start task analog read");
    auto queue = reinterpret_cast<xQueueHandle*>(messageQueue);
    sensor::Volatage voltageSensor(1, *queue, GPIO_NUM_32, {0, 3300});
    voltageSensor.run();
}

void task_button(void* messageQueue)
{
    auto queue = reinterpret_cast<xQueueHandle*>(messageQueue);
    Button button{2, *queue, GPIO_NUM_12};
    button.run();
}

extern "C" void app_main() 
{ 
    static auto queue = xQueueCreate(10, sizeof(utils::Message));
    xTaskCreate(task_message_handler, "Task.MessageHandler", 3024, &queue, 1, nullptr);
    xTaskCreate(task_analog_reader, "Task.AnalogRead", 8000, &queue, 1, nullptr);
    xTaskCreate(task_button, "Task.Button", 3000, &queue, 1, nullptr);
}
