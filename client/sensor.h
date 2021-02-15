#ifndef SENSOR_CLASS_H
#define SENSOR_CLASS_H

#define MAX_SENSORS 50

#include "haddockSensorRandom.h"
#include "haddockSensorVoltageADS1115.h"
#include "haddockSensorCurrentADS1115.h"
#include "haddockSensorTemperatureMAX6675.h"

typedef struct {
  char* macAddr;
  char* sensorName;
  int sensorType;
  char* sensorVariableName;
  float parameter1;
  float parameter2;
} HaddockSensorSettings;

class Sensor
{
  private:
    int _sensorCount;
    HaddockSensorSettings _sensorSettings[MAX_SENSORS];
    HaddockSensorRandom _sensorRandom;
    HaddockSensorVoltageADS1115 _sensorVoltage;
    HaddockSensorCurrentADS1115 _sensorCurrent;
    HaddockSensorTemperatureMAX6675 _sensorTemperature;

  public:
    Sensor();
    void addSensor(char* macAddr, char* sensorName, int sensorType, char* sensorVariableName);
    void addSensor(char* macAddr, char* sensorName, int sensorType, char* sensorVariableName, float parameter1);
    void addSensor(char* macAddr, char* sensorName, int sensorType, char* sensorVariableName, float parameter1, float parameter2);
    HaddockSensorSettings getSensorSettings(String macAddrStr);
    char* getSensorName(String macAddrStr);
    char* getSensorVariableName(String macAddrStr);
    void initialise(String macAddrStr);
    float measure(String macAddrStr);
};

#endif
