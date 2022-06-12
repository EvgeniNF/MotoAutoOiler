#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <freertos/queue.h>
#include <logs.hpp>
#include <MessageQueue.hpp>
#include <PumpService/PumpService.hpp>

static xTaskHandle control;

void controller_task(void* ptr)
{
    auto srvHendler = reinterpret_cast<xTaskHandle*>(ptr);
    auto servicesMessages = utils::getInstanceQueue();
    while (true)
    {
        auto result = servicesMessages->waitMessage();
        if (result.serviceId == 1){
            xTaskNotify(*srvHendler, result.data, eSetValueWithoutOverwrite);
        } 
        else 
        {
            xTaskNotify(control, 0, eSetValueWithoutOverwrite);
        }
    }
    
}


void start_pump(void*ptr) 
{
    auto servicesMessages = utils::getInstanceQueue();
    while (true)
    {
        utils::Message message{1, 2};
        servicesMessages->sendMessage(message);
        uint32_t val;
        xTaskNotifyWait(0x00, ULONG_MAX, NULL, portMAX_DELAY);
        vTaskDelay(1000 / portMAX_DELAY);
    }
}


extern "C" void app_main() 
{
    static uint32_t durationImp = 1000;
    static uint32_t impInterval = 2000;
    static uint16_t numberImp = 5;
    PumpServiceData data{&durationImp, &impInterval, &numberImp};
    static PumpService pumpService{data, GPIO_NUM_14, GPIO_NUM_15};

    static auto serviceHendler = pumpService.createService();
    xTaskCreate(controller_task, "2", 3024, &serviceHendler, 1, NULL);
    xTaskCreate(start_pump, "1", 3024, NULL, 1, &control);

}
