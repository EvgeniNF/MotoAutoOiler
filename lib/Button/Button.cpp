#include "Button.hpp"

void IRAM_ATTR buttonInterruptHandler(void* ptr) noexcept
{
    auto button = reinterpret_cast<Button*>(ptr);
    gpio_intr_disable(button->pin);
    static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(button->hendler, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void initButton(Button& button) noexcept
{
    gpio_pad_select_gpio(button.pin);
    gpio_set_direction(button.pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(button.pin, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(button.pin, GPIO_INTR_ANYEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL2);
    gpio_isr_handler_add(button.pin, buttonInterruptHandler, &button);
    gpio_intr_enable(button.pin);
}

void waitInterrupt(Button& button) noexcept
{
    xSemaphoreTake(button.hendler, portMAX_DELAY);
}

bool buttonHandler(Button& button) noexcept
{
    waitInterrupt(button);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    gpio_intr_enable(button.pin);
    if (gpio_get_level(button.pin) == 1)
    {
        return true;
    }
    return false;
}