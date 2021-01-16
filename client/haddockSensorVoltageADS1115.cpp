/**
   Haddock Sensor: Voltage using ADS1115
   
   The class measures voltage using ADS1115 AD converter. The resistor values of
   a voltage divider before the AD input can be given as a parameter.
   Refer to: https://ohmslawcalculator.com/voltage-divider-calculator
   
   (C) Matti Lattu 2021
*/

#include <ADS1115_WE.h> 
#include <Wire.h>

class HaddockSensorVoltageADS1115
{
  private:
    float _divider;
    ADS1115_WE _adc;
  public:
    HaddockSensorVoltageADS1115(float r1, float r2) {
      this->_divider = r2 / (r1+r2);
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
      
      _adc.setVoltageRange_mV(ADS1115_RANGE_4096);
      _adc.setCompareChannels(ADS1115_COMP_0_GND);
      _adc.setMeasureMode(ADS1115_CONTINUOUS);
    
      Serial.println("ADS1115: Initialised");
    }
    
    float measure() {
      float measuredVoltage = 0.0;
      measuredVoltage = _adc.getResult_V();
      
      Serial.printf("HaddockSensorVoltageADS1115Measured voltage: %f\n", measuredVoltage);
    
      return measuredVoltage / _divider;
    }
};
