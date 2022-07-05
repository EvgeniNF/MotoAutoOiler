#include "Controller.hpp"

#include <iostream>
#include <asserts.hpp>

Controller::Controller()
: m_messageQueue(xQueueCreate(30, sizeof(utils::Message))),
  m_storage(),
  m_pumps(1, m_storage.durationImpuls, m_storage.pumpingImpulsInterval, config::PUMP_1, config::PUMP_2, m_messageQueue),
  m_speedSensor(config::SPEED_SENSOR, m_messageQueue, 2, m_storage.numberOfImpulsByResolution, m_storage.weelCircle),
  m_button(3, m_messageQueue, config::BUTTON),
  m_voltageSensor(4, m_messageQueue, config::VOLTAGE_SENSOR, {0, 15})  
{
    xTaskHandle handleSpeedTask;
    xTaskCreate(speedTask, "Task.SpeedSensor", 4000, &m_speedSensor, 1, &handleSpeedTask);
    utils::assert_null(handleSpeedTask, "Speed task is null");
    xTaskHandle handleButtonTask;
    xTaskCreate(buttonTask, "Task.Button", 4000, &m_button, 1, &handleButtonTask);
    utils::assert_null(handleButtonTask, "Button task is null");
    xTaskHandle handleVoltageTask;
    xTaskCreate(voltageTask, "Task.VoltageSensor", 4000, &m_voltageSensor, 1, &handleVoltageTask);
    utils::assert_null(handleVoltageTask, "Voltage sensor task is null");
}

void Controller::run() noexcept
{
    while (true)
    {
        utils::Message message;
        xQueueReceive(m_messageQueue, &message, portMAX_DELAY);
        
        switch (static_cast<Devices>(message.serviceId)) {
        
            case Devices::Pump:
                handleCountPuls();
                break;
        
            case Devices::SpeedSensor:
                handleSpeedSensorMessage(message);
                break;
        
            case Devices::Button:
                m_pumps.stop();
                handleButtonMessage(message);
                break;
        
            case Devices::Voltage:
                m_storage.voltage = message.data[0] / 100;
                break;

            case Devices::Storage:
                break;

        }

        handlePumpControl();
    }
    
}

