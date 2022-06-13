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




struct OilerParamters
{
    
    
     
    uint16_t maxImpulsOil;                   // 
    
    // Часто пишутся
    uint16_t oilLevel;                       // 
    uint8_t mode;                            // После изменения 
};



class Sensor 
{
    public:
    

};

