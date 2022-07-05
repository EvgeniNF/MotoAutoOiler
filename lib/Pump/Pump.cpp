#include "Pump.hpp"
#include <Arduino.h>
#include <Message.hpp>
#include <iostream>
#include <utility>
#include <asserts.hpp>
#include <Timer.hpp>

namespace device
{

Pumps::Pumps(uint16_t id, uint16_t onTime, uint16_t offTime, gpio_num_t pump1, gpio_num_t pump2, xQueueHandle m_messageHandler) :
m_id(id), m_messageHandler(m_messageHandler), m_pump1(pump1), m_pump2(pump2),
m_timerOn1(onTime, std::bind(&Pumps::callbackOn1, this)),
m_timerOn2(onTime, std::bind(&Pumps::callbackOn2, this)),
m_timerOnAll(onTime, std::bind(&Pumps::callbackAllOn, this)),
m_timerOffAll(offTime, std::bind(&Pumps::callbackOffAll, this)) {}

void Pumps::callbackOn1() noexcept {
    offPump1();
    utils::Message message {
        .serviceId = m_id,
        .data = {1},
        .size = 1
    };
    xQueueSend(m_messageHandler, &message, 0);
    m_runningFlag = false;
}

void Pumps::callbackOn2() noexcept {
    offPump2();
    utils::Message message {
        .serviceId = m_id,
        .data = {2},
        .size = 1
    };
    xQueueSend(m_messageHandler, &message, 0);
    m_runningFlag = false;
}

void Pumps::callbackAllOn() noexcept {
    offPump1();
    offPump2();
    utils::Message message {
        .serviceId = m_id,
        .data = {3},
        .size = 1
    };
    xQueueSend(m_messageHandler, &message, 0);
    m_timerOffAll.start();
}

void Pumps::callbackOffAll() noexcept {
    startPumping();
}

void Pumps::offPump1() const noexcept {
    digitalWrite(m_pump1, false);
}

void Pumps::offPump2() const noexcept {
    digitalWrite(m_pump2, false);
}

void Pumps::pulsPump1() noexcept {
    digitalWrite(m_pump1, true);
    m_timerOn1.start();
    m_runningFlag = true;
}

void Pumps::pulsPump2() noexcept {
    digitalWrite(m_pump2, true);
    m_timerOn2.start();
    m_runningFlag = true;
}

void Pumps::startPumping() noexcept {
    digitalWrite(m_pump1, true);
    digitalWrite(m_pump2, true);
    m_timerOnAll.start();
    m_runningFlag = true;
}

void Pumps::stop() noexcept {
    m_timerOn1.stop();
    m_timerOn2.stop();
    m_timerOnAll.stop();
    m_timerOffAll.stop();
    offPump1();
    offPump2(); 
    m_runningFlag = false;
}

bool Pumps::isRunning() const noexcept {
    return m_runningFlag;
}

}
