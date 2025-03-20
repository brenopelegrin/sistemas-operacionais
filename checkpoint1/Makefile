CC = gcc
CFLAGS = -Wall

TARGET = checkpoint1

default: main

main:
	@echo "\n> Compiling main.c\n"
	$(CC) $(CFLAGS) -o build/main.o src/main.c
	
clean:
	@echo "Cleaning build"
	rm -rf ./build/*