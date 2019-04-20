/* Author: Jacob Dawson */
/* COM S 352 Project 2 */

#include "memory-map-helpers.h"
#define REPORTER 0

int main(void) {
    if (initSemaphore() == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    if (mapFileToMemory(filename) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    int pid = fork();
    pid_t reporter_pid = REPORTER;
    if (pid == REPORTER) {
        bool isFirstRun = true;
        clock_t now = clock(), previous = now;
        unsigned char* vec = (unsigned char*)calloc(1, (fileInfo.st_size + getpagesize() - 1)/getpagesize());
        while (true) {
            now = clock();
            if ((double)(now - previous) >= 10 * CLOCKS_PER_SEC ||
                (isFirstRun && (double)(now - previous) >= 0.25 * CLOCKS_PER_SEC))  {
                isFirstRun = false;
                previous = clock();
                int pagesize = getpagesize();
                cout << "Stats:\nPage size: " << pagesize << endl;
                cout << "Mapped File & mincore output:" << endl;
                semWait();
                printMappedFile();
                mincore(fileData, fileInfo.st_size, vec);
                semSignal();
                for(int block = 0; block <= sizeof(vec)/sizeof(*vec); ++block) {
                    if (vec[block] & 1) {
                        cout << "Block: " << block << endl;
                    }
                }
                cout << "\n\n";
            }
        }
    } else { /* Provider process (the parent process) */
        int resourceIndex = EXIT_FAILURE, unitsAvailableForResource = EXIT_FAILURE;
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
            } else {
                resourceRequestComplete = true;
                kill(reporter_pid, SIGKILL);
            }
        }
    }
    return EXIT_SUCCESS;
}
