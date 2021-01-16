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
