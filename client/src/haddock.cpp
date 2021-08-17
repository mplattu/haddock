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
char* SIGNALK_SERVER_URI;

int WAIT_BETWEEN_MEASUREMENTS;


char* NTP_TIMEZONE;
char* NTP_SERVER;

char* OTA_SERVER;
int OTA_VERSION;

#include "haddock.h"
#include "haddockSettings.h"
#include "haddockSignalK.h"

// Always include wifi library as we need the mac address of the device to get its settings
#include <ESP8266WiFiMulti.h>

HaddockSignalK sensorKClient;
ESP8266WiFiMulti WiFiMulti;

#ifdef WIFI
// For HTTP OTA updates
#include <ESP8266httpUpdate.h>
#endif

// We read the WiFi MAC (Ethernet) address to this global variable
// It will be used as a key to the settings (see haddockSettings.cpp)
String thisSensorMac;

void haddockSettingsGeneral() {
  WIFI_NAME = SENSOR_WIFI_NAME;
  WIFI_PASSWORD = SENSOR_WIFI_PASSWORD;
  SIGNALK_SERVER_URI = SIGNALK_URI;

  WAIT_BETWEEN_MEASUREMENTS = 1000;

  // Your timezone (https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html)
  NTP_TIMEZONE = "UTC+2";
  // Your NTP server URL (this is where you run your server)
  NTP_SERVER = SERVER_IP;

  // Your OTA server IP
  OTA_SERVER = SERVER_IP;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

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

  haddockSettingsGeneral();
  haddockSettingsSensors();

  // Find configuration for this particular device
  thisSensorMac = WiFi.macAddress();

  char myMac[thisSensorMac.length()+1];
  thisSensorMac.toCharArray(myMac, thisSensorMac.length()+1);

  Serial.printf("My ethernet address: %s\n", myMac);
  HaddockSensorSettings sensorSettings = sensors.getSensorSettings(thisSensorMac);

  if (sensorSettings.sensorType == SensorTypeUnknown) {
    while (1) {
      Serial.printf("This sensor %s has no configured role\n", myMac);
      pinMode(LED_BUILTIN, LOW);
      delay(100);
      pinMode(LED_BUILTIN, HIGH);
      delay(100);
    }
  }

  Serial.printf("My name: %s\n", sensorSettings.sensorName);
  Serial.printf("My type: %d\n", sensorSettings.sensorType);

#ifdef WIFI
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to %s: ", WIFI_NAME);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("connected, IP: ");
  Serial.println(WiFi.localIP());

  configTzTime(NTP_TIMEZONE, NTP_SERVER);

  sensorKClient.initialise(SIGNALK_SERVER_URI, sensorSettings.sensorName);
  //sensorKClient.resetNvm();
#endif

  // Initialise sensor using sensor settings
  sensors.initialise(thisSensorMac);

  Serial.println("---Setup finished");
}

char* getOTAPath(int nextVersion) {
  static char path[128] = {'\0'};
  snprintf(path, sizeof(path), "/ota-files/haddock-%d.bin", nextVersion);
  return path;
}

void loop() {

#ifdef WIFI
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {
#else
  if (1) {
#endif

    digitalWrite(LED_BUILTIN, LOW);

    // Get sensor & variable name for this sensor
    char* sensorName = sensors.getSensorName(thisSensorMac);
    char* variableName = sensors.getSensorVariableName(thisSensorMac);

    // Measure value
    float measurement = sensors.measure(thisSensorMac);

    Serial.printf("%s: %s = %f\n", sensorName, variableName, measurement);

#ifdef WIFI
    // Report measured value to SignalK server
    bool success = sensorKClient.sendValue(sensorName, variableName, measurement);
    if (!success) {
      int n;
      for (n=0; n<50; n++) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
      }
    }

    // Do we have an OTA update?
    WiFiClient client;
    char* otaPath = getOTAPath(OTA_VERSION+1);
    ESPhttpUpdate.update(client, OTA_SERVER, 80, otaPath);
#endif

    digitalWrite(LED_BUILTIN, HIGH);

    sensors.afterMeasure(thisSensorMac);
  }

  delay(WAIT_BETWEEN_MEASUREMENTS);
}
