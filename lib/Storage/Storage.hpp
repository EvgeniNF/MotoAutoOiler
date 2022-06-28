#pragma once

#include <EEPROM.h>

namespace device
{

enum class Mode : uint8_t
{
    Off,
    PumpingOn,
    PumpingOff,
    Oiling,
    OilingRain,
    ForsedPumpingOn,
    ForsedPumpingOff
};

enum class Parameter
{
    mode,
    numberOfImpulsByResolution,
    weelCircle,
    maxTownSpeed,
    minSpeedOiling,
    maxSpeedOiling,
    intervalTownPump1,
    intervalTownPump2,
    intervalRoadPump1,
    intervalRoadPump2,
    rainCoef,
    pumpingImpulsInterval,
    durationImpuls,
    numberOfImpulsInPumping,
    oilImpuls,
    maxNumOfImpuls,
    password,
    accessPointName
};

struct Storage
{
   public:
    Storage();
    ~Storage(); 

   public:
    Mode mode{Mode::Off};
    uint16_t numberOfImpulsByResolution{12};            
    uint16_t weelCircle{2040};// mm

    uint16_t maxTownSpeed{80};            
    uint16_t minSpeedOiling{20};             
    uint16_t maxSpeedOiling{180};

    uint16_t intervalTownPump1 {1400};
    uint16_t intervalTownPump2 {1400};
    uint16_t intervalRoadPump1 {1700};
    uint16_t intervalRoadPump2 {1700};

    uint16_t actualDistancePump1{0};
    uint16_t actualDistancePump2{0};

    float rainCoef{0.0};
    float voltage{0.0};
    uint16_t speed{0};

    uint16_t pumpingImpulsInterval{1000};
    uint16_t durationImpuls{500};
    uint16_t numberOfImpulsInPumping{60};
    uint16_t impulsCounter{0};

    uint16_t oilImpuls{0};
    uint16_t maxNumOfImpuls{0};
    
    String accessPointName{"MotoOiler"};
    String password{"12345678"};

    void write(uint16_t value, Parameter const parameter) noexcept;
    void write(float value, Parameter const parameter) noexcept;
    void write(Mode value, Parameter const parameter) noexcept;
    void write(String value, Parameter const parameter) noexcept;
   private:
    void read() noexcept;
    static void write(String value, uint8_t address) noexcept; 
    static void read(String& value, uint8_t address) noexcept;
};

}
