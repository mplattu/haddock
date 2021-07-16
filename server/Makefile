include ../settings.mk

create-network:
	docker network create haddock_net

clean-network:
	-docker network rm haddock_net

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

create-prod-httpd: create-dev-httpd 99-haddock-params.conf
	docker start haddock_httpd
	docker cp 99-haddock-params.conf haddock_httpd:/etc/lighttpd/conf-available/
	docker exec haddock_httpd ln -sf /etc/lighttpd/conf-available/99-haddock-params.conf /etc/lighttpd/conf-enabled/
	docker stop haddock_httpd
	docker update --restart always haddock_httpd

start-httpd: 99-haddock-params.conf
	docker start haddock_httpd
	docker cp 99-haddock-params.conf haddock_httpd:/etc/lighttpd/conf-available/
	docker exec haddock_httpd ln -sf /etc/lighttpd/conf-available/99-haddock-params.conf /etc/lighttpd/conf-enabled/
	docker restart haddock_httpd

clean-httpd:
	-docker rm haddock_httpd
	-docker rmi haddock_httpd:latest

create-prod: create-network create-prod-ntp create-prod-influxdb create-prod-httpd

create-dev: create-network create-dev-ntp create-dev-influxdb create-dev-httpd

clean: clean-ntp clean-influxdb clean-httpd clean-network

99-haddock-params.conf: ../settings.mk
	echo 'server.modules += ( "mod_setenv" )' >99-haddock-params.conf
	echo 'setenv.add-environment = ( "INFLUXDB_TOKEN_READ" => "$(INFLUXDB_TOKEN_READ)",' >>99-haddock-params.conf
	echo '"INFLUXDB_URL" => "$(INFLUXDB_URL)",' >>99-haddock-params.conf
	echo '"INFLUXDB_ORG" => "$(INFLUXDB_ORG)",' >>99-haddock-params.conf
	echo '"INFLUXDB_BUCKET" => "$(INFLUXDB_BUCKET)" )' >>99-haddock-params.conf

update-milou:
	docker exec haddock_httpd rm -f /var/www/html/milou.cgi
	docker cp ../webclient/bin/milou haddock_httpd:/var/www/html/index.cgi

update-ota:
	docker exec haddock_httpd rm -fR /var/www/html/ota-files/
	tar --create ota-files/ | docker cp -a - haddock_httpd:/var/www/html

start: start-httpd
	docker start haddock_ntp
	docker start haddock_influxdb

stop:
	docker stop haddock_ntp
	docker stop haddock_influxdb
	docker stop haddock_httpd
