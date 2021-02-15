#ifndef HADDOCK_HEADERS
#define HADDOCK_HEADERS

const int SensorTypeUnknown = 0;
const int SensorTypeCurrent = 1;
const int SensorTypeVoltage = 2;
const int SensorTypeTemperature = 3;
const int SensorTypeRandom = 99;

extern char* WIFI_NAME;
extern char* WIFI_PASSWORD;

extern int WAIT_BETWEEN_MEASUREMENTS;

extern char* NTP_TIMEZONE;
extern char* NTP_SERVER;

extern char* INFLUXDB_URL;
extern char* INFLUXDB_TOKEN;
extern char* INFLUXDB_ORG;
extern char* INFLUXDB_BUCKET;
extern char* INFLUXDB_VAR;

#endif
