CC = gcc
CFLAGS = -Wall

TARGET = checkpoint2

default: main

main: lib-queuelkdlist
	@echo "\n> compiling main\n"
	$(CC) $(CFLAGS) -I lib-queuelkdlist/ -o build/main.o src/main.c -lm build/lib-queuelkdlist.o

lib-queuelkdlist:
	@echo "\n> compiling lib-queuelkdlist\n"
	$(CC) $(CFLAGS) -c -o build/lib-queuelkdlist.o src/lib-queuelkdlist/queuelkdlist.c
	
clean:
	@echo "cleaning build"
	rm -rf ./build/*