#ifndef HADDOCK_SENSOR_VOLTAGE_MULTI_H
#define HADDOCK_SENSOR_VOLTAGE_MULTI_H

#include <ADS1115_WE.h>

class HaddockSensorVoltageMultiADS1115
{
  private:
    float _divider;
    ADS1115_WE _adc;

    void setCompareChannels(int pin);
public:
    void initialise(float r1, float r2);
    float measure(int pin);
    void afterMeasure();
};

#endif
