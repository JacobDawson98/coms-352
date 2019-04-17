#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define KEY_alloc 0x1111
#define KEY_prov 0x2222
#define KEY_rep 0x3333

using namespace std;

int numResources = -1, resRowLength = 4;
char * fileData;
struct stat fileInfo;

/* semaphore variables */
union semUn {
    int val;
    struct semi_ds *buf;
    unsigned short array;
};
int semid, semid_prov, semid_rep;
struct sembuf w, s;
union semUn semaphoreUnion;

int getResourceIndex(int resourceToFind), mapFileToMemory(char* filename),
    initSemaphore(), semWait(), semSignal();
void printMappedFile();

/* Opens the passed in name of a file and maps it to a memory region */
/* @param char* filename - Name of the file to open. */
/* @returns EXIT_SUCCESS - if res is mapped successfully, EXIT_FAILURE otherwise. */
int mapFileToMemory(char* filename) {
    int fileDes = open(filename, O_RDWR);
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

/* Compares given resource to find with resources found in memory mapped file. */
/* Returns the index of the given resource if it is found in the memory mapped file. */
/* Returns -1 otherwise. */
/* @returns int index - the index of the desired resource, -1 if resource is not found. */
int getResourceIndex(int resourceToFind) {
    if (numResources == -1) {
        mapFileToMemory((char *)("res.txt"));
    }
    for(int resource = 0; resource < numResources; ++resource) {
        if (resourceToFind == fileData[resource * resRowLength]) {
            return resource * resRowLength;
        }
    }
    return EXIT_FAILURE;
}

/* Pretty prints file that is mapped to memory */
void printMappedFile() {
    if (numResources == -1) {
        mapFileToMemory((char *)("res.txt"));
    }
    cout << "\nR       U\n=========\n" << fileData << endl;
}

/* Initialize sempahore */
int initSemaphore() {
    /* The assigning of w and s here only work only big daddy linux */
    w = { 0, -1, 0 }; // semwait
    s = { 0, +1, 0 }; // semsignal

    /* Create semaphore */
    if ((semid = semget(KEY_alloc, 1, 0666 | IPC_CREAT)) < 0) {
        cout << "Unable to create semaphore" << endl;
        return EXIT_FAILURE;
    }

    /* Initialize semaphore to a counter of 1 in parent */
    semaphoreUnion.val = 1;
    if (semctl(semid, 0, SETVAL, semaphoreUnion) < 0) {
        cout << "Unable to initialize semaphore." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* Set the semaphore to wait */
int semWait() {
    if (semop(semid, &w, 1) < 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/* Signal sempahore */
int semSignal() {
    if (semop(semid, &s, 1) < 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
