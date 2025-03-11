#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    const char *fifo_path = "/tmp/myfifo";
    int fifo_fd = open(fifo_path, O_WRONLY);
    if (fifo_fd == -1) {
        perror("Error opening FIFO");
        return 1;
    }

    char buffer[256];
    printf("Enter text to send to the other terminal (type 'exit' to quit):\n");
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
        write(fifo_fd, buffer, strlen(buffer));
    }

    close(fifo_fd);
    return 0;
}