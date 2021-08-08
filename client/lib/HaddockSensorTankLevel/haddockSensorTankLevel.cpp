/**
   Haddock Sensor: Tank level with ESP8266 internal AD converter
   
   The class measures resistance (0-180 ohm) typically used in 
   tank volume sensors.
   
   (C) Matti Lattu 2021
*/

#ifndef HADDOCK_SENSOR_TANKLEVEL_INTERNAL_AD
#define HADDOCK_SENSOR_TANKLEVEL_INTERNAL_AD

// For pinMode(), analogRead()
#include <Arduino.h>

#include "haddockSensorTankLevel.h"

#define HaddockSensorTankLevel_ADC_Connector A0

// initialise() params must be float
void HaddockSensorTankLevel::initialise(float read_min, float read_max) {
  pinMode(HaddockSensorTankLevel_ADC_Connector, INPUT);
  this->_read_min = (int) read_min;
  this->_read_max = (int) read_max;

  Serial.println("HaddockSensorTankLevel: Initialised");
}

float HaddockSensorTankLevel::measure() {
  int analog_value = analogRead(HaddockSensorTankLevel_ADC_Connector);
  float tank_level = (float) map(analog_value, this->_read_min, this->_read_max, 100, 0);

  Serial.printf("HaddockSensorTankLevel: Measured tank level: %f\n", tank_level);

  return tank_level;
}

#endif
