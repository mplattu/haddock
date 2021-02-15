/**
   Haddock Sensor Data Sender
   https://github.com/mplattu/haddock
   (C) Matti Lattu 2021
*/

// Comment this to disable WiFi
#define WIFI

#include "sensor.h"

Sensor sensors;
char* WIFI_NAME;
char* WIFI_PASSWORD;

int WAIT_BETWEEN_MEASUREMENTS;

char* NTP_TIMEZONE;
char* NTP_SERVER;

char* INFLUXDB_URL;
char* INFLUXDB_TOKEN;
char* INFLUXDB_ORG;
char* INFLUXDB_BUCKET;
char* INFLUXDB_VAR;

#include "haddock.h"
#include "haddockSettings.h"

// Always include wifi library as we need the mac address of the device to get its settings
#include <ESP8266WiFiMulti.h>

#ifdef WIFI
#include <InfluxDbClient.h>
InfluxDBClient influxDbClient;
ESP8266WiFiMulti WiFiMulti;
#endif

// We read the WiFi MAC (Ethernet) address to this global variable
// It will be used as a key to the settings (see haddockSettings.cpp)
String thisSensorMac;

void setup() {
  haddockSettings();

  influxDbClient.setConnectionParams(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  // Find configuration for this particular device
  thisSensorMac = WiFi.macAddress();

  char myMac[thisSensorMac.length()+1];
  thisSensorMac.toCharArray(myMac, thisSensorMac.length()+1);
  
  Serial.printf("My ethernet address: %s\n", myMac);
  HaddockSensorSettings sensorSettings = sensors.getSensorSettings(thisSensorMac);

  Serial.printf("My name: %s\n", sensorSettings.sensorName);
  Serial.printf("My type: %d\n", sensorSettings.sensorType);

#ifdef WIFI
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
  Serial.print("Connecting to wifi: ");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("connected, IP: ");
  Serial.println(WiFi.localIP());

  configTzTime(NTP_TIMEZONE, NTP_SERVER);

  if (influxDbClient.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(influxDbClient.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(influxDbClient.getLastErrorMessage());
  }
#endif

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  // Initialise sensor using sensor settings
  sensors.initialise(thisSensorMac);

  Serial.println("---Setup finished");
}

void loop() {

#ifdef WIFI
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
#else
  if (1) {
#endif

    digitalWrite(LED_BUILTIN, LOW);

    // Get InfluxDB sensor & variable name for this sensor
    char* sensorName = sensors.getSensorName(thisSensorMac);
    char* variableName = sensors.getSensorVariableName(thisSensorMac);

    // Measure value
    float measurement = sensors.measure(thisSensorMac);

    Serial.printf("%s: %f\n", variableName, measurement);
    
#ifdef WIFI
    // Report measured value to InfluxDB
    Point pointDevice(sensorName);
    pointDevice.addField(variableName, measurement);
    if (! influxDbClient.writePoint(pointDevice)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(influxDbClient.getLastErrorMessage());
    }
#endif

    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(WAIT_BETWEEN_MEASUREMENTS);
}
