package valuereader

import (
  "context"
  "fmt"
  "strings"

  "github.com/influxdata/influxdb-client-go/v2"
)

type InfluxdbParams struct {
  InfluxdbURL string
  InfluxdbOrganisation string
  InfluxdbBucket string
  InfluxdbToken string
}

type Sensor struct {
  SensorName string
  SensorValueName string
  SensorValue string
}


func getFluxFilter(influxdbBucket string, sensorNames []Sensor) string {
  var filterArray []string

  for _, sensor := range sensorNames {
    filterArray = append(filterArray, fmt.Sprintf(`(r["_measurement"] == "%s" and r["_field"] == "%s")`, sensor.SensorName, sensor.SensorValueName))
  }

  filterString := strings.Join(filterArray, " or ")

  return fmt.Sprintf(`from(bucket: "%s")
|> range(start:-15m, stop:now())
|> filter(fn: (r) => %s)
|> last()`, influxdbBucket, filterString)
}

func GetLatestValues(influxdbParams InfluxdbParams, sensorNames []Sensor) []Sensor {
  sensors := []Sensor{}

  client := influxdb2.NewClient(influxdbParams.InfluxdbURL, influxdbParams.InfluxdbToken)

  // Get query client
  queryAPI := client.QueryAPI(influxdbParams.InfluxdbOrganisation)
  // get QueryTableResult
  result, err := queryAPI.Query(context.Background(), getFluxFilter(influxdbParams.InfluxdbBucket, sensorNames))

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
      fmt.Printf("error making query: %s\n", result.Err().Error())
  }

  // Ensures background processes finishes
  client.Close()

  return sensors
}
