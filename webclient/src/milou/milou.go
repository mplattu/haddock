package main

import (
  "fmt"
  
  "milou/valuereader"
)

const INFLUXDB_URL = "http://localhost:8086"
const INFLUXDB_ORG = "Haddock"
const INFLUXDB_BUCKET = "haddock"
const INFLUXDB_TOKEN = "joUlJ9vmLXppW0zlMz6hL5u4yUP1qtRcoQJZJUVw7SCnT0XNokrOKIqG-4eOIpaZvpJB7IjXzjpa-dr6G9JSAA=="

var INFLUXDB_PARAMS = valuereader.InfluxdbParams{
  INFLUXDB_URL,
  INFLUXDB_ORG,
  INFLUXDB_BUCKET,
  INFLUXDB_TOKEN,
}

var SENSORS = []valuereader.Sensor{
  {"Sensor1", "sensor1_valuename", ""},
  {"Sensor2", "sensor2_valuename", ""},
}

func main() {
  fmt.Print("Content-type: text/html\n\n")
  fmt.Println("<html><body>")

  sensorValues := valuereader.GetLatestValues(INFLUXDB_PARAMS, SENSORS)

  fmt.Println("<table>")
  for _, sensor := range sensorValues {
    fmt.Printf("<tr><td>%s</td><td>%s</td><td>%s</td></tr>\n", sensor.SensorName, sensor.SensorValueName, sensor.SensorValue)
  }
  fmt.Println("</table>")

  fmt.Println("</body></html>")
}
