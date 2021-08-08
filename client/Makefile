include ../settings.mk

BUILD_ENV_STR = INFLUXDB_TOKEN_WRITE=$(INFLUXDB_TOKEN_WRITE) \
		SERVER_IP=$(SERVER_IP) \
		SENSOR_WIFI_NAME=$(SENSOR_WIFI_NAME) \
		SENSOR_WIFI_PASSWORD=$(SENSOR_WIFI_PASSWORD) \
		INFLUXDB_ORG=$(INFLUXDB_ORG) \
		INFLUXDB_BUCKET=$(INFLUXDB_BUCKET)

build:
	$(BUILD_ENV_STR) pio run --environment wemosd1r1

upload:
	$(BUILD_ENV_STR) pio run --environment wemosd1r1 -t upload

monitor:
	pio device monitor
