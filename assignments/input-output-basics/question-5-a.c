/* Prompt and ask for the user to enter the name of two files (using their pathnames). */
/* Open and read the characters of the fierst file one character at a time, convert all */
/* the characters to uppercase and write the characters to the second file, and then close the */
/* files. Print out the time spent for the conversion */
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_PATH_LENGTH 50

int main(void) {
    int fdRead, fdWrite;
    char filename[MAX_PATH_LENGTH];

    printf("Enter the path to the file to read from.\n");
    scanf("%s", filename);
    fdRead = open(filename, O_RDONLY);
    if (fdRead == EXIT_FAILURE) {
        perror("Could not open file to read.");
    }
    printf("Enter the path to the file to place the converted characters from the previous file to.\n");
    scanf("%s", filename);
    fdWrite = open(filename, O_WRONLY);
    if (fdWrite == EXIT_FAILURE) {
        perror("Could not write to file");
    }
    clock_t begin = clock();
    char charRead;
    while (read(fdRead, &charRead, 1) != 0) {
        write(fdWrite, &charRead, 1);
    }
    clock_t end = clock();
    printf("Total time spent was %lf seconds.\n", (double)(end - begin) / CLOCKS_PER_SEC);
    close(fdRead);
    close(fdWrite);
}
