#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Controller.hpp>
#include <nvs_flash.h>

extern "C" void app_main() 
{ 
    nvs_flash_init();
    Controller controller;
    controller.run();
}
