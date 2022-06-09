#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <logs.hpp>
#include <vector>

namespace utils
{

class conditional_variable
{
    public:
     conditional_variable() = default;
     ~conditional_variable() = default;

    public:
     void wait() const noexcept
     {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 
     }

     void notify() noexcept
     {
        for (auto value: m_tasks){
           configASSERT(value);
           xTaskNotifyGive(value);
        }     
     }
    
     TaskHandle_t* const getNewTask() noexcept 
     {
        m_tasks.emplace_back(TaskHandle_t{NULL});
        auto reference = &m_tasks.at(m_tasks.size() - 1); 
        return reference;
     }

   private:
     std::vector<TaskHandle_t> m_tasks;
};

}
