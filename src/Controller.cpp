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
        switch (message.serviceId)
        {
        case 0:
            std::cout << "Message{ Pump 1 -> " << "Off" <<  "}" << std::endl;
            break;
        case 1:
            std::cout << "Message{ Pump 2 -> " << "Off" <<  "}" << std::endl;
            break;
        case 2:
            std::cout << "Message{ speed sensor -> speed: " << message.data[0] << ", distance: " << message.data[1] <<  "}" << std::endl;
            break;
        case 3:
            if (message.data[0] == 0)
            {
                std::cout << "Message{ button -> " << "click" <<  "}" << std::endl;
            } 
            else if (message.data[0] == 1)
            {
                std::cout << "Message{ button -> " << "double click" <<  "}" << std::endl;
            }
            else
            {
                std::cout << "Message{ button -> " << "hold click" <<  "}" << std::endl;
            }
            break;
        case 4:
            std::cout << "Message{ voltage sensor -> " << message.data[0] << "}" << std::endl;
            break;
        }
        
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
