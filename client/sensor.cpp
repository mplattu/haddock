#ifndef HADDOCK_SENSOR_CLASS
#define HADDOCK_SENSOR_CLASS

#include <ESP8266WiFiMulti.h>
#include "haddock.h"
#include "sensor.h"

Sensor::Sensor() {
  this->_sensorCount=0;
}

char** Sensor::splitSensorNames(char* sensorName) {
  static char** names = new char* [4];
  for (int n=0; n < 4; n++) {
    names[n] = new char[128];
    strcpy(names[n], "");
  }
  
  int strIndex = 0;
  char newName[128];
  
  for (int n=0; n < strlen(sensorName); n++) {
      if (sensorName[n] == ',') {
          strIndex++;
      }
    else {
      int strLength = strlen(names[strIndex]);

      strcpy(newName, names[strIndex]);
      newName[strLength] = sensorName[n];
      newName[strLength+1] = '\0';
      strcpy(names[strIndex], newName);
    }
  }

  return names;
}

void Sensor::addSensor(char* macAddr, char* sensorName, int sensorType, char* sensorVariableName) {
  if (_sensorCount == MAX_SENSORS-1) {
    // Cannot add more sensors
    return;
  }

  _sensorCount++;
  _sensorSettings[_sensorCount-1] = {macAddr, sensorName, NULL, sensorType, sensorVariableName, 0, 0};
}

void Sensor::addSensor(char* macAddr, char* sensorName, int sensorType, char* sensorVariableName, float parameter1) {
  addSensor(macAddr, sensorName, sensorType, sensorVariableName);
  _sensorSettings[_sensorCount-1].parameter1 = parameter1;
}

void Sensor::addSensor(char* macAddr, char* sensorName, int sensorType, char* sensorVariableName, float parameter1, float parameter2) {
  addSensor(macAddr, sensorName, sensorType, sensorVariableName);
  _sensorSettings[_sensorCount-1].parameter1 = parameter1;
  _sensorSettings[_sensorCount-1].parameter2 = parameter2;

  if (sensorType == SensorTypeVoltageMulti) {
    static char** sensorNames = splitSensorNames(sensorName);
    
    _sensorSettings[_sensorCount-1].sensorNames = sensorNames;
  }
}


HaddockSensorSettings Sensor::getSensorSettings(String macAddrStr) {
  char macAddr[macAddrStr.length()+1];
  macAddrStr.toCharArray(macAddr, macAddrStr.length()+1);

  for (int n=0; n < _sensorCount; n++) {
    if (strcmp(_sensorSettings[n].macAddr, macAddr) == 0) {
      return _sensorSettings[n];
    }
  }

  return {"00:00:00:00:00:00", "Unknown", NULL, SensorTypeUnknown, "var_unknown", 0, 0};
}

char* Sensor::getSensorName(String macAddrStr) {
  HaddockSensorSettings sensorSettings = getSensorSettings(macAddrStr);

  if (sensorSettings.sensorType == SensorTypeVoltageMulti) {
    return sensorSettings.sensorNames[_sensorVoltageMultiCurrentPin];
  }


  return sensorSettings.sensorName;
}

char* Sensor::getSensorVariableName(String macAddrStr) {
  HaddockSensorSettings sensorSettings = getSensorSettings(macAddrStr);
  return sensorSettings.sensorVariableName;
}

void Sensor::initialise(String macAddrStr) {
  HaddockSensorSettings sensorSettings = getSensorSettings(macAddrStr);

  switch (sensorSettings.sensorType) {
    case SensorTypeUnknown:
      Serial.println("Cannot initialise an unknown sensor");
      return;
    case SensorTypeCurrent:
      _sensorCurrent.initialise(sensorSettings.parameter1);
      return;
    case SensorTypeVoltage:
      _sensorVoltage.initialise(sensorSettings.parameter1, sensorSettings.parameter2);
      return;
    case SensorTypeVoltageMulti:
      _sensorVoltageMulti.initialise(sensorSettings.parameter1, sensorSettings.parameter2);
      _sensorVoltageMultiCurrentPin = 0;
      return;
    case SensorTypeTemperature:
      _sensorTemperature.initialise();
      return;
    case SensorTypeRandom:
      _sensorRandom.initialise();
      return;
    case SensorTypeTankLevel:
      _sensorTankLevel.initialise(sensorSettings.parameter1, sensorSettings.parameter2);
      return;
  }

  Serial.printf("There is no initialisation rule for sensor type %d", sensorSettings.sensorType);
}

float Sensor::measure(String macAddrStr) {
  HaddockSensorSettings sensorSettings = getSensorSettings(macAddrStr);

  switch (sensorSettings.sensorType) {
    case SensorTypeUnknown:
      Serial.println("Cannot measure an unknown sensor");
      return 0.0;
    case SensorTypeCurrent:
      return _sensorCurrent.measure();
    case SensorTypeVoltage:
      return _sensorVoltage.measure();
    case SensorTypeVoltageMulti:
      return _sensorVoltageMulti.measure(_sensorVoltageMultiCurrentPin);
    case SensorTypeTemperature:
      return _sensorTemperature.measure();
    case SensorTypeRandom:
      return _sensorRandom.measure();
    case SensorTypeTankLevel:
      return _sensorTankLevel.measure();
  }

  Serial.printf("There is no measurement rule for sensor type %d\n", sensorSettings.sensorType);
  return 0.0;
}

void Sensor::afterMeasure(String macAddrStr) {
  HaddockSensorSettings sensorSettings = getSensorSettings(macAddrStr);

  // Only VoltageMulti needs attention after measurement
  if (sensorSettings.sensorType == SensorTypeVoltageMulti) {
    // Add pin pointer by one so we have a new pin on the next round
    _sensorVoltageMultiCurrentPin++;
    _sensorVoltageMultiCurrentPin = _sensorVoltageMultiCurrentPin % 4;
  }

}

#endif HADDOCK_SENSOR_CLASS
