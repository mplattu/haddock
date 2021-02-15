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
   * Adafruit_ADS1015 (by Adafruit)
   * MAX6675 (by Adafruit)
 1. Create `client/haddockSettings.cpp` based on `client/haddockSettings.cpp.sample`
 1. Open the client directory in the Arduino IDE and upload it to your Wemos D1 board.
 1. Follow the serial output to see the Mac address of the board.
 1. Add the sensor definition for this particular board.
 1. Repeat the sensor confiruration step for each board.
