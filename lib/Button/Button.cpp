#include "Button.hpp"
#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <driver/gpio.h>
#include <Message.hpp>
#include <iostream>

Button::Button(uint16_t serviceId, xQueueHandle messageQueue, gpio_num_t gpio) :
               m_serviceId(serviceId), m_messageQueue(messageQueue), m_buttonPin(gpio)
{
    m_interruptFlag = xSemaphoreCreateBinary();
    pinMode(m_buttonPin, INPUT_PULLUP);
    attachInterrupt(m_buttonPin, std::bind(&Button::interruptHandler, this), CHANGE);
    m_timerClick = xTimerCreate("ClickTimer", 500 / portTICK_PERIOD_MS, pdFALSE, this, timerClickHandler);
    m_timerHold = xTimerCreate("HoldTimer", 1500 / portTICK_PERIOD_MS, pdFALSE, this, timerHoldHandler);
}

void Button::timerHoldHandler(void* buttonPtr) noexcept
{
    auto button = reinterpret_cast<Button*>(pvTimerGetTimerID(buttonPtr));
    button->checkHold();
}

void Button::timerClickHandler(void* buttonPtr) noexcept
{
    auto button = reinterpret_cast<Button*>(pvTimerGetTimerID(buttonPtr));
    button->checkClick();
}

void Button::checkHold() noexcept
{
    m_holdFlag = true;
}

void Button::checkClick() noexcept
{
    if (m_clickCounter == 1)
    {
        utils::Message message
        {
            .serviceId = m_serviceId,
            .data = {0},
            .size = 1
        };
        m_clickCounter = 0;
        xQueueSend(m_messageQueue, &message, 0);
        stopHoldTimer();
    }
}

void Button::run() noexcept
{
    while (true)
    {
        xSemaphoreTake(m_interruptFlag, portMAX_DELAY);
        vTaskDelay(150 / portTICK_PERIOD_MS);
        if (m_clickCounter == 0)
        {
            xTimerStart(m_timerClick, 0);
            xTimerStart(m_timerHold, 0);
        }
        
        if (digitalRead(m_buttonPin) == true)
        {
            ++m_clickCounter;
        }
        if (m_clickCounter >= 2)
        {
            utils::Message message
            {
                .serviceId = m_serviceId,
                .data = {1},
                .size = 1
            };
            xQueueSend(m_messageQueue, &message, 0);
            m_clickCounter = 0;

            stopAllTimers();
        }

        if (m_holdFlag && m_clickCounter == 1)
        {
            utils::Message message
            {
                .serviceId = m_serviceId,
                .data = {2},
                .size = 1
            };
            xQueueSend(m_messageQueue, &message, 0);
            m_clickCounter = 0;
            m_holdFlag = false;
            stopAllTimers();
        }

        gpio_intr_enable(m_buttonPin);
    }
    
}

void Button::interruptHandler() noexcept
{
    gpio_intr_disable(m_buttonPin);
    xSemaphoreGive(m_interruptFlag);
}

void Button::stopAllTimers() noexcept
{
    stopHoldTimer();
    if (xTimerIsTimerActive(m_timerClick))
    {
        xTimerStop(m_timerClick, 0);
    }
}

void Button::stopHoldTimer() noexcept
{
    if (xTimerIsTimerActive(m_timerHold))
    {
        xTimerStop(m_timerHold, 0);
    }
}
