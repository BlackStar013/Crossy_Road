CDFLAGS = -std=c99 -Wall -Wextra -pedantic -g -fsanitize=address
CC = clang

all: test_PrototypeRoutes

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