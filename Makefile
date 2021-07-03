configure:
	@if [ "$(INFLUXDB_TOKEN_READ)" = "" ]; then\
		echo "Error: variable INFLUXDB_TOKEN_READ is missing";\
		exit 1;\
	fi

	@if [ "$(INFLUXDB_TOKEN_WRITE)" = "" ]; then\
		echo "Error: variable INFLUXDB_TOKEN_WRITE is missing";\
		exit 1;\
	fi

	echo "INFLUXDB_TOKEN_READ = $(INFLUXDB_TOKEN_READ)" >settings.mk
	echo "INFLUXDB_TOKEN_WRITE = $(INFLUXDB_TOKEN_WRITE)" >>settings.mk
	echo "INFLUXDB_URL = http://haddock_influxdb:8086" >>settings.mk
	echo "INFLUXDB_ORG = Haddock" >>settings.mk
	echo "INFLUXDB_BUCKET = haddock" >>settings.mk
