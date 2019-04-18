/* Author: Jacob Dawson */
/* COM S 352 Project 2 */

#include "common.h"


int main(void) {
    mapFileToMemory((char*)("res.txt"));
    int resourceIndex = -1, unitsAvailableForResource = -1;

    /* Ask how many units of a resource type the user wishes to allocate*/
    bool resourceRequestComplete = false;
    while (!resourceRequestComplete) {
        char userInput;
        int desiredUnits;
        cout << "Would you like to make a request to specify desired units of a resource type? (y/n) ";
        cin >> userInput;
        if (userInput == 'y') {
            cout << "Which resource type would you like to make a request from?" << endl;
            printMappedFile();
            cin >> userInput;

            semWait();
            resourceIndex = getResourceIndex(userInput);
            semSignal();
            if (resourceIndex != EXIT_FAILURE) {
                semWait();
                unitsAvailableForResource = fileData[resourceIndex + 2] - '0';
                semSignal();
                cout << "How much of resource " << userInput << " would you like to allocate? (There is/are currently " << unitsAvailableForResource << " units available)." << endl;
                cin >> desiredUnits;
                if (desiredUnits > unitsAvailableForResource) {
                    cout << "You requested too many units from resource " << userInput << ". Resource " << userInput << " has " << unitsAvailableForResource << " resources." << endl;
                } else {
                    semWait();
                    fileData[resourceIndex + 2] = ('0' + unitsAvailableForResource - desiredUnits);
                    msync(fileData, fileInfo.st_size, MS_SYNC);
                    semSignal();
                    cout << "Units available for resource " << userInput << " is now " << unitsAvailableForResource - desiredUnits << endl;
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

