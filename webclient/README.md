# Milou, Haddock Web Client

Milou is a small and simple web client for Haddock written in Go. It serves the latest
values from the sensors. Basically, it reads the last values from InfluxDB and
shows them in a table.

## Compiling

Before compiling:

1. Create a write-enabled token for your InfluxDB (Data > Tokens > Generate).
The token string should be placed into the `INFLUXDB_TOKEN` variable.
1. Update `SENSORS` to point to the sensors which you want to show on the
Web page.

By default the web server runs in a containerised Linux. Thus, it is enough to
build a Linux version of the Milou.

Docker builder:

`make docker`

Build within your OS:

`make linux`

To install this to your running webserver:
```
cd server
make update-milou
```

Browse to `http://localhost/` (or whatever your server root is)
