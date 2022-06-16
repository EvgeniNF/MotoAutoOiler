#include "Server.hpp"
#include <iostream>

namespace device
{

Server::Server() : m_server(80)
{
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAPConfig({192, 168, 1, 1}, {192, 168, 1, 1}, {255, 255, 255, 0});
    WiFi.softAP("TestPoint", "12345678");

    m_dnsServer.setTTL(300);

    m_dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);

    std::cout << m_dnsServer.start(53, "www.motooiler.com", {192, 168, 1, 1}) << std::endl;

    m_server.on("/", [this](AsyncWebServerRequest *request){
        homePage(request);
    });

    m_server.begin();
    
}

void Server::run() noexcept
{
    while (true)
    {
        m_dnsServer.processNextRequest();
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

void Server::homePage(AsyncWebServerRequest* request) noexcept
{
    request->send(200, "text/plain", "Hello world");
}

}