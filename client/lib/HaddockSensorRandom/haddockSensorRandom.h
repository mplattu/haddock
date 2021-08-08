#ifndef HADDOCK_SENSOR_RANDOM_H
#define HADDOCK_SENSOR_RANDOM_H

class HaddockSensorRandom
{
  private:
    int _increment;
    int _value;
  public:
    HaddockSensorRandom();
    void initialise();
    float measure();
};

#endif
