.PHONY: clean build

CC = gcc
CFLASG = -O2 -Wall -Wextra -std=c99

build:
	$(CC) $(CFLASG) bignum.c main.c -o build/main

run:
	./build/main

clean:
	rm -rf build
	mkdir build

