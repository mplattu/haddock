/**
   Haddock Sensor: Current using ADS1115

   The class measures voltage using ADS1115 AD converter and shunt resistor.
   The resistor drops . The resistor values of
   a voltage divider before the AD input can be given as a parameter.
   Refer to: https://ohmslawcalculator.com/voltage-divider-calculator

   (C) Matti Lattu 2021
*/

#ifndef HADDOCK_SENSOR_CURRENT
#define HADDOCK_SENSOR_CURRENT

#include <Adafruit_ADS1015.h>
#include <Wire.h>

#include "haddockSensorCurrentADS1115.h"

void HaddockSensorCurrentADS1115::initialise(float c) {
  this->_calibration = c;
  
  Wire.begin();

  _adc.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  _adc.begin();

  Serial.println("ADS1115: Initialised");
}

float HaddockSensorCurrentADS1115::measure() {
  int16_t result = _adc.readADC_Differential_0_1(); ;
  float measuredCurrent = ((float)result * 256.0) / 32768.0;
  measuredCurrent = measuredCurrent * _calibration;

  Serial.printf("HaddockSensorCurrentADS1115 Measured current: %f\n", measuredCurrent);

  return measuredCurrent;
}

#endif
