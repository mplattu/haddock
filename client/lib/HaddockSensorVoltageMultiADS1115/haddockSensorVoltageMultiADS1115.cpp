/**
   Haddock Sensor: Voltage using ADS1115

   The class measures voltage using ADS1115 AD converter. The resistor values of
   a voltage divider before the AD input can be given as a parameter.
   Refer to: https://ohmslawcalculator.com/voltage-divider-calculator

   This class measures voltage from all ADS1115 pins.

   (C) Matti Lattu 2021
*/

#ifndef HADDOCK_SENSOR_VOLTAGE_MULTI
#define HADDOCK_SENSOR_VOLTAGE_MULTI

#include <ADS1115_WE.h>
#include <Wire.h>

#include "haddockSensorVoltageMultiADS1115.h"

// Resistor values of the voltage divider (R1=82 kOhm, R2=15 kOhm), 0-30V -> 0-4.6V
void HaddockSensorVoltageMultiADS1115::initialise(float r1, float r2) {
  this->_divider = r2 / (r1+r2);

  this->_adc = ADS1115_WE();

  Wire.begin();
  if(!_adc.init()){
    while (1) {
      Serial.println("ADS1115 not connected!");
      delay(1000);
    }
  }

  _adc.setVoltageRange_mV(ADS1115_RANGE_4096);
  _adc.setMeasureMode(ADS1115_CONTINUOUS);

  Serial.println("ADS1115: Initialised");
}

void HaddockSensorVoltageMultiADS1115::setCompareChannels(int pin) {
  switch (pin) {
    case 0:
      _adc.setCompareChannels(ADS1115_COMP_0_GND);
      break;
    case 1:
      _adc.setCompareChannels(ADS1115_COMP_1_GND);
      break;
    case 2:
      Serial.println("Setting pin 2");
      _adc.setCompareChannels(ADS1115_COMP_2_GND);
      break;
    case 3:
      Serial.println("Setting pin 3");
      _adc.setCompareChannels(ADS1115_COMP_3_GND);
      break;
    default:
      Serial.printf("setCompareChannels got unknown pin value: %d\n", pin);
  }
}

float HaddockSensorVoltageMultiADS1115::measure(int pin) {
  // Set current pin
  setCompareChannels(pin);

  // Wait until current compare channels has been set
  delay(100);

  float measuredVoltage = 0.0;
  measuredVoltage = _adc.getResult_V();

  Serial.printf("HaddockSensorVoltageADS1115 Measured voltage: %f from pin %d\n", measuredVoltage, pin);

  return measuredVoltage / _divider;
}

#endif
