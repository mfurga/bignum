.PHONY: build tests

CC = gcc
CFLASG = -O2 -Wall -Wextra -std=c99

BUILD_DIR = build
TESTS_DIR = tests
RANDOM_TESTS_DIR = $(TESTS_DIR)/random

OBJS = $(BUILD_DIR)/bignum.o

build: clean
	$(CC) $(CFLASG) bignum.c -c -o $(BUILD_DIR)/bignum.o

tests: build
	$(CC) $(CFLASG) -I. $(OBJS) $(TESTS_DIR)/unit.c -o $(TESTS_DIR)/unit
	$(TESTS_DIR)/unit

# Random tests based on Python arithmetic.
testsrandom: build
	$(CC) $(CFLASG) -I. $(OBJS) $(RANDOM_TESTS_DIR)/check_tests.c -o $(RANDOM_TESTS_DIR)/check_tests
	$(RANDOM_TESTS_DIR)/run.sh 2>/dev/null

clean:
	rm -rf build
	mkdir build

