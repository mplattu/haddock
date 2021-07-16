# Milou, Haddock Web Client

Milou is a small and simple web client for Haddock written in Go. It serves the latest
values from the sensors. Basically, it reads the last values from InfluxDB and
shows them in a table.

## Configuring, Compiling and Installing

Before compiling:

1. Create a read-enabled token for your InfluxDB (Data > Tokens > Generate).
1. Execute in the top level `INFLUXDB_TOKEN_WRITE=your_sensor_token_you_created_earlier
  INFLUXDB_TOKEN_READ=your_read_token_you_just_created make configure` which updates
  the `settings.mk`
1. Install the new secrets to your running webserver:
  ```
  cd server
  make stop
  make start
  ```
1. Update `SENSORS` to point to the sensors which you want to show on the
Web page.

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
