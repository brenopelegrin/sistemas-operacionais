CC = gcc
CFLAGS = -Wall

TARGET = checkpoint1

default: main

main:
	@echo "\n> Compiling main.c\n"
	$(CC) $(CFLAGS) -o build/main.o src/main.c

memory:
	@echo "\n> Compiling memory/main.c\n"
	$(CC) $(CFLAGS) -o build/memory.o src/memory/main.c

io-and-files:
	@echo "\n> Compiling io-and-files/main.c\n"
	$(CC) $(CFLAGS) -o build/io-and-files.o src/io-and-files/main.c

processes:
	@echo "\n> Compiling processes/main.c\n"
	$(CC) $(CFLAGS) -o build/processes.o src/processes/main.c

cpu-bound:
	@echo "\n> Compiling cpu-bound/main.c\n"
	$(CC) $(CFLAGS) -o build/cpu-bound.o src/cpu-bound/main.c

io-bound:
	@echo "\n> Compiling io-bound/main.c\n"
	$(CC) $(CFLAGS) -o build/io-bound.o src/io-bound/main.c
	
clean:
	@echo "Cleaning build"
	rm -rf ./build/*