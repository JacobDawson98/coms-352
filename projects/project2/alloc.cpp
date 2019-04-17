/* Author: Jacob Dawson */
/* COM S 352 Project 2 */

#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
/* #include <string> */
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int proccessRes(char* filename);

int fileDes = 0;
struct stat fileInfo;
char* fileData;

int main() {
    proccessRes((char*)("res.txt"));
    cout << "first char of res.txt: "  << fileData << endl;

    return EXIT_SUCCESS;
}

/* Opens the passed in name of a file and maps it to a memory region */
/* @param char* filename - Name of the file to open. */
/* @returns EXIT_SUCCESS - if res is mapped successfully, EXIT_FAILURE otherwise. */
int proccessRes(char* filename) {
    fileDes = open(filename, O_RDONLY);
    if (fileDes == -1) {
        cout << "Null File Descriptor" << endl;

        return EXIT_FAILURE;
    }
    size_t filestat = fstat(fileDes, &fileInfo);

    if (filestat) {
        cout << "fstat error" << endl;
        close(fileDes);

        return EXIT_FAILURE;
    }
    fileData = (char *)mmap(NULL, fileInfo.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileDes, 0);
    close(fileDes);

    return EXIT_SUCCESS;
}
