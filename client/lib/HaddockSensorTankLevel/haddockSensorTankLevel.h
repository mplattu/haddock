#ifndef HADDOCK_SENSOR_TANKLEVEL_INTERNAL_AD_H
#define HADDOCK_SENSOR_TANKLEVEL_INTERNAL_AD_H

class HaddockSensorTankLevel
{
  private:
    float _read_min;
    float _read_max;
  public:
    void initialise(float read_min, float read_max);
    float measure();
};

#endif
