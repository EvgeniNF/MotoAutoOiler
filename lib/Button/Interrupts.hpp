#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <freertos/queue.h>
#include <esp_intr_alloc.h>
#include <logs.hpp>
#include <unordered_map>
#include <functional>

static xQueueHandle gpioEventQueue = xQueueCreate(10, sizeof(uint32_t));

static std::unordered_map<uint32_t, std::function<void(uint32_t)>> hendlers;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio = (uint32_t) arg;
    xQueueSendFromISR(gpioEventQueue, &gpio, NULL);
}

template<gpio_num_t gpio, 
         gpio_pull_mode_t pullup_mode=GPIO_PULLUP_ONLY,
         gpio_int_type_t intType=GPIO_INTR_POSEDGE>
void addInterrupt() 
{
    gpio_pad_select_gpio(gpio);
    gpio_set_direction(gpio, GPIO_MODE_INPUT);
    gpio_set_pull_mode(gpio, pullup_mode);
    gpio_set_intr_type(gpio, intType);
    gpio_intr_enable(gpio);

    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL2);
    gpio_isr_handler_add(gpio, gpio_isr_handler, (void*) gpio);
}

void task_wait_interrupts(void* arg)
{
    auto callbacks = reinterpret_cast<std::unordered_map<uint32_t, std::function<void(uint32_t)>>*>(arg);
    uint32_t io_num;
    while(true) {
        if(xQueueReceive(gpioEventQueue, &io_num, portMAX_DELAY)) {
            callbacks->at(io_num)(io_num);
        }
    }    

}


