#ifndef HADDOCK_SIGNALK_H
#define HADDOCK_SIGNALK_H

#include <string>
#include <Nvm.h>
#include <ArduinoJson.h>

class HaddockSignalK
{
  private:
    std::string _serverUri;
    std::string _sensorDescription;
    Nvm* _nvm;

    std::string getUuid();
    std::string getAccessToken();
    std::string getNvmString(char* keyName);

  public:
    void resetNvm();
    void initialise(char* serverUri, char* sensorDescription);
    bool sendValue(char* sensorName, char* variableName, float measurement);
};

#endif
