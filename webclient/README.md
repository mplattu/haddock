# Milou, Haddock Web Client

Milou is a small and simple web client for Haddock. It serves the latest
values from the sensors. Basically, it reads the last values from InfluxDB and
shows them in a table.

## Compiling

Before compiling create a write-enabled token for your InfluxDB (Data > Tokens > Generate).
The token string should be placed into the `INFLUXDB_TOKEN` variable.

By default the web server runs in a containerised Linux. Thus, it is enough to
build a Linux version of the Milou.

Docker builder:

`make docker`

Build within your OS:

`make linux`
