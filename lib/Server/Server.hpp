#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

namespace device
{

class Server
{
   public: 
    Server();
    ~Server() = default;

   public:
    void run() noexcept; 

   private:
    void homePage(AsyncWebServerRequest* request) noexcept;  

   private:
    AsyncWebServer m_server;
    DNSServer m_dnsServer;
};

}
