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
int fileSize;
unsigned char *fileData;

/* FUNCTION PROTOTYPES */
char getMainMenuInput();
void getDisplayMenuInput();
int getFilenameMenuInput();
void readFile();
void displayASCII();
void displayHex();
char getAfterDisplayMenuInput();
void exitProgram();

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
                    selection = getAfterDisplayMenuInput();
                    if(selection == 'm') {
                        close(fileDescriptor);
                    } else {
                        exitProgram();
                    }
                }
                break;
            case 'd':
                getDisplayMenuInput();
                break;
            default:
                exitProgram();               
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
        printf("Cannot open file %s.\n\n", filename);
        return -1;
    }

    printf("\n");

    return 1;
}

void readFile() {
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
    for(int i = 0; i < fileSize; i++) {
        if((fileData[i] >= 0x0 && fileData[i] <= 0x9) || (fileData[i] >= 0xB && fileData[i] <= 0x1F)) {
            fileData[i] = 0x20;
        }
        if(fileData[i] >= 0x7F) {
            fileData[i] = 0x3F;
        }
        printf("%c", fileData[i]);
    }

    printf("\n\n");
}

void displayHex() {
    int lines = (fileSize/16) + ((fileSize % 16) != 0) + 1;
    int counter = 0;
    int indexCount = 0;
    int prevIndex = 0;

    for(int i = 0; i < lines; i++) {
        printf("%08x", (indexCount+prevIndex));
        prevIndex += indexCount;
        indexCount = 0;
        for(int i = 0; i < 16; i++) {
            if(fileData[i+counter] == '\0') {
                break;
            }
            if(i != 0 && (i+counter) % 8 == 0) {
                printf(" ");
            }
            indexCount++;
            printf(" %02x", fileData[i+counter]);
        }
        printf("\n");
        counter += 16;
    }
    printf("\n");
}

char getAfterDisplayMenuInput() {
    char selection[BUFFER_SIZE];

    printf("Would you like to continue?\n");
    printf("\t-Enter \'m\' to return to the main menu.\n");
    printf("\t-Enter \'x\' to exit.\n");

    do {
        printf("Enter: ");
        fgets(selection, BUFFER_SIZE, stdin);
        selection[strlen(selection)-1] = '\0';
        if(strcmp(selection, "m") != 0 && strcmp(selection, "x") !=0) {
            printf("Invalid Input.\n");
        }
    } while(strcmp(selection, "m") != 0 && strcmp(selection, "x") !=0);
    printf("\n");

    return selection[0];
}

void exitProgram() {
    close(fileDescriptor);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    exit(0);
}