/* Author: Jacob Dawson */
/* COM S 352 Project 2 */

#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

int proccessRes(char* filename);
void listResourceTypes();
bool resourceExists(int resourceToFind);

int numResources, resRowLength = 4, fileDes = 0;
struct stat fileInfo;
char * fileData;

int main(void) {
    proccessRes((char*)("res.txt"));
    cout << "file data:\n" << fileData << endl;

    /* Ask how many units of a resource type is needed */
    bool resourceRequestComplete = false;
    char userInput;
    do {
        cout << "Would you like to make a request to specify desired units of a resource type? (y/n) ";
        cin >> userInput;
        if (userInput == 'y') {
            cout << "\nWhich resource type would you like to make a request from?" << endl;
            listResourceTypes();
            cin >> userInput;
            if (resourceExists(userInput)) {
                cout << "Resource exists buddy" << endl;
            } else {
                cout << "no tengo resource" << endl;
            }
        } else if (userInput == 'n') {
            cout << "no" << endl;
            resourceRequestComplete = true;
        } else {
            cout << "Invalid input. Answer with \'y\' indicating yes, or \'n\' indicating no." << endl;
        }
    } while (!resourceRequestComplete);

    return EXIT_SUCCESS;
}

/* Opens the passed in name of a file and maps it to a memory region */
/* @param char* filename - Name of the file to open. */
/* @returns EXIT_SUCCESS - if res is mapped successfully, EXIT_FAILURE otherwise. */
int proccessRes(char* filename) {
    fileDes = open(filename, O_RDWR);
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
    numResources = fileInfo.st_size / resRowLength;
    close(fileDes);

    return EXIT_SUCCESS;
}

/* Lists the available resource types */
void listResourceTypes() {
    cout << "=================================================" << endl;
    cout << "Available Resources: ";
    for(int resource = 0; resource < numResources; ++resource) {
        cout << fileData[resource * resRowLength];
        if (resource != numResources - 1) {
            cout << ", ";
        } else {
            cout << "\n";
        }
    }
    cout << "=================================================" << endl;
}

bool resourceExists(int resourceToFind) {
    for(int resource = 0; resource < numResources; ++resource) {
        if (resourceToFind == fileData[resource * resRowLength]) {
            return true;
        }
    }
    return false;
}
