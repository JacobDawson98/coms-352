/* Prompt and ask for the user to enter the name of two files (using their pathnames). */
/* Open and read the characters of the fierst file one character at a time, convert all */
/* the characters to uppercase and write the characters to the second file, and then close the */
/* files. Print out the time spent for the conversion */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_PATH_LENGTH 200

int main(void) {
    char filename[MAX_PATH_LENGTH];
    printf("Enter the path to the file to read from.\n");
    scanf("%s", filename);
    FILE *read = fopen(filename, "r");
    if (read == NULL) {
        perror("Could not read file");
    }
    printf("Enter the path to the file to place the converted characters from the previous file to.\n");
    scanf("%s", filename);
    FILE *write = fopen(filename, "w");
    if (write == NULL) {
        perror("Could not write to file");
    }
    clock_t begin = clock();
    char charRead, temp;
    while (!feof(read)) {
        fread(&charRead, 1, 1, read);
        temp = toupper(charRead);
        fwrite(&temp, 1, 1, write);
    }
    clock_t end = clock();
    printf("Time elapsed is %f seconds.\n", (double)(end - begin) / CLOCKS_PER_SEC);
    fclose(read);
    fclose(write);
}
