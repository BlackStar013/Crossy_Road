CFLAGS = -Wall -Wextra -std=c99 -pedantic
CC = clang
comp : reader.c writer.c
	$(CC) $(CFLAGS) reader.c -o reader
	$(CC) $(CFLAGS) writer.c -o writer
run_pipe:
	mkfifo /tmp/myfifo
run_writer: writer
	./writer
run_reader: reader
	./reader