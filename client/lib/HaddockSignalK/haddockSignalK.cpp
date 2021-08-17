/**
   Haddock Sensor: SignalK library

   The implements part of the SignalK protocol 1.5.0.

   It can create access requests and store the token given by the server
   after an approved request.

   After getting the access token it starts to post configured values to
   the server.

   (C) Matti Lattu 2021
*/

#ifndef HADDOCK_SIGNALK
#define HADDOCK_SIGNALK
#define HADDOCK_SIGNALK_JSON_SIZE 256

#include <string>

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include <Nvm.h>
#include <ArduinoJson.h>
#include <ESP8266TrueRandom.h>

#include "haddockSignalK.h"
#include "haddockSignalKTypes.h"
#include "haddockHttpJson.h"

static NvmField nvmFields[] = {
  {"INITIALISED" , "" , 16, 0},
  {"CLIENT_ID"   , "" , 16, 0},
  {"ACCESS_TOKEN", "" , 64, 0},
  {"REQUEST_ID"  , "" , 36, 0},
  {0             , 0  ,  0, 0}, // Mandatory sentinel
};


void HaddockSignalK::resetNvm() {
  this->_nvm->put("INITIALISED", "initialised");
  this->_nvm->put("CLIENT_ID", "-");
  this->_nvm->put("ACCESS_TOKEN", "-");
  this->_nvm->put("REQUEST_ID", "-");
  Serial.printf("HaddockSignalK initialised NVM\n");
}

void HaddockSignalK::initialise(char* serverUri, char* sensorDescription) {
  this->_serverUri = serverUri;
  this->_sensorDescription = sensorDescription;
  this->_nvm = new Nvm(nvmFields);

  std::string ifInitialised = getNvmString("INITIALISED");
  if (ifInitialised == "initialised") {
    Serial.printf("HaddockSignalK found previously initialised NVM\n");
  }
  else {
    resetNvm();
  }
}

std::string HaddockSignalK::getNvmString(char* keyName) {
  char nvmVal[NVM_MAX_LENZ];

  this->_nvm->get(keyName, nvmVal);

  std::string finalVal(nvmVal);

  //Serial.printf("NVM read: %s = %s\n", keyName, finalVal.c_str());

  return finalVal;
}

std::string HaddockSignalK::getUuid() {
    const char *v = "0123456789abcdef";
    const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

    std::string res;
    for (int i = 0; i < 16; i++) {
        if (dash[i]) res += "-";
        res += v[ESP8266TrueRandom.random(0,15)];
    }

    return res;
}

std::string HaddockSignalK::getAccessToken() {
  std::string accessToken = getNvmString("ACCESS_TOKEN");
  if (accessToken == "-") {
    // We don't have a valid access token

    std::string requestID = getNvmString("REQUEST_ID");
    if (requestID == "-") {
      // There is no ongoing request, so we start by making one

      // Make sure we have a client ID
      std::string clientID = getNvmString("CLIENT_ID");
      if (clientID == "-") {
        std::string newClientID = getUuid();
        Serial.printf("SignalK created a new client ID: %s\n", newClientID.c_str());
        this->_nvm->put("CLIENT_ID", newClientID.c_str());
        clientID = newClientID;
      }

      HaddockHttpJson client;
      HaddockSignalKJsonDocument payload;

      payload["clientId"] = clientID;
      payload["description"] = this->_sensorDescription;

      HaddockSignalKJsonDocument response;
      response = client.HttpPostJson(this->_serverUri, "/signalk/v1/access/requests", payload);

      const char* jsonError = response["HADDOCK_HTTP_JSON_ERROR"];
      if (jsonError) {
        Serial.printf("SignalK access request returned an error '%s', starting over...\n", jsonError);
        resetNvm();
        return "-";
      }

      std::string responseState = response["state"];
      if (responseState == "PENDING") {
        const char* responseRequestId = response["requestId"];
        this->_nvm->put("REQUEST_ID", responseRequestId);
        Serial.println("SignalK access request is pending, a new Request ID was stored");
      }
    }
    else {
      // We have an ongoing request, so let's check its status

      HaddockHttpJson client;

      std::string requestPathPrefix = "/signalk/v1/requests/";
      std::string requestPathCombined = requestPathPrefix + requestID;

      HaddockSignalKJsonDocument response;
      response = client.HttpGetJson(this->_serverUri, requestPathCombined.c_str());

      const char* jsonError = response["HADDOCK_HTTP_JSON_ERROR"];
      if (jsonError) {
        Serial.printf("SignalK access status request returned an error '%s', starting over...\n", jsonError);
        resetNvm();
        return "-";
      }

      std::string responseState = response["state"];
      if (responseState == "PENDING") {
        Serial.println("SignalK access request is still pending");
      }
      if (responseState == "COMPLETED") {
        std::string responsePermission = response["accessRequest"]["permission"];
        if (responsePermission == "APPROVED") {
          std::string newAccessToken = response["accessRequest"]["token"];
          this->_nvm->put("ACCESS_TOKEN", newAccessToken.c_str());
          accessToken = newAccessToken;
          Serial.println("SignalK access token was granted by the server and stored");
        }
        if (responsePermission == "DENIED") {
          resetNvm();
          Serial.println("SignalK access was denied - restart request process");
        }
      }
    }
  }

  return accessToken;
}

bool HaddockSignalK::sendValue(char* sensorName, char* variableName, float measurement) {
  std::string accessToken = getAccessToken();

  if (accessToken == "-") {
    Serial.println("SignalK is missing access token, could not send value to server");
    return false;
  }

  Serial.printf("sendValue: %s %s %s %f\n", this->_serverUri.c_str(), sensorName, variableName, measurement);

  return true;
}

#endif
