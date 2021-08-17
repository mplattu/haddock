#ifndef HADDOCK_HTTP_JSON_H
#define HADDOCK_HTTP_JSON_H

#include <string>

#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
#include <SPI.h>

#include "haddockSignalKTypes.h"

class HaddockHttpJson
{
  private:
    HaddockSignalKJsonDocument getErrorJson(
      std::string errorMessage
    );
  public:
    HaddockSignalKJsonDocument HttpPostJson(std::string serverUri, std::string serverPath, HaddockSignalKJsonDocument payload);
    HaddockSignalKJsonDocument HttpGetJson(std::string serverUri, std::string serverPath);
};

#endif
