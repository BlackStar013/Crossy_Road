#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int main() {
    struct termios old_tio, new_tio;

    // Get the current terminal attributes
    tcgetattr(STDIN_FILENO, &old_tio);

    // Make a copy of the current attributes for modification
    new_tio = old_tio;

    // Disable canonical mode and echoing
    new_tio.c_lflag &= ~(ICANON | ECHO);

    // Apply the modified terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    printf("Press any key (press 'q' to quit):\n");
    char ch;
    while (1) {
        // Read a single character from the terminal
        read(STDIN_FILENO, &ch, 1);

        // Check if the 'q' key is pressed to exit the loop
        if (ch == 'q') {
            break;
        }

        // Print the captured key
        printf("Key pressed: %c\n", ch);
    }

    // Restore the original terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return 0;
}