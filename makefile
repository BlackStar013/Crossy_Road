CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g -fsanitize=address

all: clean main sdl_main test

main.o: main.c tuiles.h mob.h ia.h
	clang -c $(CFLAGS) main.c

main: main.o tuiles.o mob.o map.o core.o ia.o
	clang -o main $(CFLAGS) main.o tuiles.o mob.o map.o core.o ia.o -lncurses

main_SDL.o: main_SDL.c map.h tuiles.h mob.h display.h ia.h
	clang -c $(CFLAGS) main_SDL.c

sdl_main: main_SDL.o tuiles.o mob.o map.o core.o display.o ia.o menu.o SDLdisplay.o save_load.o
	clang -o sdl_main $(CFLAGS) main_SDL.o tuiles.o mob.o menu.o map.o display.o core.o ia.o SDLdisplay.o save_load.o -lSDL2 -lSDL2_image -lSDL2_ttf -lcjson

menu.o: menu.c
	clang -c $(CFLAGS) menu.c

SDLdisplay.o: SDLdisplay.c
	clang -c $(CFLAGS) SDLdisplay.c

run_menu: menu.o
	clang -o menu $(CFLAGS) menu.o -lSDL2 -lSDL2_image -lSDL2_ttf

map.o: map.c tuiles.h
	clang -c $(CFLAGS) map.c

ia.o: ia.c
	clang -c $(CFLAGS) ia.c

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

test: test.o tuiles.o mob.o map.o core.o ia.o
	clang -o test $(CFLAGS) test.o tuiles.o mob.o map.o core.o ia.o

testload: save_load
	./save_load

save_load: save_load.o
	clang -o save_load $(CFLAGS) save_load.o -lcjson

save_load.o: save_load.c
	clang -c $(CFLAGS) save_load.c

clean: 
	rm -f main sdl_main test *.o

run: sdl_main
	./sdl_main

runt : main
	./main

