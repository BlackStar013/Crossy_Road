CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g -fsanitize=address

all: clean main sdl_main test

main.o: main.c
	clang -c $(CFLAGS) main.c

main: main.o menu.o
	clang -o main $(CFLAGS) main.o menu.o

menu.o: menu.c
	clang -c $(CFLAGS) menu.c

clean: 
	rm -f main sdl_main test *.o

run: sdl_main
	./sdl_main