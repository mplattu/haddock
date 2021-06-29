package main

import (
  "context"
  "fmt"
  "strings"

  "github.com/influxdata/influxdb-client-go/v2"
)

const INFLUXDB_ORG = "Haddock"
const INFLUXDB_BUCKET = "haddock"
const INFLUXDB_TOKEN = "qVz53e-uVXaIC-_ME1YNr9iDSynYgApRQNrRQdYiP4kK2DPF55h5KgeuVusyMB1pt8j9IeENxTuhcJ-e-N21sw=="

type Sensor struct {
  SensorName string
  SensorValueName string
  SensorValue string
}

var SENSORS = []Sensor{
  {"Sensor1", "sensor1_valuename", ""},
  {"Sensor2", "sensor2_valuename", ""},
}

func getFluxFilter(sensorNames []Sensor) string {
  var filterArray []string

  for _, sensor := range sensorNames {
    filterArray = append(filterArray, fmt.Sprintf(`(r["_measurement"] == "%s" and r["_field"] == "%s")`, sensor.SensorName, sensor.SensorValueName))
  }

  filterString := strings.Join(filterArray, " or ")

  return fmt.Sprintf(`from(bucket: "%s")
|> range(start:-15m, stop:now())
|> filter(fn: (r) => %s)
|> last()`, INFLUXDB_BUCKET, filterString)
}

func getLatestValues(sensorNames []Sensor) []Sensor {
  sensors := []Sensor{}

  // Create a new client using an InfluxDB server base URL and an authentication token
  client := influxdb2.NewClient("http://localhost:8086", INFLUXDB_TOKEN)
  // Get query client
  queryAPI := client.QueryAPI(INFLUXDB_ORG)
  // get QueryTableResult
  //result, err := queryAPI.Query(context.Background(), `from(bucket:"haddock")|> range(start: -1h) |> filter(fn: (r) => r._measurement == "stat")`)
  result, err := queryAPI.Query(context.Background(), getFluxFilter(sensorNames))

  if err == nil {
      // Iterate over query response
      for result.Next() {
          // Access data
          newSensor := Sensor{SensorName:result.Record().Measurement(), SensorValueName:result.Record().Field(), SensorValue:fmt.Sprintf("%v", result.Record().Value())}
          sensors = append(sensors, newSensor)
      }
      // check for an error
      if result.Err() != nil {
          fmt.Printf("query parsing error: %s\n", result.Err().Error())
      }
  } else {
      panic(err)
  }
  // Ensures background processes finishes
  client.Close()

  return sensors
}


func main() {
  fmt.Print("Content-type: text/html\n\n")
  fmt.Println("<html><body>")

  sensorValues := getLatestValues(SENSORS)

  fmt.Println("<table>")
  for _, sensor := range sensorValues {
    fmt.Printf("<tr><td>%s</td><td>%s</td><td>%s</td></tr>\n", sensor.SensorName, sensor.SensorValueName, sensor.SensorValue)
  }
  fmt.Println("</table>")

  fmt.Println("</body></html>")
}
