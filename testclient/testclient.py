#!/usr/bin/python3.6
# -*- coding: utf-8 -*-

import random

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

INFLUXDB_URL = "http://localhost:8086"
INFLUXDB_ORG = "Haddock"
INFLUXDB_TOKEN = "xULuzDHQVezx_7f0FH4ZTUlmTkZmTK26Xo4N8qFvzisd1NdE4HHX_QIYNzkRJWPvsTfiLaaZhrHG_J6qdKr1sg=="

SENSORS = {
    "Sensor1": "sensor1_valuename",
    "Sensor2": "sensor2_valuename"
}

MY_VERSION="testclient-0.0"

def get_random_sensor_value():
    return random.randint(0,100)

def main():
    client = InfluxDBClient(url=INFLUXDB_URL, token=INFLUXDB_TOKEN, org=INFLUXDB_ORG)
    write_api = client.write_api(write_options=SYNCHRONOUS)

    for this_sensor in SENSORS:
        this_value = get_random_sensor_value()
        print("Sensor name: %s Value: %d" % (this_sensor, this_value))
        write_api.write("haddock", "Haddock", Point(this_sensor).field(SENSORS[this_sensor], this_value).field("version", MY_VERSION))

    write_api.close()
    client.close()

main()
