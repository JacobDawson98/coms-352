/* Author: Jacob Dawson */
/* COM S 352 Project 2 */

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void proccessRes(string filename);

int main() {
    proccessRes("res.txt");
    return EXIT_SUCCESS;
}

void proccessRes(string filename) {
    ifstream infile(filename);
    string currentLine;
    int resourceType, numUnits;
    while (infile >> resourceType) {
        infile >> numUnits;
    }

    infile.close();
}
