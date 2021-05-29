TAG=$(shell git describe --tags --always)
VERSION=$(TAG:v%=%)
NAME=atto
REPO=shawntoffel/$(NAME)
CC=gcc
CFLAGS=-Wall -static -s -Os -fno-unwind-tables -fno-asynchronous-unwind-tables
C_FILES := $(wildcard *.c)

.PHONY: all build build-musl docker-build docker-deploy clean

all: bin build 

bin:
	@mkdir bin

build: $(C_FILES)
	$(CC) $(CFLAGS) -o bin/$(NAME) $^

build-musl: $(C_FILES)
	musl-$(CC) $(CFLAGS) -o bin/$(NAME) $^

docker-build:
	docker build -t $(REPO):$(VERSION) .

docker-deploy:
	docker push $(REPO):$(VERSION)

clean:
	@find bin -type f -delete -print