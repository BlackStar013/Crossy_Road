CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g -fsanitize=address

all: clean main sdl_main test

main.o: main.c tuiles.h mob.h
	clang -c $(CFLAGS) main.c

main: main.o tuiles.o mob.o map.o core.o
	clang -o main $(CFLAGS) main.o tuiles.o mob.o map.o core.o -lncurses

main_SDL.o: main_SDL.c map.h tuiles.h mob.h display.h
	clang -c $(CFLAGS) main_SDL.c

sdl_main: main_SDL.o tuiles.o mob.o map.o core.o display.o
	clang -o sdl_main $(CFLAGS) main_SDL.o tuiles.o mob.o map.o display.o core.o -lSDL2 -lSDL2_image

map.o: map.c tuiles.h
	clang -c $(CFLAGS) map.c

#fonctions.o: fonctions.c tuiles.h mob.h
#	clang -c $(CFLAGS) fonctions.c

tuiles.o: tuiles.c
	clang -c $(CFLAGS) tuiles.c

mob.o: mob.c
	clang -c $(CFLAGS) mob.c

core.o: core.c
	clang -c $(CFLAGS) core.c

display.o: display.c
	clang -c $(CFLAGS) display.c

test.o: test.c tuiles.h mob.h
	clang -c $(CFLAGS) test.c

test: test.o tuiles.o mob.o map.o core.o
	clang -o test $(CFLAGS) test.o tuiles.o mob.o map.o core.o

clean: 
	rm -f main sdl_main test *.o

run: sdl_main
	./sdl_main

runt : main
	./main


#main.o: main.c fonctions.h tuiles.h mob.h
#	clang -c $(CFLAGS) main.c
#
#main: main.o fonctions.o tuiles.o mob.o 
#	clang -o main $(CFLAGS) main.o fonctions.o tuiles.o mob.o -lSDL2
#
#fonctions.o: fonctions.c tuiles.h mob.h
#	clang -c $(CFLAGS) fonctions.c
#
#tuiles.o: tuiles.c
#	clang -c $(CFLAGS) tuiles.c
#
#mob.o: mob.c
#	clang -c $(CFLAGS) mob.c
#
#test.o: test.c fonctions.h tuiles.h mob.h
#	clang -c $(CFLAGS) test.c
#
#test: test.o
#	clang -o $(CFLAGS) test.o
#
#clean: 
#	rm -f main test *.o


