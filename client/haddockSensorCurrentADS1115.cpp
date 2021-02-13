/**
   Haddock Sensor: Current using ADS1115
   
   The class measures voltage using ADS1115 AD converter and shunt resistor.
   The resistor drops . The resistor values of
   a voltage divider before the AD input can be given as a parameter.
   Refer to: https://ohmslawcalculator.com/voltage-divider-calculator
   
   (C) Matti Lattu 2021
*/

#include <ADS1115_WE.h> 
#include <Wire.h>

class HaddockSensorCurrentADS1115
{
  private:
    float _shuntResistance;
    ADS1115_WE _adc;
  public:
    HaddockSensorCurrentADS1115(float r) {
      this->_shuntResistance = r;
    }

    void initialise() {
      this->_adc = ADS1115_WE();
    
      Wire.begin();
      if(!_adc.init()){
        while (1) {
          Serial.println("ADS1115 not connected!");
          delay(1000);
        }
      }
      
      _adc.setVoltageRange_mV(ADS1115_RANGE_0512);
      _adc.setCompareChannels(ADS1115_COMP_0_1);
      _adc.setMeasureMode(ADS1115_CONTINUOUS);
      _adc.setConvRate(ADS1115_8_SPS);
      
      Serial.println("ADS1115: Initialised");
    }
    
    float measure() {
      float measuredVoltage = 0.0;
      measuredVoltage = _adc.getResult_V();
      float measuredCurrent = measuredVoltage / _shuntResistance/1.5;
      
      Serial.printf("HaddockSensorCurrentADS1115 Measured voltage: %f\n", measuredVoltage);
      Serial.printf("HaddockSensorCurrentADS1115 Measured current: %f\n", measuredCurrent);
    
      return measuredCurrent;
    }
};
