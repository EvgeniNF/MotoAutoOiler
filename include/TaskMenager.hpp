#pragma once
#include <freertos/FreeRTOS.h>

// Контроль насосов
// Индикация
// Измерение напряжения
// Http server
// Запись настроек

/**
 * Данные для хранения в памяти:
 * 1. MIntervalTown1 
 * 2. MIntervalRoad1 
 * 3. MCoefRain 
 * 4. MMinSpeed 
 * 5. MRoadSpeed   
 * 6. MWeelCircle 
 * 7. MSignalsOn1 
 * 8. DurationImp 
 * 9. MIntervalTown2 
 * 10. MIntervalRoad2 
 * 12. PNumberImp 
 * 13. PImpInterval 
 * 14. REG Режим
 * 15. MaxSpeed 
 * 16. OilFull Количество импульсов из одного полного бачка
 * 17. OilLevel_real Уровень масла в имп
 */

struct PumpingParameters
{
    uint16_t pumpingImpulsInterval{1000}; // Интервал между импульсами при прокачке
    uint16_t durationImpuls{500};         // Длительность импульса во всех режимах
    uint16_t numberOfImpulsInPumping{60}; // Количество импульсов за цикл прокачки
};

struct OilerParamters
{
    uint16_t minIntervalTownPump1 {1400};    // Интервал смазки в режиме Город по пробегу в м для насоса 1
    uint16_t minIntervalTownPump2 {1400};    // Интервал смазки в режиме Город по пробегу в м для насоса 2
    uint16_t minIntervalRoadPump1 {1700};    // Интервал смазки в режиме Трасса по пробегу в м для насоса 1
    uint16_t minIntervalRoadPump2 {1700};    // Интервал смазки в режиме Трасса по пробегу в м для насоса 2

    uint16_t maxTownSpeed{80};               // Скорость смены режима с Города на Трассу
    uint16_t minSpeedOiling{20};             // Минимальная скорость смазки
    uint16_t maxSpeedOiling{180};            // Максимальная скорость смазки
    float rainCoef{0.0};                     // Коэф. увеличения частоты смазки в дождь 
    uint16_t numberOfImpulsByResolution{12}; // Чисто импульсов на оборот колеса
    uint16_t weelCircle{2040};               // Длина окружности колеса 
    uint16_t maxImpulsOil;                   // 
    
    // Часто пишутся
    uint16_t oilLevel;                       // 
    uint8_t mode;                            // После изменения 
};



class Sensor 
{
    public:
    

};

