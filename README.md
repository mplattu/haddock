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
 1. Create the Docker containers for NTP, InfluxDB and nginx
    ```
    cd server
    sudo make create-prod
    ```
    If you're creating containers for development, say `make create-dev` in order not to start
    the containers automatically.
 1. In case you did `create-dev` start NTP, InfluxDB and nginx servers
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
 1. Add a token for your sensors:
   * "Data" (left navi) > "Tokens" (tab) > "Generate" (button) > "Read/Write Token"
   * Give a description (e.g. "sensors") and create a scoped write token for bucket "haddock" > "Save"
   * Click the token description and copy the token string to your `haddockSettings.cpp` for the
     `INFLUXDB_TOKEN` value.

## Compiling Sensor Code

 1. Install Arduino IDE.
 1. Add ESP8266 boards definitions to your Arduino IDE
   * File > Preferences > Additional Boards Manager URLs: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   * Tools > Board > Boards Manager > Install "esp8266" by "ESP8266 Community" version 2.7.4
 1. Select board ESP8266 () > Wemos D1 R1.
 1. Install following libraries using Arduino IDE Library Manager (Tools > Manage Libraries):
   * ESP8266 Influxdb (3.7.0)
   * ADS1115_WE (1.2.3)
   * Adafruit_ADS1015 (by Adafruit, 1.1.2)
   * MAX6675 (by Adafruit, 1.1.0)
 1. Create `client/haddockSettings.cpp` based on `client/haddockSettings.cpp.sample`
 1. Open the client directory in the Arduino IDE and upload it to your Wemos D1 board.
 1. Follow the serial output to see the Mac address of the board.
 1. Add the sensor definition for this particular board.
 1. Repeat the sensor confiruration step for each board.

## Updating code OTA using nginx

The OTA setting is very straightforward. The main loop of the sensor tries to retrieve
new binary from the nginx server. If the file is found it is treated as a new binary
and installed to the ESP8266 sensor.

To update client code:
 1. Increase the `OTA_VERSION` value (defined in `haddockSettings.cpp`) by one.
    This will be the version code of your new updated binary.
 1. Compile a new binary with the Arduino IDE: Sketch > Export compiled Binary.
 1. Copy binary to `server/ota-files/haddock-XXX.bin` where the XXX is your new version
    number. For example:

    `cp client/haddock.ino.d1.bin server/ota-files/haddock-2.bin`

 1. To start the upgrade of all sensors you need to copy the content of
    `ota-files/` to nginx container:
    ```
    cd server
    make update-ota
    ```

Make sure you update the `OTA_VERSION` always by one as the Haddock sensor expects
to find the next update from the version code from a following URL:
`http://[OTA_SERVER]:80/ota-files/haddock-[OTA_VERSION+1].bin`. It is a good practice
to keep all your upgraded binaries to give outdated sensors a full upgrade path.
