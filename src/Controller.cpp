#include "Controller.hpp"

#include <iostream>

Controller::Controller()
: m_messageQueue(xQueueCreate(30, sizeof(utils::Message))),
  m_storage(),
  m_pump1(0, config::PUMP_1, m_messageQueue, m_storage.durationImpuls),
  m_pump2(1, config::PUMP_2, m_messageQueue, m_storage.durationImpuls),
  m_speedSensor(config::SPEED_SENSOR, m_messageQueue, 2, m_storage.numberOfImpulsByResolution, m_storage.weelCircle),
  m_button(3, m_messageQueue, config::BUTTON),
  m_voltageSensor(4, m_messageQueue, config::VOLTAGE_SENSOR, {0, 15})  
{
    xTaskCreate(speedTask, "Task.SpeedSensor", 4000, &m_speedSensor, 1, NULL);
    xTaskCreate(buttonTask, "Task.Button", 4000, &m_button, 1, NULL);
    xTaskCreate(voltageTask, "Task.VoltageSensor", 4000, &m_voltageSensor, 1, NULL);
    //xTaskCreate(serverTask, "Task.Server", 8000, &m_server, 1, NULL);
}

void Controller::run() noexcept
{
    m_pump1.startPuls();
    m_pump2.startPuls();
    while (true)
    {
        utils::Message message;
        xQueueReceive(m_messageQueue, &message, portMAX_DELAY);
        switch (static_cast<Devices>(message.serviceId))
        {
        case Devices::Pump1:
            
            break;
        case Devices::Pump2:
            
            break;
        case Devices::SpeedSensor:
            m_storage.speed = message.data[0];
            break;
        case Devices::Button:
            handleButtonMessage(message);
            break;
        case Devices::Voltage:
            m_storage.voltage = message.data[0] / 100;
            break;
        }
        
    }
    
}

void Controller::handleButtonMessage(utils::Message const& message) noexcept
{
    switch (message.data[0])
    {
    case 0:         /// <<- click
        switch (m_storage.mode)
        {
        case device::Mode::Oiling:
            m_storage.mode = device::Mode::OilingRain;
            m_storage.write(m_storage.mode, device::Parameter::mode);
            break;

        case device::Mode::OilingRain:
            m_storage.mode = device::Mode::Oiling;
            m_storage.write(m_storage.mode, device::Parameter::mode);
            break;

        case device::Mode::PumpingOn:
            m_storage.mode = device::Mode::PumpingOff;
            break;
        
        case device::Mode::PumpingOff:
            m_storage.mode = device::Mode::PumpingOn;
            break;

        case device::Mode::ForsedPumpingOn:
            m_storage.mode = device::Mode::PumpingOff;
            break;

        case device::Mode::ForsedPumpingOff:
            m_storage.mode = device::Mode::PumpingOn;
            break;
        }
        break;
    case 1:         /// <<- double click
        switch (m_storage.mode)
        {
        case device::Mode::Off:
        case device::Mode::ForsedPumpingOn:
        case device::Mode::ForsedPumpingOff:
        case device::Mode::Oiling:
        case device::Mode::OilingRain:
            m_storage.mode = device::Mode::PumpingOff;
            m_storage.write(m_storage.mode, device::Parameter::mode);
            break;

        case device::Mode::PumpingOn:
        case device::Mode::PumpingOff:
            m_storage.mode = device::Mode::Oiling;
            m_storage.write(m_storage.mode, device::Parameter::mode);
            break;
        }
        break;
    case 2:         /// <<- hold click
        switch (m_storage.mode)
        {
        case device::Mode::PumpingOff:
        case device::Mode::PumpingOn:
        case device::Mode::Oiling:
        case device::Mode::OilingRain:
            m_storage.mode = device::Mode::ForsedPumpingOff;
            m_storage.write(m_storage.mode, device::Parameter::mode);
            break;
        case device::Mode::ForsedPumpingOn:
        case device::Mode::ForsedPumpingOff:
            m_storage.mode = device::Mode::Off;
            break;
        case device::Mode::Off:
            m_storage.mode = device::Mode::Oiling;
            break;
        }
        break;
    }
}

void Controller::buttonTask(void* buttonPtr) noexcept
{
    auto button = reinterpret_cast<Button*>(buttonPtr);
    button->run();
}

void Controller::speedTask(void* speedSensorPtr) noexcept
{
    auto speedSensor = reinterpret_cast<sensor::Speed*>(speedSensorPtr);
    speedSensor->run();
}

void Controller::serverTask(void* serverPtr) noexcept
{
    auto server = reinterpret_cast<device::Server*>(serverPtr);
    server->run();
}

void Controller::voltageTask(void* voltageSensorPtr) noexcept
{
    auto voltageSensor = reinterpret_cast<sensor::Volatage*>(voltageSensorPtr);
    voltageSensor->run();
}
