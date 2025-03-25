CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -g -fsanitize=address

all: main test

main.o: main.c tuiles.h mob.h
	clang -c $(CFLAGS) main.c

main: main.o tuiles.o mob.o map.o
	clang -o main $(CFLAGS) main.o tuiles.o mob.o map.o -lSDL2

map.o: map.c
	clang -c $(CFLAGS) map.c

#fonctions.o: fonctions.c tuiles.h mob.h
#	clang -c $(CFLAGS) fonctions.c

tuiles.o: tuiles.c
	clang -c $(CFLAGS) tuiles.c

mob.o: mob.c
	clang -c $(CFLAGS) mob.c

test.o: test.c tuiles.h mob.h
	clang -c $(CFLAGS) test.c

test: test.o
	clang -o test $(CFLAGS) test.o

clean: 
	rm -f main test *.o


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
