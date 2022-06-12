#include "MessageQueue.hpp"
#include <logs.hpp>

namespace utils
{
    
MessageQueue::MessageQueue(size_t queueSize)
{
    m_queue = xQueueCreate(queueSize, sizeof(Message));
    if (m_queue == NULL) 
    {
        LOG_ERROR("Message.queue", "Message queue was not created");
        vTaskDelay(portMAX_DELAY);
    }
}

Message MessageQueue::waitMessage(uint32_t waitingTimeout) noexcept 
{
    Message message;
    xQueueReceive(m_queue, &message, portMAX_DELAY);
    LOG_INFO("Message.queue", "Resive message: service id: " << message.serviceId << "; message: " << message.data);
    return message;
}

bool MessageQueue::sendMessage(Message const& message) noexcept
{
    LOG_INFO("Message.queue", "Send message: service id: " << message.serviceId << "; message: " << message.data);
    if(xQueueSend(m_queue, &message, 0) != pdPASS) 
    {
        LOG_ERROR("SendMessage", "Send message error");
        return false;
    }
    return true;
}

bool MessageQueue::isExist() const noexcept 
{
    if (m_queue == NULL)
    {
        return false;
    }
    else 
    {
        return true;
    }
}

MessageQueue* getInstanceQueue() noexcept
{
    static utils::MessageQueue queue(10);
    return &queue;
}

} // namespace utils
