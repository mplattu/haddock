/**
   Haddock Sensor: Temperature using MAX6675K

   The class measures voltage using MAX6675K thermocouple sensor

   (C) Matti Lattu 2021
*/

#ifndef HADDOCK_SENSOR_TEMPERATURE
#define HADDOCK_SENSOR_TEMPERATURE

#include <max6675.h>
#include <Wire.h>

#include "haddockSensorTemperatureMAX6675.h"

/*  
 *   int pin_SCLK=14;  // D6
 *   int pin_CS=13;    // D7
 *   int pin_S0=12;    // D8
 */

HaddockSensorTemperatureMAX6675::HaddockSensorTemperatureMAX6675() : _adc(14, 13, 12) {
  // Nothing here
}

void HaddockSensorTemperatureMAX6675::initialise() {
  Wire.begin();

  Serial.println("MAX6675: Initialised");
}

float HaddockSensorTemperatureMAX6675::measure() {
  float measuredTemp = this->_adc.readCelsius();

  Serial.printf("HaddockSensorTemperatureMAX6675 Measured temperature: %f\n", measuredTemp);

  return measuredTemp;
}

#endif
