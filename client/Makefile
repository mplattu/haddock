include ../settings.mk

BUILD_ENV_STR = \
		SERVER_IP=$(SERVER_IP) \
		SIGNALK_URI=$(SIGNALK_URI) \
		SENSOR_WIFI_NAME=$(SENSOR_WIFI_NAME) \
		SENSOR_WIFI_PASSWORD=$(SENSOR_WIFI_PASSWORD)

build:
	$(BUILD_ENV_STR) pio run --environment wemosd1r1

upload:
	$(BUILD_ENV_STR) pio run --environment wemosd1r1 -t upload

monitor:
	pio device monitor

clean:
	rm -fR .pio/
