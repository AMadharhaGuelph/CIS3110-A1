#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>

/* MACROS */
#define BUFFER_SIZE 4096

/* GLOBAL VARIABLES */
char displayMode;
char filename[BUFFER_SIZE];

/* FUNCTION PROTOTYPES */
char getMainMenuInput();
void getDisplayMenuInput();
int getFilenameMenuInput();

int main() {
    displayMode = 'a';

    while(true) {
        char firstOptionSelected = getMainMenuInput();

        write(STDIN_FILENO, "\n", 1);

        switch(firstOptionSelected) {
            case 'o':
                getFilenameMenuInput();
                break;
            case 'd':
                getDisplayMenuInput();
                break;
            default:
                break;
        }
    }

    return 0;
}

char getMainMenuInput() {
    char optionSelected[1];
    int c;

    if(displayMode == 'a') {
        write(STDOUT_FILENO, "Current display mode: ASCII\n", 28);
    } else {
        write(STDOUT_FILENO, "Current display mode: Hex\n", 26);
    }

    write(STDOUT_FILENO, "\t- Enter \'o\' to enter a file name.\n", 35);
    write(STDOUT_FILENO, "\t- Enter \'d\' to select a display mode.\n", 39);
    write(STDOUT_FILENO, "\t- Enter \'x\' to exit.\n", 22);

    do {
        write(STDOUT_FILENO, "Enter (will take first letter as input): ", 41);
        read(STDIN_FILENO, optionSelected, 1);
        while ((c = getchar()) != '\n' && c != EOF);
    } while(optionSelected[0] != 'o' && optionSelected[0] != 'd' && optionSelected[0] != 'x');

    return optionSelected[0];
}

void getDisplayMenuInput() {
    char optionSelected[1];
    int c;

    write(STDOUT_FILENO, "Enter display mode:\n", 20);
    write(STDOUT_FILENO, "\t-Enter \'a\' for ASCII.\n", 23);
    write(STDOUT_FILENO, "\t-Enter \'h\' for Hex.\n", 21);

    do {
        write(STDOUT_FILENO, "Enter (will take first letter as input): ", 41);
        read(STDIN_FILENO, optionSelected, 1);
        while ((c = getchar()) != '\n' && c != EOF);
    } while(optionSelected[0] != 'a' && optionSelected[0] != 'h');

    displayMode = optionSelected[0];
}

int getFilenameMenuInput() {
    size_t n;
    int fd;

    write(STDOUT_FILENO, "Enter file name: ", 17);
    n = read(STDIN_FILENO, filename, BUFFER_SIZE);

    filename[n-1] = '\0';

    fd = open(filename, O_RDONLY);

    if(fd < 0) {
        char errorMsg* = malloc(18+n);
        errorMsg = "Cannot open file " + filename + ".";
        write(STDOUT_FILENO, errorMsg, );
        return -1;
    }

    return 1;
}
