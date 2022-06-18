#pragma once

#include <Button.hpp>
#include <Pump.hpp>
#include <Speed.hpp>
#include <Voltage.hpp>
#include <Storage.hpp>
#include <Message.hpp>
#include <Server.hpp>
#include <driver/gpio.h>
#include "config.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

enum class Devices
{
    Pump1,
    Pump2,
    SpeedSensor,
    Button,
    Voltage
};

enum class ButtonSignals
{
    Click,
    DoubleClick,
    Hold
};

class Controller
{
   public:
    Controller();
    ~Controller() = default;

   private:
    static void buttonTask(void* buttonPtr) noexcept;
    static void speedTask(void* speedSensorPtr) noexcept;
    static void serverTask(void* serverPtr) noexcept;
    static void voltageTask(void* voltageSensorPtr) noexcept;

   public:
    void run() noexcept; 

   private:
    void handleButtonMessage(utils::Message const& message) noexcept; 

    void handleClickEvent() noexcept;

    void handleDoubleClickEvent() noexcept;

    void handleHoldClickEvent() noexcept;

    void handleSpeedSensorMessage(utils::Message const& message) noexcept;

    void handleOiling() noexcept;

    void handlePumping() noexcept;

   private:
    xQueueHandle m_messageQueue;
    device::Storage m_storage; 
    device::Pump m_pump1;
    device::Pump m_pump2;
    sensor::Speed m_speedSensor;
    Button m_button;
    sensor::Volatage m_voltageSensor;
    device::Server m_server;
    device::Mode m_previosMode;
    xTimerHandle m_pump1TimerOff;
    xTimerHandle m_pump2TimerOff;
};