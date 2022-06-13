#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <PumpService.hpp>



void reaction1()
{
    LOG_INFO("Reaction", "Reaction 1");
}

void reaction2()
{
    LOG_INFO("Reaction", "Reaction 2");
}

void task_signals(void * arg) 
{
    auto signals = reinterpret_cast<std::vector<std::function<void()>>*>(arg);
    while (true)
    {
        signals->at(0)();
        vTaskDelay(1000 / portTICK_RATE_MS);
        signals->at(1)();
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

extern "C" void app_main() 
{ 
    static TaskController controller;
    Reaction r1 {1, reaction1};
    Reaction r2 {2, reaction2};
    static std::vector<std::function<void()>> signals;
    signals.emplace_back() = controller.createSignal(r1);
    signals.emplace_back() = controller.createSignal(r2);
    xTaskCreate(controller_task, "Task.Controller", 6024, &controller, 1, nullptr);
    xTaskCreate(task_signals, "Task.Signals", 1024, &signals, 1, nullptr);
}
