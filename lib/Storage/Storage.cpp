#include "Storage.hpp"

namespace device
{

namespace adresses
{
    constexpr uint8_t writeFlag                 {0x00};

    constexpr uint8_t mode                      {0x01};
    constexpr uint8_t numberOfImpulsByResolution{0x03};
    constexpr uint8_t weelCircle                {0x05};
    constexpr uint8_t maxTownSpeed              {0x07};
    constexpr uint8_t minSpeedOiling            {0x09};
    constexpr uint8_t maxSpeedOiling            {0x0B};
    constexpr uint8_t intervalTownPump1         {0x0D};
    constexpr uint8_t intervalTownPump2         {0x11};
    constexpr uint8_t intervalRoadPump1         {0x13};
    constexpr uint8_t intervalRoadPump2         {0x15};
    constexpr uint8_t pumpingImpulsInterval     {0x17};
    constexpr uint8_t durationImpuls            {0x19};
    constexpr uint8_t numberOfImpulsInPumping   {0x1B};
    constexpr uint8_t oilImpuls                 {0x1D};
    constexpr uint8_t maxNumOfimpuls            {0x1F};
    constexpr uint8_t rainCoef                  {0x21};
    constexpr uint8_t password                  {0x23};
    constexpr uint8_t nameAp                    {0x2C}; 
} // namespace adresses


Storage::Storage()
{
    EEPROM.begin(100);
    uint16_t flag = static_cast<uint16_t>(EEPROM.read(adresses::writeFlag));
    if (flag == 10)
    {
        read();
    }
    else 
    {
        EEPROM.write(adresses::writeFlag, 10);
        write(mode, Parameter::mode);
        write(numberOfImpulsByResolution, Parameter::numberOfImpulsByResolution);
        write(weelCircle, Parameter::weelCircle);
        write(maxTownSpeed, Parameter::maxTownSpeed);
        write(minSpeedOiling, Parameter::minSpeedOiling);
        write(maxSpeedOiling, Parameter::maxSpeedOiling);
        write(intervalTownPump1, Parameter::intervalTownPump1);
        write(intervalTownPump2, Parameter::intervalTownPump2);
        write(intervalRoadPump1, Parameter::intervalRoadPump1);
        write(intervalRoadPump2, Parameter::intervalRoadPump2);
        write(rainCoef, Parameter::rainCoef);
        write(pumpingImpulsInterval, Parameter::pumpingImpulsInterval);
        write(durationImpuls, Parameter::durationImpuls);
        write(numberOfImpulsInPumping, Parameter::numberOfImpulsInPumping);
        write(oilImpuls, Parameter::oilImpuls);
        write(maxNumOfImpuls, Parameter::maxNumOfImpuls);
        write(accessPointName, Parameter::accessPointName);
        write(password, Parameter::password);
    }
}
Storage::~Storage()
{
    EEPROM.end();
}

void Storage::write(Mode value, Parameter const parameter) noexcept
{
    mode = value;
    EEPROM.write(adresses::mode, static_cast<uint8_t>(value));
    EEPROM.commit();
}

void Storage::write(uint16_t value, Parameter const parameter) noexcept
{
    switch (parameter)
    {
    case Parameter::numberOfImpulsByResolution:
        numberOfImpulsByResolution = value;
        EEPROM.writeShort(adresses::numberOfImpulsByResolution, numberOfImpulsByResolution);
        break;
    case Parameter::weelCircle:
        weelCircle = value;
        EEPROM.writeShort(adresses::weelCircle, weelCircle);
        break;
    case Parameter::maxTownSpeed:
        maxTownSpeed = value;
        EEPROM.writeShort(adresses::maxTownSpeed, maxTownSpeed);
        break;
    case Parameter::minSpeedOiling:
        minSpeedOiling = value;
        EEPROM.writeShort(adresses::minSpeedOiling, minSpeedOiling);
        break;
    case Parameter::maxSpeedOiling:
        maxSpeedOiling = value;
        EEPROM.writeShort(adresses::maxSpeedOiling, maxSpeedOiling);
        break;
    case Parameter::intervalTownPump1:
        intervalTownPump1 = value;
        EEPROM.writeShort(adresses::intervalTownPump1, intervalTownPump1);
        break; 
    case Parameter::intervalTownPump2:
        intervalTownPump2 = value;
        EEPROM.writeShort(adresses::intervalTownPump2, intervalTownPump2);
        break; 
    case Parameter::intervalRoadPump1:
        intervalRoadPump1 = value;
        EEPROM.writeShort(adresses::intervalRoadPump1, intervalRoadPump1);
        break; 
    case Parameter::intervalRoadPump2:
        intervalRoadPump2 = value;
        EEPROM.writeShort(adresses::intervalRoadPump2, intervalRoadPump2);
        break; 
    case Parameter::pumpingImpulsInterval:
        pumpingImpulsInterval = value;
        EEPROM.writeShort(adresses::pumpingImpulsInterval, pumpingImpulsInterval);
        break;    
    case Parameter::durationImpuls:
        durationImpuls = value;
        EEPROM.writeShort(adresses::durationImpuls, durationImpuls);
        break;  
    case Parameter::numberOfImpulsInPumping:
        numberOfImpulsInPumping = value;
        EEPROM.writeShort(adresses::numberOfImpulsInPumping, numberOfImpulsInPumping);
        break;  
    case Parameter::oilImpuls:
        oilImpuls = value;
        EEPROM.writeShort(adresses::oilImpuls, oilImpuls);
        break;  
    case Parameter::maxNumOfImpuls:
        maxNumOfImpuls = value;
        EEPROM.writeShort(adresses::maxNumOfimpuls, maxNumOfImpuls);
        break;  
    }
    EEPROM.commit();
}

void Storage::write(float value, Parameter const parameter) noexcept
{
    rainCoef = value;
    uint16_t valueForWrite = rainCoef * 100;
    EEPROM.writeUShort(adresses::rainCoef, valueForWrite);
    EEPROM.commit();
}

void Storage::write(String value, Parameter const parameter) noexcept
{
    uint8_t startAddr;
    switch (parameter)
    {
    case Parameter::password:
        password = value;
        write(password, adresses::password);
        break;

    case Parameter::accessPointName:
        accessPointName = value;
        write(accessPointName, adresses::nameAp);
        break;
    }
}

void Storage::write(String value, uint8_t address) noexcept
{
    auto size = value.length();
    EEPROM.write(address, static_cast<uint8_t>(size));
    ++address;

    for (uint16_t offset = 0; offset < size; ++offset)
    {
        EEPROM.write(address + offset, value.charAt(offset));
    }
    EEPROM.commit();
}

void Storage::read() noexcept
{
    mode = static_cast<Mode>(EEPROM.read(adresses::mode));
    
    numberOfImpulsByResolution = EEPROM.readShort(adresses::numberOfImpulsByResolution);
    weelCircle = EEPROM.readShort(adresses::weelCircle);
    maxTownSpeed = EEPROM.readShort(adresses::maxTownSpeed);
    minSpeedOiling = EEPROM.readShort(adresses::minSpeedOiling);
    maxSpeedOiling = EEPROM.readShort(adresses::maxSpeedOiling);
    intervalTownPump1 = EEPROM.readShort(adresses::intervalTownPump1);
    intervalTownPump2 = EEPROM.readShort(adresses::intervalTownPump2);
    intervalRoadPump1 = EEPROM.readShort(adresses::intervalRoadPump1);
    intervalRoadPump2 = EEPROM.readShort(adresses::intervalRoadPump2);
    rainCoef = static_cast<float>(EEPROM.readShort(adresses::rainCoef)) / 100;
    pumpingImpulsInterval = EEPROM.readShort(adresses::pumpingImpulsInterval);
    durationImpuls = EEPROM.readShort(adresses::pumpingImpulsInterval);
    numberOfImpulsInPumping = EEPROM.readShort(adresses::durationImpuls);
    oilImpuls = EEPROM.readShort(adresses::oilImpuls);
    maxNumOfImpuls = EEPROM.readShort(adresses::maxNumOfimpuls);
    
    pumpingImpulsInterval = EEPROM.readShort(adresses::pumpingImpulsInterval);

    read(password, adresses::password);
    read(accessPointName, adresses::nameAp);
}

void Storage::read(String& value, uint8_t address) noexcept
{
    value.clear();
    uint16_t len = static_cast<uint16_t>(EEPROM.read(address));
    ++address;


    for (uint16_t offset = 0; offset < len; ++offset)
    {
        value[offset] = EEPROM.read(address + offset);
    }
}

}
