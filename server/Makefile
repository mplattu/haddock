create:
	docker create --name=ntp      \
		--restart=always         \
		--publish=123:123/udp    \
		--cap-add=SYS_TIME       \
		cturra/ntp
	docker create --name influxdb \
		--publish 8086:8086      \
		quay.io/influxdb/influxdb:v2.0.3 --reporting-disabled

start:
	docker start ntp
	docker start influxdb

stop:
	docker stop ntp
	docker stop influxdb
