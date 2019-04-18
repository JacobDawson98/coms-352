/* Author: Jacob Dawson */
/* COM S 352 Project 2 */

#include "common.h"
#define REPORTER 0

int main(void) {
    if (initSemaphore() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (mapFileToMemory(filename) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    int pid = fork();
    if (pid == REPORTER) {

    } else { /* Provider process (the parent process) */
        /* * In a loop, it keeps asking whether new resources need to be added. */
        /* * If yes, */
        /* * it receives from the user input, the resource type and the number of units, and */
        /*   adds them to the memory region. */
        /*   Once added, using system call `msync()`, it synchronizes the content of the memory */
        /* region with the physical file. */
        int resourceIndex = -1, unitsAvailableForResource = -1;
        bool resourceRequestComplete = false;
        while(!resourceRequestComplete) {
            char userInput;
            int desiredUnitsToAdd;
            cout << "Would you like to make a request to specify which resource to add units to? (y/n) ";
            cin >> userInput;
            if (userInput == 'y') {
                cout << "Which resource type would you like to add units to?" << endl;
                semWait();
                printMappedFile();
                semSignal();
                cin >> userInput;

                semWait();
                resourceIndex = getResourceIndex(userInput);
                semSignal();
                if (resourceIndex != EXIT_FAILURE) {
                    cout << "How many units would you like to add to resource " << userInput << " ?";
                    cin >> desiredUnitsToAdd;
                    semWait();
                    unitsAvailableForResource = fileData[resourceIndex + 2] - '0';
                    fileData[resourceIndex + 2] = ('0' + unitsAvailableForResource + desiredUnitsToAdd);
                    msync(fileData, fileInfo.st_size, MS_SYNC);
                    semSignal();
                }
            }
        }
    }
    return EXIT_SUCCESS;
}
