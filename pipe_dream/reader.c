#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifo_path = "/tmp/myfifo";
    int fifo_fd = open(fifo_path, O_RDONLY);
    if (fifo_fd == -1) {
        perror("Error opening FIFO");
        return 1;
    }

    char buffer[256];
    while (1) {
        int bytes_read = read(fifo_fd, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }
        buffer[bytes_read] = '\0';
        printf("Received: %s", buffer);
    }

    close(fifo_fd);
    return 0;
}