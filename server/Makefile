create:
	docker create --name=ntp      \
		--restart=always         \
		--publish=123:123/udp    \
		--cap-add=SYS_TIME       \
		cturra/ntp
	docker create --name influxdb \
		--publish 8086:8086      \
		quay.io/influxdb/influxdb:v2.0.3 --reporting-disabled
	docker create --name nginx \
		--publish 80:80 \
		nginx

update-ota:
	docker exec nginx rm -fR /usr/share/nginx/html/ota-files/
	tar --create ota-files/ | docker cp -a - nginx:/usr/share/nginx/html

start:
	docker start ntp
	docker start influxdb
	docker start nginx

stop:
	docker stop ntp
	docker stop influxdb
	docker stop nginx
