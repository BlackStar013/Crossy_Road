CFLAGS = -Wall -Wextra -std=c99 -pedantic
CC = clang
all : main

main: display.o PrototypeRoutes.o
	$(CC) display.o PrototypeRoutes.o -o main

display.o: display.c
	$(CC) -c $(CFLAGS) display.c -lSDL2

PrototypeRoutes.o: PrototypeRoutes.c
	$(CC) -c $(CFLAGS) PrototypeRoutes.c

termitest: terminallib.c
	$(CC) $(CFLAGS) -o tlt terminallib.c
