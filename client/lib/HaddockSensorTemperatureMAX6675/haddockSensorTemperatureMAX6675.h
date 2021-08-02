#ifndef HADDOCK_SENSOR_TEMPERATURE_H
#define HADDOCK_SENSOR_TEMPERATURE_H

#include <max6675.h>

class HaddockSensorTemperatureMAX6675
{
  private:
    MAX6675 _adc;
  public:
    HaddockSensorTemperatureMAX6675();
    void initialise();
    float measure();
};

#endif
