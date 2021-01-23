/**
   Haddock Sensor: Temperature using MAX6675K
   
   The class measures voltage using MAX6675K thermocouple sensor
   
   (C) Matti Lattu 2021
*/

#include <max6675.h> 
#include <Wire.h>

class HaddockSensorTemperatureMAX6675
{
  int pin_SCLK=14;
  int pin_CS=13;
  int pin_S0=12;
  
  private:
    MAX6675 _adc;
  public:
    HaddockSensorTemperatureMAX6675() : _adc(pin_SCLK, pin_CS, pin_S0) {
      // Nothing here
    }

    void initialise() {
      Wire.begin();
    
      Serial.println("MAX6675: Initialised");
    }
    
    float measure() {
      float measuredTemp = this->_adc.readCelsius();
      
      Serial.printf("HaddockSensorTemperatureMAX6675 Measured temperature: %f\n", measuredTemp);
    
      return measuredTemp;
    }
};
