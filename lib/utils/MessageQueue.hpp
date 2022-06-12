#pragma once

#include "Message.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace utils
{

    class MessageQueue
    {
       public:
        MessageQueue(size_t queueSize);
        MessageQueue(MessageQueue&) = delete;
        MessageQueue(MessageQueue&&) noexcept = default;
        MessageQueue& operator=(MessageQueue const&) = delete;
        ~MessageQueue() = default;

       public:
        Message waitMessage(uint32_t waitingTimeout = portMAX_DELAY) noexcept; 

        bool sendMessage(Message const& message) noexcept;

        bool isExist() const noexcept;

       private:
        xQueueHandle m_queue;
    };

    MessageQueue* getInstanceQueue() noexcept;
}


