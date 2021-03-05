create:
	docker create --name haddock_ntp      \
		--restart always                    \
		--publish 123:123/udp               \
		--cap-add SYS_TIME                  \
		cturra/ntp
	docker create --name haddock_influxdb \
		--publish 8086:8086                 \
		quay.io/influxdb/influxdb:v2.0.3 --reporting-disabled
	docker create --name haddock_nginx    \
		--publish 80:80                     \
		nginx

update-ota:
	docker exec haddock_nginx rm -fR /usr/share/nginx/html/ota-files/
	tar --create ota-files/ | docker cp -a - haddock_nginx:/usr/share/nginx/html

start:
	docker start haddock_ntp
	docker start haddock_influxdb
	docker start haddock_nginx

stop:
	docker stop haddock_ntp
	docker stop haddock_influxdb
	docker stop haddock_nginx
