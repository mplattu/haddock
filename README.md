# Haddock - Instrument Panel for Your Yacht

The Project Haddock aims to make a data collection and monitoring system for a motor yacht based on the following technologies:
 * [InfluxDB](https://www.influxdata.com/) database and monitoring system executed as a Docker container
 * Sensors running in a [Wemos D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html)

At the moment, following sensors are considered:
 * Voltage and current for monitoring battery level and charging from different sources (shore power, alternator, solar panels)
 * Current for estimating power usage
 * Temperature for monitoring exhaust cooling system
 * Tank levels (fuel, fresh water, septic) using standard resistor-based sensors

The system runs on a Linux laptop which is used also for the navigation.

## Setup (InfluxDB)

 1. Install Docker (e.g. in Debian/Ubuntu `sudo apt install docker.io`).
 1. Start NTP and InfluxDB servers
    ```
    cd server
    make start
    ```
 1. Browse to `http://localhost:8086`
 1. Enter following data:
   * Username: `haddock`
   * Password: whatever password
   * Initial Organization Name: `Haddock`
   * Initial Bucket Name: `haddock`
   * Click "Continue"
   * Click "Configure Later"

## Compiling Sensor Code

 1. Install Arduino IDE.
 1. Add ESP8266 boards definitions to your Arduino IDE.
 1. Select board ESP8266 > Wemos D1 R1.
 1. Install following libraries using Arduino IDE Library Manager:
   * ESP8266 Influxdb
   * ADS1115_WE
 1. Create `client/haddockSettings.cpp` based on this template:
    ```
    #define WIFI_NAME "your_wifi_network_name"
    #define WIFI_PASSWORD "your_wifi_network_password"

    #define WAIT_BETWEEN_MEASUREMENTS 3000

    // InfluxDB 2 server url, e.g. http://192.168.1.48:8086 (Use: InfluxDB UI -> Lo$
    #define INFLUXDB_URL "influxdb-url"
    // InfluxDB 2 server or cloud API authentication token (Use: InfluxDB UI -> Loa$
    // It is a best practice to create a write-only token for your "haddock" bucket
    #define INFLUXDB_TOKEN "token"
    // InfluxDB 2 organization name or id (Use: InfluxDB UI -> Settings -> Profile $
    #define INFLUXDB_ORG "Haddock"
    // InfluxDB 2 bucket name (Use: InfluxDB UI -> Load Data -> Buckets)
    #define INFLUXDB_BUCKET "haddock"
    // InfluxDB variable name (the measurement data will be written using this vari$
    #define INFLUXDB_VAR "your_variable_name"
    ```
