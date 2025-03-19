CDFLAGS = -std=c99 -Wall -Wextra -pedantic -g -fsanitize=address
CC = clang

all: test_PrototypeRoutes main

test_PrototypeRoutes: PrototypeRoutes.o test_PrototypeRoutes.o
	$(CC) $(CDFLAGS) -o test_PrototypeRoutes PrototypeRoutes.o test_PrototypeRoutes.o

PrototypeRoutes.o: PrototypeRoutes.c PrototypeRoutes.h
	$(CC) -c $(CDFLAGS) PrototypeRoutes.c

test_PrototypeRoutes.o : PrototypeRoutes.h test_PrototypeRoutes.c
	$(CC) -c $(CDFLAGS) test_PrototypeRoutes.c

clean:
	rm -r *.o test_PrototypeRoutes

test : test_PrototypeRoutes
	./test_PrototypeRoutes

main: display.o PrototypeRoutes.o
	$(CC) display.o PrototypeRoutes.o -o main

display.o: display.c
	$(CC) -c $(CFLAGS) display.c -lSDL2

PrototypeRoutes.o: PrototypeRoutes.c
	$(CC) -c $(CFLAGS) PrototypeRoutes.c

