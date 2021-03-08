#ifndef SENSOR_CLASS_H
#define SENSOR_CLASS_H

#define MAX_SENSORS 50

#include "haddockSensorRandom.h"
#include "haddockSensorVoltageADS1115.h"
#include "haddockSensorVoltageMultiADS1115.h"
#include "haddockSensorCurrentADS1115.h"
#include "haddockSensorTemperatureMAX6675.h"

typedef struct {
  char* macAddr;
  char* sensorName;
  char** sensorNames;   // Sensor names for VoltageMulti class
  int sensorType;
  char* sensorVariableName;
  float parameter1;
  float parameter2;
} HaddockSensorSettings;

class Sensor
{
  private:
    int _sensorCount;
    int _sensorVoltageMultiCurrentPin;
    HaddockSensorSettings _sensorSettings[MAX_SENSORS];
    HaddockSensorRandom _sensorRandom;
    HaddockSensorVoltageADS1115 _sensorVoltage;
    HaddockSensorVoltageMultiADS1115 _sensorVoltageMulti;
    HaddockSensorCurrentADS1115 _sensorCurrent;
    HaddockSensorTemperatureMAX6675 _sensorTemperature;

    char** splitSensorNames(char* sensorName);
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
    void afterMeasure(String macAddrStr);
};

#endif
