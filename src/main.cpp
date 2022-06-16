#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Voltage.hpp>
#include <iostream>
#include <Message.hpp>
#include <Button.hpp>
#include <Storage.hpp>
#include <Server.hpp>
#include <nvs_flash.h>

extern "C" void app_main() 
{ 
    nvs_flash_init();
    static device::Server server;
    server.run();
}
