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
 1. Create InfluxDB tokens and `settings.mk`:
   * Copy `settings.mk.sample` to your local `settings.mk`
   * Create InfluxDB token: "Data" (left navi) > "Tokens" (tab) > "Generate" (button) > "Read/Write Token"
   * Give a description (e.g. "sensors") and create a scoped **write token** for bucket "haddock" > "Save".
     This will be your `INFLUXDB_TOKEN_WRITE`.
   * Create another token (e.g. "webclient") with **read access** for bucket "haddock" which is your
     `INFLUXDB_TOKEN_READ`.
   * Click the token description text and copy the token strings to `settings.mk`
 1. Edit rest of the `settings.mk`:
   * Set `SERVER_IP` which will be used to contact your InfluxDB, NTP and httpd servers (the last for the OTA updates)
   * Set `SENSOR_WIFI_NAME` and `SENSOR_WIFI_PASSWORD` to tell your sensors how to join your WiFi   network

## Compiling Sensor Code

 1. Install [PlatformIO CLI](https://docs.platformio.org/en/latest//core/installation.html).
 1. Make sure command `pio` is executable.
 1. Create `client/src/haddockSettings.cpp` based on `client/src/haddockSettings.cpp.sample`. This is where you define the role and parameters for your sensors.
 1. `make build` compiles the code.
 1. Connect your sensor with USB and upload the code by executing `make upload`
 1. You can monitor the serial output with `make monitor`
 1. Follow the serial output to see the Mac address of the board.
 1. Add the sensor definition for this particular board to `client/src/haddockSettings.cpp`.
 1. Repeat the sensor configuration step for each board.

## Updating code OTA using httpd

The OTA setting is very straightforward. The main loop of the sensor tries to retrieve
new binary from the httpd server. If the file is found it is treated as a new binary
and installed to the ESP8266 sensor.

To update client code:
 1. Increase the `OTA_VERSION` value (defined in `haddockSettings.cpp`) by one.
    This will be the version code of your new updated binary.
 1. Compile a new binary: `cd client && make build`
 1. Copy binary to `server/ota-files/haddock-XXX.bin` where the XXX is your new version
    number. For example:

    `cp client/.pio/build/wemosd1r1/firmware.bin server/ota-files/haddock-2.bin`

 1. To start the upgrade of all sensors you need to copy the content of
    `ota-files/` to httpd container:
    ```
    cd server
    make update-ota
    ```

Make sure you update the `OTA_VERSION` always by one as the Haddock sensor expects
to find the next update from the version code from a following URL:
`http://[SERVER_IP]:80/ota-files/haddock-[OTA_VERSION+1].bin`. It is a good practice
to keep all your upgraded binaries to give outdated sensors a full upgrade path.

## Milou - A Simple Web Client

Milou is a super-simple web client which shows selected values in a simple text
format. It is basically a CGI script which retrieves hard-coded variables from
the InfluxDB. For more information see `webclient/README.md`.
