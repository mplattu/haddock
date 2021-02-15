/**
   Haddock Sensor: Random
   
   The random sensor class creates random data. It can be used for testing.
   
   (C) Matti Lattu 2021
*/

#ifndef HADDOCK_SENSOR_RANDOM
#define HADDOCK_SENSOR_RANDOM

// For random() function
#include <Arduino.h>

#include <ESP8266WiFiMulti.h>
#include "haddockSensorRandom.h"

HaddockSensorRandom::HaddockSensorRandom() {
  this->_value = 0;
}

void HaddockSensorRandom::initialise() {
  this->_increment = 1;
  this->_value = 0;
  Serial.println("initialise executed");
}

float HaddockSensorRandom::measure() {
  // Possibility 1:50 to change the course of the value
  long randomNumber = random(50);
  if (randomNumber < 1) {
    _increment = -_increment;
  }
  
  _value = _value + _increment;
  
  return (float) _value;
}

#endif
