/**
   Haddock Sensor Data Sender
   https://github.com/mplattu/haddock
   (C) Matti Lattu 2021
*/

// Comment this to disable WiFi
#define WIFI

#include "haddockSettings.cpp"

#ifdef WIFI
#include <ESP8266WiFiMulti.h>
#include <InfluxDbClient.h>
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
#endif

// For random values
#include "haddockSensorRandom.cpp"
HaddockSensorRandom haddockSensor;

// For measuring voltage using ADS1115
//#include "haddockSensorVoltageADS1115.cpp"
// Resistor values of the voltage divider (R1=82 kOhm, R2=15 kOhm), 0-30V -> 0-4.6V
//HaddockSensorVoltageADS1115 haddockSensor(82000, 15000);

#ifdef WIFI
ESP8266WiFiMulti WiFiMulti;
#endif

void setup() {

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

#ifdef WIFI
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_NAME, WIFI_PASSWORD);
  Serial.print("Connecting to wifi: ");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("connected\n");

  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
#endif

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

  haddockSensor.initialise();
}

void loop() {

#ifdef WIFI
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
#else
  if (1) {
#endif

    digitalWrite(LED_BUILTIN, LOW);

    // Measure value
    Serial.print("Starting to measure...\n");
    float measurement = haddockSensor.measure();
    Serial.printf("Measured value: %f\n", measurement);

#ifdef WIFI
    // Report measured value to InfluxDB
    Point pointDevice("measurement");
    pointDevice.addField(INFLUXDB_VAR, measurement);
    if (! client.writePoint(pointDevice)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }
#endif

    digitalWrite(LED_BUILTIN, HIGH);
  }

  delay(WAIT_BETWEEN_MEASUREMENTS);
}
