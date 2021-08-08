#!/usr/bin/python3.6
# -*- coding: utf-8 -*-

import random
import os

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

SENSORS = {
    "Sensor1": "sensor1_valuename",
    "Sensor2": "sensor2_valuename"
}

MY_VERSION="testclient-0.0"

def exit_with_error(message):
    print(message)
    exit(1)

def get_random_sensor_value():
    return random.randint(0,100)

def main():
    try:
        INFLUXDB_URL = "http://"+os.environ["SERVER_IP"]+":8086"
    except KeyError:
        exit_with_error("Please set SERVER_IP environment variable")

    try:
        INFLUXDB_ORG = os.environ["INFLUXDB_ORG"]
    except KeyError:
        exit_with_error("Please set INFLUXDB_ORG environment variable")

    try:
        INFLUXDB_BUCKET = os.environ["INFLUXDB_BUCKET"]
    except KeyError:
        exit_with_error("Please set INFLUXDB_BUCKET environment variable")

    try:
        INFLUXDB_TOKEN_WRITE = os.environ["INFLUXDB_TOKEN_WRITE"]
    except KeyError:
        exit_with_error("Please set INFLUXDB_TOKEN_WRITE environment variable")

    client = InfluxDBClient(url=INFLUXDB_URL, token=INFLUXDB_TOKEN_WRITE, org=INFLUXDB_ORG)
    write_api = client.write_api(write_options=SYNCHRONOUS)

    for this_sensor in SENSORS:
        this_value = get_random_sensor_value()
        print("Sensor name: %s Value: %d" % (this_sensor, this_value))
        write_api.write(INFLUXDB_BUCKET, INFLUXDB_ORG, Point(this_sensor).field(SENSORS[this_sensor], this_value).field("version", MY_VERSION))

    write_api.close()
    client.close()

main()
