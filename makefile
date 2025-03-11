CFLAGS = -Wall -Wextra -std=c99 -pedantic
CC = clang
all : display.c
	$(CC) $(CFLAGS) display.c PrototypeRoutes.c -o display -lSDL2 -lncurses