void Controller::handleButtonMessage(utils::Message const& message) noexcept {
    switch (static_cast<ButtonSignals>(message.data[0]))
    {
    case ButtonSignals::Click:      
        handleClickEvent();
        break;
    case ButtonSignals::DoubleClick:  
        handleDoubleClickEvent();
        break;
    case ButtonSignals::Hold:         
        handleHoldClickEvent();
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

void Controller::handleClickEvent() noexcept
{
    switch (m_storage.mode)
    {
    case device::Mode::Off:
        m_storage.mode = device::Mode::Oiling;
        break;

    case device::Mode::PumpingOn:
        m_storage.mode = device::Mode::PumpingOff;
        break;

    case device::Mode::PumpingOff:
        m_storage.mode = device::Mode::PumpingOn;
        break;

    case device::Mode::Oiling:
        m_storage.mode = device::Mode::OilingRain;
        break;

    case device::Mode::OilingRain:
        m_storage.mode = device::Mode::Oiling;
        break;

    case device::Mode::ForsedPumpingOn:
        m_storage.mode = device::Mode::ForsedPumpingOff;
        break;

    case device::Mode::ForsedPumpingOff :
        m_storage.mode = device::Mode::ForsedPumpingOn;
        break;
    }
    m_storage.write(m_storage.mode, device::Parameter::mode);
}

void Controller::handleDoubleClickEvent() noexcept {
    switch (m_storage.mode) {

    case device::Mode::Off:
    case device::Mode::Oiling:
    case device::Mode::OilingRain:
    case device::Mode::ForsedPumpingOn:
    case device::Mode::ForsedPumpingOff:
        m_storage.mode = device::Mode::PumpingOff;
        break;

    case device::Mode::PumpingOn:
    case device::Mode::PumpingOff:
        m_storage.mode = device::Mode::Oiling;
        break;

    }    

    m_storage.write(m_storage.mode, device::Parameter::mode);
}

void Controller::handleHoldClickEvent() noexcept {
    switch (m_storage.mode) {

    case device::Mode::ForsedPumpingOn:
    case device::Mode::ForsedPumpingOff:
        m_storage.mode = device::Mode::Off;
        break;

    case device::Mode::Off:
        m_storage.mode = device::Mode::Oiling;
        break;

    case device::Mode::Oiling:
    case device::Mode::OilingRain:
    case device::Mode::PumpingOn:
    case device::Mode::PumpingOff:
        m_storage.mode = device::Mode::ForsedPumpingOff;
        break;

    }    

    m_storage.write(m_storage.mode, device::Parameter::mode);
}

void Controller::handleSpeedSensorMessage(utils::Message const& message) noexcept {
    m_storage.speed = message.data[0];

    switch (m_storage.mode) {
    
        case device::Mode::Oiling:
            m_storage.actualDistancePump1 += message.data[1];
            m_storage.actualDistancePump2 += message.data[1];
            handleOiling();
            break;

        case device::Mode::OilingRain:
            m_storage.actualDistancePump1 += message.data[1] * m_storage.rainCoef;
            m_storage.actualDistancePump2 += message.data[1] * m_storage.rainCoef;
            handleOiling();
            break;

        default:
            break;
    }
}

void Controller::handleOiling() noexcept {
    if (m_storage.speed < m_storage.minSpeedOiling || m_storage.speed > m_storage.maxSpeedOiling) {
        return;
    }

    auto executePumpFunction = [&] (uint16_t interval1, uint16_t interval2) {
        
        if (m_storage.actualDistancePump1 > interval1) {
            m_storage.actualDistancePump1 = 0;
            m_pumps.pulsPump1();
        }

        if (m_storage.actualDistancePump2 > interval2) {
            m_storage.actualDistancePump2 = 0;
            m_pumps.pulsPump2();
        }
    };

    if (m_storage.speed > m_storage.maxTownSpeed) {
        executePumpFunction(m_storage.intervalRoadPump1, m_storage.intervalRoadPump2);
    } else {
        executePumpFunction(m_storage.intervalTownPump1, m_storage.intervalTownPump2);
    }

}

void Controller::handlePumping() noexcept {
    if (m_storage.speed > m_storage.minSpeedOiling && m_storage.speed < m_storage.maxSpeedOiling) {
        if (!m_pumps.isRunning()) {
            m_pumps.startPumping();
        }
    } else {
        m_pumps.stop();
    }
}

void Controller::handleForsedPumping() noexcept {
    if (!m_pumps.isRunning()) {
        m_pumps.startPumping();
    }

}

void Controller::handlePumpControl() noexcept {
    switch (m_storage.mode) {
    
    case device::Mode::Off:
    case device::Mode::ForsedPumpingOff:
    case device::Mode::PumpingOff:
        m_pumps.stop();
        break;
    
    case device::Mode::Oiling:
    case device::Mode::OilingRain:
        handleOiling();
        break;

    case device::Mode::PumpingOn:
        handlePumping();
        break;

    case device::Mode::ForsedPumpingOn:
        handleForsedPumping();
        break;
    
    }
}

void Controller::handleCountPuls() noexcept {
    
    auto checkImpulsCounter = [&] () -> bool {
        ++m_storage.impulsCounter;
        if (m_storage.impulsCounter > m_storage.numberOfImpulsInPumping) {
            m_storage.impulsCounter = 0;
            return true;
        }
        return false;
    };

    switch (m_storage.mode) {
    
        case device::Mode::Oiling:
        case device::Mode::OilingRain:
            /// ...
            break;

        case device::Mode::PumpingOn:
            if (checkImpulsCounter()) {
                m_storage.mode = device::Mode::PumpingOff;
                m_storage.write(m_storage.mode, device::Parameter::mode);
            }
            break;
    
        case device::Mode::ForsedPumpingOn:
            if (checkImpulsCounter()) {
                m_storage.mode = device::Mode::ForsedPumpingOff;
                m_storage.write(m_storage.mode, device::Parameter::mode);
            }
            break;

        default:
            break;
    }

}
