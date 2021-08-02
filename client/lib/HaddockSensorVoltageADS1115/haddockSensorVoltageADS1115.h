#ifndef HADDOCK_SENSOR_VOLTAGE_H
#define HADDOCK_SENSOR_VOLTAGE_H

#include <ADS1115_WE.h> 

class HaddockSensorVoltageADS1115
{
  private:
    float _divider;
    ADS1115_WE _adc;
  public:
    void initialise(float r1, float r2);
    float measure();
};

#endif
