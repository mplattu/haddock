current_dir = $(shell pwd)
GO=go
TESTS=milou
SOURCES=$(wildcard src/**/*.go)

bin/golangci-lint:
	curl -sSfL https://raw.githubusercontent.com/golangci/golangci-lint/master/install.sh | sh -s -- -b ./bin v1.30.0

bin/go2xunit:
	go get github.com/tebeka/go2xunit

checkstyle: bin/golangci-lint
	-cd src/milou && GOOS=linux GOARCH=amd64 CGO_ENABLED=1 ../../bin/golangci-lint run --timeout 5m0s --out-format checkstyle > $(current_dir)/checkstyle-linux.xml

lint: bin/golangci-lint
	cd src/milou && ../../bin/golangci-lint run --out-format checkstyle

ci-test: bin/go2xunit
	echo "There are not tests at the moment" >$(current_dir)/tests.xml
	#cd src/milou && 2>&1 go test -v $(TESTS) | ../../bin/go2xunit -output $(current_dir)/tests.xml

test:
	# There are no tests at the moment
	#cd src/milou && go test $(TESTS)

docker: clean
	mkdir -p bin
	-docker rm milou-build
	docker build -t milou-build-img:latest -f Dockerfile.build .
	docker run -w /app --name milou-build milou-build-img:latest make ci-test
	docker cp milou-build:/app/checkstyle-linux.xml .
	docker cp milou-build:/app/tests.xml .
	docker cp milou-build:/app/bin/milou bin/milou
	docker cp milou-build:/app/src/milou/go.sum ./src/milou/
	docker cp milou-build:/app/src/milou/go.mod ./src/milou/

all: test linux

linux: milou

milou: src/*
	cd src/milou && GOOS=linux GOARCH=amd64 CGO_ENABLED=1 $(GO) build -o ../../bin/milou milou

clean:
	rm -f bin/milou
	rm -f tests.xml
	if [ -d pkg/ ]; then chmod -R 777 pkg/; rm -fR pkg/; fi
