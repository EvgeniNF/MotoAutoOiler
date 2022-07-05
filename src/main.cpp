#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <logs.hpp>
#include "Controller.hpp"
#include <Message.hpp>



extern "C" void app_main() { 
    static Controller controller;
    controller.run();
}
