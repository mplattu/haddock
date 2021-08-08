#ifndef HADDOCK_SENSOR_CURRENT_H
#define HADDOCK_SENSOR_CURRENT_H

#include <Adafruit_ADS1015.h>

class HaddockSensorCurrentADS1115
{
  private:
    float _calibration;
    Adafruit_ADS1115 _adc;
  public:
    void initialise(float c);
    float measure();
};

#endif
