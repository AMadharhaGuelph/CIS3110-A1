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
int fileDescriptor;
char *fileData;

/* FUNCTION PROTOTYPES */
char getMainMenuInput();
void getDisplayMenuInput();
int getFilenameMenuInput();
void readFile();
void displayASCII();
void displayHex();

int main() {
    displayMode = 'a';

    while(true) {
        char selection = getMainMenuInput();
        int response;

        printf("\n");

        switch(selection) {
            case 'o':
                response = getFilenameMenuInput();
                if(response > 0) {
                    readFile();
                    if(displayMode == 'a') {
                        displayASCII();
                    } else {
                        displayHex();
                    }
                }
                break;
            case 'd':
                getDisplayMenuInput();
                break;
            default:
                exit(0);               
                break;
        }
    }

    return 0;
}

char getMainMenuInput() {
    char selection[BUFFER_SIZE];

    if(displayMode == 'a') {
        printf("Current display mode: ASCII\n");
    } else {
        printf("Current display mode: Hex\n");
    }

    printf("\t- Enter \'o\' to enter a file name.\n");
    printf("\t- Enter \'d\' to select a display mode.\n");
    printf("\t- Enter \'x\' to exit.\n");

    do {
        printf("Enter: ");
        fgets(selection, BUFFER_SIZE, stdin);
        selection[strlen(selection)-1] = '\0';
        if(strcmp(selection, "o") != 0 && strcmp(selection, "d") !=0 && strcmp(selection, "x") != 0) {
            printf("Invalid Input.\n");
        }
    } while(strcmp(selection, "o") != 0 && strcmp(selection, "d") !=0 && strcmp(selection, "x") != 0);

    return selection[0];
}

void getDisplayMenuInput() {
    char selection[BUFFER_SIZE];

    printf("Enter display mode:\n");
    printf("\t-Enter \'a\' for ASCII.\n");
    printf("\t-Enter \'h\' for Hex.\n");

    do {
        printf("Enter: ");
        fgets(selection, BUFFER_SIZE, stdin);
        selection[strlen(selection)-1] = '\0';
        if(strcmp(selection, "a") != 0 && strcmp(selection, "h") !=0) {
            printf("Invalid Input.\n");
        }
    } while(strcmp(selection, "a") != 0 && strcmp(selection, "h") !=0);

    printf("\n");

    displayMode = selection[0];
}

int getFilenameMenuInput() {
    printf("Enter file name: ");
    fgets(filename, BUFFER_SIZE, stdin);
    filename[strlen(filename)-1] = '\0';

    fileDescriptor = open(filename, O_RDONLY);

    if(fileDescriptor < 0) {
        printf("Cannot open file %s.\n", filename);
        return -1;
    }

    printf("\n");

    return 1;
}

void readFile() {
    int fileSize;
    int bytes;

    fileSize = lseek(fileDescriptor, 0, SEEK_END);
    fileData = malloc(sizeof(char) * fileSize);

    lseek(fileDescriptor, 0, SEEK_SET);

    while((bytes = read(fileDescriptor, fileData, fileSize)) > 0) {
        if(bytes < 0) {
            printf("Error while reading file.");
        }
    }
}

void displayASCII() {
  
}

void displayHex() {

}