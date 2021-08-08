# Milou, Haddock Web Client

Milou is a small and simple web client for Haddock written in Go. It serves the latest
values from the sensors. Basically, it reads the last values from InfluxDB and
shows them in a table.

## Configuring, Compiling and Installing

Before compiling make sure you have edited your `settings.mk` as explained in the
general README.

1. Install the new secrets to your running webserver:
  ```
  cd server
  make stop
  make start
  ```
1. Update value `SENSORS` in `webclient/src/milou/milou.go` to point to the sensors
which you want to show on the Web page. You'll find the relevant values from the
`client/src/haddockSettings.cpp`. Example:

  ```
  var SENSORS = []valuereader.Sensor{
    {"Battery1_Voltage", "voltage", ""},
    {"Vattery1_Current", "current", ""},
  }
  ```

1. By default the web server runs in a containerised Linux. Thus, it is enough to
build a Linux version of the Milou.

  Docker builder:

  `make docker`

  Build within your OS:

  `make linux`

1. To install the compiled binary to your running webserver:
```
cd server
make update-milou
```

1. Browse to `http://localhost/` (or whatever your server root is)
