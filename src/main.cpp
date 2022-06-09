#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../include/config.hpp"
#include <Indicator.hpp>
#include <logs.hpp>
#include <driver/gpio.h>
#include <esp_intr_alloc.h>
#include <esp_system.h>
#include <conditioanl_variable.hpp>

void interrupt(void* interrupt) noexcept
{
    auto cv = reinterpret_cast<utils::conditional_variable*>(interrupt);
    gpio_intr_disable(GPIO_NUM_25);
    cv->notify();
    
}
static uint32_t counter = 0;


void task(void* task) noexcept
{
    auto cv = reinterpret_cast<utils::conditional_variable*>(task);
    gpio_pad_select_gpio(GPIO_NUM_25);
    gpio_set_direction(GPIO_NUM_25, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_25, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(GPIO_NUM_25, GPIO_INTR_NEGEDGE);
    gpio_intr_enable(GPIO_NUM_25);
    
    auto status = gpio_isr_register(interrupt, cv, ESP_INTR_FLAG_LOWMED, NULL);
    if (status == ESP_OK) {
        LOG_INFO("ESP", "Ok");
    } else {
        LOG_ERROR("ESP", "Whats wrong" << status);
    }
    
    while (true)
    {
        cv->wait();
        vTaskDelay(200 / portTICK_PERIOD_MS);
        counter++;
        std::cout << counter << std::endl;
        gpio_intr_enable(GPIO_NUM_25);
    }
}

extern "C" void app_main() 
{
    static utils::conditional_variable cv;
    xTaskCreate(task, "interrupt", 1024, &cv, 1, cv.getNewTask());
    
}
