#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios init_terminal(void){
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    return old_tio;
}

void restore_terminal(struct termios* settings){
    tcsetattr(STDIN_FILENO, TCSANOW, settings);
}

int main() {
    struct termios old_tio = init_terminal();
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
    restore_terminal(&old_tio);

    return 0;
}