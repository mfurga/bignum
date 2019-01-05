CC = g++-5
COMPILER_FLAGS = -Wall -Wextra -std=c++14 -Wno-unused-variable
COMPILER_FLAGS_TEST = -g -lgtest -lpthread

DIRS = src/bignum.cc
DIRS_TEST = test/tests.cc

BUILDDIR = build
OUTPUT_TEST = test

all:
	$(CC) $(DIRS) $(COMPILER_FLAGS) -o $(BUILDDIR)/bignum

gtest:
	$(CC) $(DIRS_TEST) $(DIRS) $(COMPILER_FLAGS) $(COMPILER_FLAGS_TEST) -o $(BUILDDIR)/$(OUTPUT_TEST)
	./$(BUILDDIR)/$(OUTPUT_TEST)

run:
	./$(BUILDDIR)/bignum
