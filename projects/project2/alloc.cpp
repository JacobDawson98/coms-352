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
int getResourceIndex(int resourceToFind);

int numResources, resRowLength = 4, fileDes = 0;
struct stat fileInfo;
char * fileData;

int main(void) {
    proccessRes((char*)("res.txt"));
    cout << "File data:\n" << fileData << endl;
    int resourceIndex = -1, unitsAvailableForResource = -1;

    /* Ask how many units of a resource type the user wishes to allocate*/
    bool resourceRequestComplete = false;
    while (!resourceRequestComplete) {
        char userInput;
        int desiredUnits;
        cout << "Would you like to make a request to specify desired units of a resource type? (y/n) ";
        cin >> userInput;
        if (userInput == 'y') {
            cout << "\nWhich resource type would you like to make a request from?" << endl;
            listResourceTypes();
            cin >> userInput;
            resourceIndex = getResourceIndex(userInput);
            if (resourceIndex != -1) {
                unitsAvailableForResource = fileData[resourceIndex + 2] - '0';
                cout << "How much of resource " << userInput << " would you like to allocate? (There is/are currently " << unitsAvailableForResource << " units available)." << endl;
                cin >> desiredUnits;
                if (desiredUnits > unitsAvailableForResource) {
                    cout << "You requested too many units from resource " << userInput << ". Resource " << userInput << " has " << unitsAvailableForResource << " resources." << endl;
                } else {
                    unitsAvailableForResource = unitsAvailableForResource - desiredUnits;
                    cout << "Units available for resource " << userInput << " is now " << unitsAvailableForResource << endl;
                }
            } else {
                cout << "Resources " << userInput << " was not found." << endl;
            }
        } else if (userInput == 'n') {
            resourceRequestComplete = true;
        } else {
            cout << "Invalid input. Answer with \'y\' indicating yes, or \'n\' indicating no." << endl;
        }
    }

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

/* Compares given resource to find with resources found in memory mapped file. */
/* Returns the index of the given resource if it is found in the memory mapped file. */
/* Returns -1 otherwise. */
/* @returns int index - the index of the desired resource, -1 if resource is not found. */
int getResourceIndex(int resourceToFind) {
    for(int resource = 0; resource < numResources; ++resource) {
        if (resourceToFind == fileData[resource * resRowLength]) {
            return resource * resRowLength;
        }
    }
    return -1;
}
