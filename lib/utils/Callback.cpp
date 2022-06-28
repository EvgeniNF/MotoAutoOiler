#include "Callback.hpp"
#include <asserts.hpp>
#include <logs.hpp>

namespace utils 
{

void Callback::timerFunction(void* callbackPtr) noexcept
{
    auto functor = reinterpret_cast<struct Callback*>(pvTimerGetTimerID(callbackPtr));
    functor->call();
}   

static void taskFunction(void* callbackPtr) noexcept 
{
    auto functor = reinterpret_cast<struct Callback*>(callbackPtr);
    functor->call();
}

}
