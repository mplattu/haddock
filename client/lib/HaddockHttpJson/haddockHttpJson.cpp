#ifndef HADDOCK_HTTP_JSON
#define HADDOCK_HTTP_JSON

#include <string>

#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#include "haddockSignalKTypes.h"
#include "haddockHttpJson.h"

HaddockSignalKJsonDocument HaddockHttpJson::getErrorJson(std::string errorMessage) {
  HaddockSignalKJsonDocument errorJson;

  errorJson["HADDOCK_HTTP_JSON_ERROR"] = errorMessage;

  return errorJson;
}

HaddockSignalKJsonDocument HaddockHttpJson::HttpPostJson(std::string serverUri, std::string serverPath, HaddockSignalKJsonDocument payload) {
  WiFiClient client;

  std::string completeUri = serverUri + serverPath;
  String completeUriString = completeUri.c_str();

  HTTPClient http;
  http.begin(client, completeUriString);
  http.addHeader("Content-Type", "application/json");

  String payloadString;
  serializeJson(payload, payloadString);

  int httpCode = http.POST(payloadString);
  Serial.println("--- SignalK Query (POST):");
  Serial.printf("HTTP URI: %s\n", completeUriString.c_str());
  Serial.printf("HTTP response code: %d\n", httpCode);
  Serial.println(payloadString);

  if (httpCode == -1) {
    http.end();
    return getErrorJson("POST failed");
  }

  String responseContentString = http.getString();
  char* responseContent = strdup(responseContentString.c_str());
  Serial.println("--- SignalK Response:");
  Serial.println(responseContent);
  Serial.println("---");
  http.end();

  HaddockSignalKJsonDocument responseObject;

  DeserializationError error = deserializeJson(responseObject, responseContent);
  if (error) {
    std::string errorString(error.c_str());
    return getErrorJson(errorString);
  }

  return responseObject;
}

HaddockSignalKJsonDocument HaddockHttpJson::HttpGetJson(std::string serverUri, std::string serverPath) {
  WiFiClient client;

  std::string completeUri = serverUri + serverPath;
  String completeUriString = completeUri.c_str();

  HTTPClient http;
  http.begin(client, completeUriString);

  int httpCode = http.GET();
  Serial.println("--- SignalK Query (GET):");
  Serial.printf("HTTP URI: %s\n", completeUriString.c_str());
  Serial.printf("HTTP response code: %d\n", httpCode);

  if (httpCode == -1) {
    http.end();
    return getErrorJson("GET failed");
  }

  String responseContentString = http.getString();
  char* responseContent = strdup(responseContentString.c_str());
  Serial.println("--- SignalK Response:");
  Serial.println(responseContent);
  Serial.println("---");
  http.end();

  HaddockSignalKJsonDocument responseObject;

  DeserializationError error = deserializeJson(responseObject, responseContent);
  if (error) {
    std::string errorString(error.c_str());
    return getErrorJson(errorString);
  }

  return responseObject;
}

#endif
