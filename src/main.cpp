#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <Timer.hpp>
#include <logs.hpp>

struct A {

    uint16_t counter = 0;
    void increment() {
        ++counter;
        std::cout << counter << std::endl;
    }
};


extern "C" void app_main() 
{ 
    static A a;
    static utils::Timer timer(1000, std::bind(&A::increment, &a));
    timer.start();
}
