/* Author: Jacob Dawson */
/* COM S 352 Project 1 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef vector<vector<int> > matrix;

extern void *sort(void *arg);
int squaredTotal;
int phase = 1;
vector<bool> threadsStatus;
matrix matrixToSort;
pthread_mutex_t mutex_lock;
pthread_cond_t controlVar;


/* Given a filename, return the number of ints a file */
/* Parameters: */
    /* string filename - Name of file to read values from */
/* Returns: */
    /* int numInts - The number of integers in the file */
int get_num_ints(string filename) {
    ifstream infile(filename);
    string currentLine;
    int numInts = 0;
    while(getline(infile, currentLine)) {
        istringstream iss(currentLine);
        int curInt;
        while(iss >> curInt) {
            ++numInts;
        }
    }
    return numInts;
}


/* Given a filename, and the sqrt of the num vals to enter in the matrix, */
/* Populate the martix */
/* Parameters: */
    /* stringname filename - the opened file to read values from */
    /* int width - the width of the matrix */
void populate_matrix(string filename, int width) {
    ifstream infile(filename);
    string currentLine;
    while(getline(infile, currentLine)) {
        istringstream iss(currentLine);
        int curInt;
        vector<int> newRow;
        while(iss >> curInt) {
            newRow.push_back(curInt);
        }
        matrixToSort.push_back(newRow);
    }
}


/* Swap two integers */
/* Parameters: */
/*     int *left - first of two integers to swap */
/*     int *right - second of two integers to swap */
void swap(int *left, int *right) {
    int temp = *left;
    *left = *right;
    *right = temp;
}


/* Used to validate if a matrix is sorted properly by shearsort standards. */
/* Parameters: */
/*     int dimension - the size of row or col */
/*     int arr[dimension][dimension] - the 2d square array to validate */
/* Returns: */
/*     bool - true if the array is sorted properly, false otherwise */
bool is_sorted(matrix m) {
    /* Are the columns sorted? */
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal; ++col) {
            if(m[row][col] > m[row + 1][col]) {
                return false;
            }
        }
    }
    /* Are the rows sorted? */
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal - 1; ++col) {
            if(row % 2 == 0) {
                if(m[row][col] > m[row][col + 1]) {
                    return false;
                }
            } else {
                if(m[row][col] < m[row][col + 1]) {
                    return false;
                }
            }
        }
    }
    return true;
}


/* Sort a row by shearsort standards */
/* parameters: */
/*     int rowToSort - the specific row to sort in the global matrix */
void sort_row(int rowToSort) {
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal - row - 1; ++col) {
            if(rowToSort % 2 == 0) {
                if(matrixToSort[rowToSort][col] > matrixToSort[rowToSort][col + 1]) {
                    swap(matrixToSort[rowToSort][col], matrixToSort[rowToSort][col + 1]);
                }
            } else {
                if(matrixToSort[rowToSort][col] < matrixToSort[rowToSort][col + 1]) {
                    swap(matrixToSort[rowToSort][col], matrixToSort[rowToSort][col + 1]);
                }
            }
        }
    }
}


/* Sort a col by shearsort standards */
/* parameters: */
/*     int colToSort - the specific col to sort in the global matrix */
void sort_col(int colToSort) {
    for(int row = 0; row < squaredTotal - 1; ++row) {
        for(int col = 0; col < squaredTotal - row - 1; ++col) {
            if(matrixToSort[col][colToSort] > matrixToSort[col + 1][colToSort]) {
                swap(matrixToSort[col][colToSort], matrixToSort[col + 1][colToSort]);
            }
        }
    }
}


/* Prints the values in a matrix pictorially */
/* Parameters: */
/*     matrix m - The matrix to print the values of */
void print_matrix(matrix m) {
    for(int row = 0; row < squaredTotal; ++row) {
        for(int col = 0; col < squaredTotal - 1; ++col) {
            cout << m[row][col] << ", ";
        }
        cout << m[row][squaredTotal - 1] << "\n";
    }
    cout << "\n";
}


/* Sorts a matrix using threads */
/* Parameters: */
/*     void* arg - Should be passed in of type long, the threadId */
void *sort(void* arg) {
    long temp = (long)arg;
    int id = (int)temp;
    pthread_mutex_lock(&mutex_lock);
    while(phase != 0) {
        if(phase % 2 == 0) {
            sort_col(id);
        } else {
            sort_row(id);
        }
        threadsStatus[id] = true;
        pthread_cond_wait(&controlVar, &mutex_lock);
    }
    pthread_mutex_unlock(&mutex_lock);
    pthread_exit(NULL);
}


/* Returns true if all values in vector are true */
/* Parameters: */
/*     vector<bool> v - Vector to check values of */
/* Returns: */
/*     bool - true if all values are true, false otherwise */
bool threads_complete(vector<bool> v) {
    for(int i = 0; i < v.size(); ++i) {
        if(v[i] == false) {
            return false;
        }
    }
    return true;
}


int main(int argc, char *argv[]) {
    /* Populate 2d array from file */
    string filename = "input.txt";
    squaredTotal = sqrt(get_num_ints(filename));

    /* Allocate space for the 2d array */
    populate_matrix(filename, squaredTotal);
    cout << "\nPopulated matrix: \n";
    print_matrix(matrixToSort);

    /* Create n amount of threads */
    threadsStatus.resize(squaredTotal);
    fill(threadsStatus.begin(), threadsStatus.end(), false);
    pthread_t threads[squaredTotal];
    pthread_mutex_init(&mutex_lock, NULL);
    pthread_cond_init(&controlVar, NULL);
    for(long curThread = 0; curThread < squaredTotal; ++curThread) {
        if((pthread_create(&threads[curThread], NULL, sort, (void *)curThread)) != 0) {
            cout << "Error creating thread: " << curThread << "\n";
            return EXIT_FAILURE;
        }
    }

    /* Sort until the array is sorted properly */
    phase = 1;
    while(!is_sorted(matrixToSort)) {
        while(!threads_complete(threadsStatus)) {}
        cout << "Phase: " << phase << "\n";
        ++phase;
        print_matrix(matrixToSort);
        fill(threadsStatus.begin(), threadsStatus.end(), false);
        pthread_mutex_lock(&mutex_lock);
        pthread_cond_broadcast(&controlVar);
        pthread_mutex_unlock(&mutex_lock);
    }
    phase = 0;
    pthread_mutex_lock(&mutex_lock);
    pthread_cond_broadcast(&controlVar);
    pthread_mutex_unlock(&mutex_lock);

    /* Wait for all threads to complete */
    for(int i = 0; i < squaredTotal; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex_lock);

    return EXIT_SUCCESS;
}
