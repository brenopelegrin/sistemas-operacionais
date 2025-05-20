CC = g++
CFLAGS = -Wall -Wno-unused-but-set-variable

TARGET = checkpoint3

default: main

main:
	@echo "\n> compiling main\n"
	$(CC) $(CFLAGS) -o build/main.o src/main.cpp

clean:
	@echo "cleaning build"
	rm -rf ./build/*