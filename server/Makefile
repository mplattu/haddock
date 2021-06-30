create-network:
	docker network create haddock_net

clean-network:
	docker network rm haddock_net

create-dev-ntp:
	docker create --name haddock_ntp      \
		--net haddock_net                   \
		--publish 123:123/udp               \
		--cap-add SYS_TIME                  \
		cturra/ntp

create-prod-ntp: create-dev-ntp
	docker update --restart always haddock_ntp

clean-ntp:
	-docker rm haddock_ntp

create-dev-influxdb:
	docker create --name haddock_influxdb \
	  --net haddock_net                   \
		--publish 8086:8086                 \
		quay.io/influxdb/influxdb:v2.0.3 --reporting-disabled

create-prod-influxdb: create-dev-influxdb
	docker update --restart always haddock_influxdb

clean-influxdb:
	-docker rm haddock_influxdb

create-dev-httpd:
	docker build -t haddock_httpd:latest -f Dockerfile.httpd .
	docker create --name haddock_httpd    \
	  --net haddock_net                   \
		--publish 80:80                     \
		haddock_httpd:latest

create-prod-httpd:
	docker update --restart always haddock_httpd

clean-httpd:
	-docker rm haddock_httpd
	-docker rmi haddock_httpd:latest

create-prod: create-prod-ntp create-prod-influxdb create-prod-httpd

create-dev: create-dev-ntp create-dev-influxdb create-dev-httpd

clean: clean-ntp clean-influxdb clean-httpd clean-network

update-milou:
	docker exec haddock_httpd rm -f /var/www/html/milou.cgi
	docker cp ../webclient/bin/milou haddock_httpd:/var/www/html/milou.cgi

update-ota:
	docker exec haddock_httpd rm -fR /var/www/html/ota-files/
	tar --create ota-files/ | docker cp -a - haddock_httpd:/var/www/html

start:
	docker start haddock_ntp
	docker start haddock_influxdb
	docker start haddock_httpd

stop:
	docker stop haddock_ntp
	docker stop haddock_influxdb
	docker stop haddock_httpd
