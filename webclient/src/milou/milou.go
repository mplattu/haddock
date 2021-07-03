package main

import (
  "fmt"
  "os"
  "strings"

  "milou/valuereader"
)

var INFLUXDB_PARAMS = valuereader.InfluxdbParams{
  os.Getenv("INFLUXDB_URL"),
  os.Getenv("INFLUXDB_ORG"),
  os.Getenv("INFLUXDB_BUCKET"),
  os.Getenv("INFLUXDB_TOKEN_READ"),
}

var SENSORS = []valuereader.Sensor{
  {"Sensor1", "sensor1_valuename", ""},
  {"Sensor2", "sensor2_valuename", ""},
}

func getValueTableHTML() string {
  var lines []string

  lines = append(lines, "Content-Type: text/html; charset=UTF-8\n")
  lines = append(lines, "<html><body>")

  sensorValues := valuereader.GetLatestValues(INFLUXDB_PARAMS, SENSORS)

  lines = append(lines, "<table>")
  for _, sensor := range sensorValues {
    lines = append(lines, fmt.Sprintf("<tr><td>%s</td><td>%s</td><td>%s</td></tr>\n", sensor.SensorName, sensor.SensorValueName, sensor.SensorValue))
  }
  lines = append(lines, "</table>")

  lines = append(lines, "</body></html>")

  return strings.Join(lines, "\n")
}

func main() {
  fmt.Println(getValueTableHTML())
}
